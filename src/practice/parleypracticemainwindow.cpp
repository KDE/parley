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
#include <KDebug>
#include <KStandardDirs>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>
#include <QGraphicsProxyWidget>
#include <KAction>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KApplication>

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
#include <keduvocdocument.h>


ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{
    setupBase();
    setupActions();
    setupModeSpecifics();

    setupGUI(Default, QString::fromLatin1("parleypracticeui.rc"));

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();

    m_view->show();
}


void ParleyPracticeMainWindow::slotCheckAnswer(const QString& input)
{
    m_validator->slotCheckAnswer(m_manager->currentSolution(), input);
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
void ParleyPracticeMainWindow::slotToggleCheckAnswerContinueActions()
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

    emit signalCheckAnswerContinueActionsToggled();
}

// This has all the GUI independent action setup code.
void ParleyPracticeMainWindow::setupActions()
{
    KAction *skipKnownAction = new KAction(this);
    skipKnownAction->setText(i18n("Skip (Answer Known)"));
    actionCollection()->addAction("skip known", skipKnownAction);
    connect(skipKnownAction, SIGNAL(triggered()), m_stats, SLOT(slotSkippedKnown()));
    if (!Prefs::skipKnownEnabled())
    {
        skipKnownAction->setVisible(false);
    }

    KAction *skipUnknownAction = new KAction(this);
    skipUnknownAction->setText(i18n("Skip (Answer Not Known)"));
    actionCollection()->addAction("skip unknown", skipUnknownAction);
    connect(skipUnknownAction, SIGNAL(triggered()), m_stats, SLOT(slotSkippedUnknown()));

    KAction *showSolutionAction = new KAction(this);
    showSolutionAction->setText(i18n("Show Solution"));
    actionCollection()->addAction("show solution", showSolutionAction);
    connect(showSolutionAction, SIGNAL(triggered()), m_stats, SLOT(slotSolutionShown()));
    connect(showSolutionAction, SIGNAL(triggered()), this, SLOT(slotShowSolution()));

    KAction *checkAnswerAction = new KAction(this);
    checkAnswerAction->setText(i18n("Check Answer"));
    actionCollection()->addAction("check answer", checkAnswerAction);
    connect(checkAnswerAction, SIGNAL(triggered()), this, SLOT(slotToggleCheckAnswerContinueActions()));
    checkAnswerAction->setVisible(true);

    KAction *continueAction = new KAction(this);
    continueAction->setText(i18n("Continue"));
    actionCollection()->addAction("continue", continueAction);
    connect(continueAction, SIGNAL(triggered()), m_manager, SLOT(slotNewEntry()));
    connect(continueAction, SIGNAL(triggered()), this, SLOT(slotToggleCheckAnswerContinueActions()));
    continueAction->setVisible(false);

    KAction *hintAction = new KAction(this);
    hintAction->setText(i18n("Show Hint"));
    actionCollection()->addAction("hint", hintAction);
    if (!Prefs::showHints())
        hintAction->setVisible(false);

    KStandardAction::preferences(this, SLOT(slotCreatePreferencesDialog()),
                                       actionCollection());

    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    connect(m_stats, SIGNAL(signalQuit()), kapp, SLOT(quit()));
}

void ParleyPracticeMainWindow::setupBase()
{
    m_view = new PracticeView;
    setCentralWidget(m_view);

    QGraphicsScene* m_scene = new QGraphicsScene(this);
    m_view->setScene(m_scene);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     QGraphicsSvgItem * backgroundsvg = new QGraphicsSvgItem();
     m_renderer = new KSvgRenderer();
     KGameTheme kgtheme;
     kDebug() << "kgametheme valid:" << kgtheme.load("parley/themes/default.desktop");
     kDebug() << "graphics svg path" << kgtheme.graphics();
     m_renderer->load(kgtheme.graphics());
     backgroundsvg->setSharedRenderer(m_renderer);
     m_scene->addItem(backgroundsvg);

    m_scene->setSceneRect(backgroundsvg->boundingRect());
    m_view->setSceneRect(m_scene->sceneRect());

    m_manager = new PracticeEntryManager(this);

    m_stats = new Statistics(m_manager, this);
    connect(m_manager, SIGNAL(signalEntryChanged(PracticeEntry*)), m_stats, SLOT(slotSetEntry(PracticeEntry*)));
    connect(m_manager, SIGNAL(signalSetFinished()), m_stats, SLOT(slotSetFinished()));
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
    kDebug() << "file open code:" << code;

    m_manager->open(doc);


    m_validator = new AnswerValidator(doc, this);
    m_validator->setLanguage(Prefs::solutionLanguage());
    connect(m_validator, SIGNAL(signalCorrection(float, Statistics::ErrorType, const QString&)), m_stats, SLOT(slotCorrection(float, Statistics::ErrorType, const QString&)));
}


void ParleyPracticeMainWindow::setupModeSpecifics()
{

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_view, "image_box");
    m_scene->addWidget(iprompt);
    connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));

    SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_view, "image_box");
    m_scene->addWidget(sprompt);
    connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));

    TextualInput * input = new TextualInput(m_renderer, m_view, "practice_text_translation_background");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(m_validator, SIGNAL(signalCorrection(float, Statistics::ErrorType, const QString&)), input, SLOT(slotChangeAnswerColor(float)));
    connect(m_manager, SIGNAL(signalShowSolution(const QString&)), input, SLOT(slotShowSolution(const QString&)));


    SvgBarStatistics * barstats = new SvgBarStatistics(m_renderer, "bar", "bar_background");
    m_scene->addItem(barstats);
    connect(m_stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));

    StdButton * stdbutton = new StdButton("Check Answer", m_renderer, m_view, "check_answer_and_continue_button");
    QGraphicsProxyWidget * gstdbutton = m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled()), stdbutton, SLOT(slotToggleText()));
    gstdbutton->setVisible(true); // enable for now
    connect(actionCollection()->action("show solution"), SIGNAL(triggered()), stdbutton, SLOT(slotSolutionShown()));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));

    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));


    if (Prefs::practiceTimeout() && Prefs::practiceTimeoutTimePerAnswer()) // timeout can't be 0
    {
        kDebug() << "timer" << Prefs::practiceTimeout() << Prefs::practiceTimeoutTimePerAnswer();
        InvisibleTimer * timer = new InvisibleTimer(this);
        timer->setLength(Prefs::practiceTimeoutTimePerAnswer()*1000); // seconds -> milliseconds
        // when the timer triggers, it will assume their current input is their answer
        connect(timer, SIGNAL(signalTimeout()), actionCollection()->action("check answer"), SIGNAL(triggered()));
        connect(m_manager, SIGNAL(signalNewEntry()), timer, SLOT(slotStart()));
        connect(input, SIGNAL(signalInput(const QString&)), timer, SLOT(slotStop()));
    }

}