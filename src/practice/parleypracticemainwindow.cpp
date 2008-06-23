/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "parleypracticemainwindow.h"

#include <KXmlGuiWindow>
#include <KLineEdit>
#include <KDebug>
#include <KStandardDirs>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QLCDNumber>
#include <KFileDialog>
#include <QPushButton>
#include <QGraphicsLinearLayout>
#include <KAction>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KApplication>
#include <QTimer>

#include "practiceview.h"
#include "input.h"
#include "prompt.h"
#include "statistics.h"
#include "stdbuttons.h"
#include "hint.h"
#include "practiceentrymanager.h"
#include "answervalidator.h"
#include "prefs.h"
#include "timer.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"


#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <keduvocdocument.h>


ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{

    //// Basic setup ////
    m_view = new PracticeView;
    setCentralWidget(m_view);

    QGraphicsScene* scene = new QGraphicsScene(this);
    m_view->setScene(scene);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //KGameThemeSelector

     QGraphicsSvgItem * backgroundsvg = new QGraphicsSvgItem();
     KSvgRenderer * krenderer = new KSvgRenderer();
     KGameTheme kgtheme;
     kDebug() << "kgametheme valid:" << kgtheme.load("parley/themes/default.desktop");
     kDebug() << "graphics svg path" << kgtheme.graphics();
     krenderer->load(kgtheme.graphics());
     backgroundsvg->setSharedRenderer(krenderer);
     scene->addItem(backgroundsvg);
     m_backgroundRect = backgroundsvg->boundingRect();

    scene->setSceneRect(m_backgroundRect);
    m_view->setSceneRect(scene->sceneRect());

    //// Loading the Document -- temporary ////
    KEduVocDocument * doc = new KEduVocDocument(this);

    // for the fun of it - use parleyrc
    kDebug() << "open file from parleyrc";
    KConfig parleyConfig("parleyrc");
    kDebug() << "groupList" << parleyConfig.groupList();
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
    // take the last file, but there are File1..n and Name1..n entries..
    QString sourceFile = recentFilesGroup.readEntry(recentFilesGroup.keyList().value(recentFilesGroup.keyList().count() / 2 - 1), QString());

    int code = doc->open(sourceFile);
    kDebug() << code;

    // this is the only object/widget the window directly keeps track of (outside of the canvas, etc).
    m_manager = new PracticeEntryManager(this);
    m_manager->open(doc);


    ////// Graphical Widgets Setup //////

    TextualPrompt * tprompt = new TextualPrompt(krenderer, "practice_text_background");
    scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    ImagePrompt * iprompt = new ImagePrompt(krenderer, m_view, "image_box");
    scene->addWidget(iprompt);
    connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));

    SoundPrompt * sprompt = new SoundPrompt(krenderer, m_view, "image_box");
    scene->addWidget(sprompt);
    connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));

    TextualInput * input = new TextualInput(krenderer, m_view, "practice_text_translation_background");
    scene->addWidget(input);

    Statistics * stats = new Statistics(m_manager, this);
    SvgBarStatistics * barstats = new SvgBarStatistics(krenderer, "bar", "bar_background");
    scene->addItem(barstats);
    connect(stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));
    connect(m_manager, SIGNAL(signalExpressionChanged(KEduVocExpression*)), stats, SLOT(slotSetExpression(KEduVocExpression*)));
    connect(m_manager, SIGNAL(signalSetFinished()), stats, SLOT(slotSetFinished()));


    StdButton * stdbutton = new StdButton("Check Answer", krenderer, m_view, "check_answer_and_continue_button");
    QGraphicsProxyWidget * gstdbutton = scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    gstdbutton->setVisible(true); // enable for now


    //// Input and Validation Setup ////
    AnswerValidator * validator = new AnswerValidator(doc, this);
    validator->setLanguage(Prefs::solutionLanguage());
    connect(input, SIGNAL(signalInput(const QString&)), this, SLOT(slotGetInput(const QString&)));
    connect(this, SIGNAL(signalCheckInput(const QString&, const QString&)), validator, SLOT(checkUserAnswer(const QString&, const QString&)));
    connect(validator, SIGNAL(signalCorrection(float, Statistics::ErrorType, const QString&)), stats, SLOT(slotCorrection(float, Statistics::ErrorType, const QString&)));
    connect(validator, SIGNAL(signalCorrection(float, Statistics::ErrorType, const QString&)), input, SLOT(slotChangeAnswerColor(float)));

    /////////// KAction Setup /////////////

    //// Skip Action Setup ////
    KAction *skipKnownAction = new KAction(this);
    skipKnownAction->setText(i18n("Skip (Answer Known)"));
    actionCollection()->addAction("skip known", skipKnownAction);
    connect(skipKnownAction, SIGNAL(triggered()), stats, SLOT(slotSkippedKnown()));

    KAction *skipUnknownAction = new KAction(this);
    skipUnknownAction->setText(i18n("Skip (Answer Not Known)"));
    actionCollection()->addAction("skip unknown", skipUnknownAction);
    connect(skipUnknownAction, SIGNAL(triggered()), stats, SLOT(slotSkippedUnknown()));


    //// Show Solution Setup ////
    KAction *showSolutionAction = new KAction(this);
    showSolutionAction->setText(i18n("Show Solution"));
    actionCollection()->addAction("show solution", showSolutionAction);
    connect(showSolutionAction, SIGNAL(triggered()), stats, SLOT(slotSolutionShown()));
    connect(showSolutionAction, SIGNAL(triggered()), stdbutton, SLOT(slotSolutionShown()));
    connect(showSolutionAction, SIGNAL(triggered()), this, SLOT(slotShowSolution()));
    connect(this, SIGNAL(signalShowSolution(const QString&)), input, SLOT(slotShowSolution(const QString&)));
    connect(stdbutton, SIGNAL(signalToggleContinueShowAnswerActions()), this, SLOT(slotToggleShowSolutionContinueActions()));

    //// Hint + Hint Action Setup ////
    Hint * hint = new Hint(this);
    KAction *hintAction = new KAction(this);
    hintAction->setText(i18n("Show Hint"));
    actionCollection()->addAction("hint", hintAction);
    connect(hintAction, SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    connect(hint, SIGNAL(signalShowHint()), showSolutionAction, SIGNAL(triggered())); // this is the hint for now :)
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));

    //// Check Answer Setup ////
    KAction *checkAnswerAction = new KAction(this);
    checkAnswerAction->setText(i18n("Check Answer"));
    actionCollection()->addAction("check answer", checkAnswerAction);
    connect(stdbutton, SIGNAL(signalCheckAnswer()), checkAnswerAction, SIGNAL(triggered()));
    connect(checkAnswerAction, SIGNAL(triggered()), input, SLOT(slotEmitCurrentInput()));
    connect(checkAnswerAction, SIGNAL(triggered()), this, SLOT(slotToggleShowSolutionContinueActions()));
    connect(checkAnswerAction, SIGNAL(triggered()), stdbutton, SLOT(slotToggleText()));

    checkAnswerAction->setVisible(true);

    //// Continue Action Setup ////
    KAction *continueAction = new KAction(this);
    continueAction->setText(i18n("Continue"));
    actionCollection()->addAction("continue", continueAction);
    connect(stdbutton, SIGNAL(signalContinue()), continueAction, SIGNAL(triggered()));
    connect(continueAction, SIGNAL(triggered()), m_manager, SLOT(slotNewEntry()));
    connect(continueAction, SIGNAL(triggered()), this, SLOT(slotToggleShowSolutionContinueActions()));
    connect(continueAction, SIGNAL(triggered()), stdbutton, SLOT(slotToggleText()));
    connect(continueAction, SIGNAL(triggered()), input, SLOT(slotClear()));
    continueAction->setVisible(false);

    //// Prefs Action Setup ///
    KStandardAction::preferences(this, SLOT(slotCreatePreferencesDialog()),
                                       actionCollection());

    //// Quit action setup ////
    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    connect(stats, SIGNAL(signalQuit()), kapp, SLOT(quit()));


    //// Timer setup ////
    if (Prefs::practiceTimeout() && Prefs::practiceTimeoutTimePerAnswer()) // timeout can't be 0
    {
        kDebug() << "timer" << Prefs::practiceTimeout() << Prefs::practiceTimeoutTimePerAnswer();
        InvisibleTimer * timer = new InvisibleTimer(this);
        timer->setLength(Prefs::practiceTimeoutTimePerAnswer()*1000); // seconds -> milliseconds
        // when the timer triggers, it will assume their current input is their answer
        connect(timer, SIGNAL(signalTimeout()), checkAnswerAction, SIGNAL(triggered()));
        connect(m_manager, SIGNAL(signalNewEntry()), timer, SLOT(slotStart()));
        connect(input, SIGNAL(signalInput(const QString&)), timer, SLOT(slotStop()));
    }


    //// Final Graphics Setup ////

    setupGUI(Default, QString::fromLatin1("parleypracticeui.rc"));

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();

    m_view->show();
}


void ParleyPracticeMainWindow::slotGetInput(const QString& input)
{
    emit signalCheckInput(m_manager->currentSolution(), input);
}

void ParleyPracticeMainWindow::slotShowSolution()
{
    emit signalShowSolution(m_manager->currentSolution());
}

void ParleyPracticeMainWindow::slotCreatePreferencesDialog()
{
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Prefs::self());
    dialog->addPage(new KGameThemeSelector(dialog, Prefs::self()), i18n("Theme"), "game_theme");
    kDebug() << "hit";
    dialog->show();
}

// this one is a mouthful...
void ParleyPracticeMainWindow::slotToggleShowSolutionContinueActions()
{
    // This simply toggles between the two actions since both shouldn't be shown at the same time.
    QAction* showAnswerAction;
    showAnswerAction = actionCollection()->action("check answer");

    if (showAnswerAction->isVisible())
    {
        showAnswerAction->setVisible(false);
        actionCollection()->action("continue")->setVisible(true);
    }
    else
    {
        showAnswerAction->setVisible(true);
        actionCollection()->action("continue")->setVisible(false);
    }
}
