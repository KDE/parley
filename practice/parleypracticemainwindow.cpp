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

#include "input.h"
#include "prompt.h"
#include "statistics.h"
#include "stdbuttons.h"
#include "hint.h"
#include "testentrymanager.h"
#include "answervalidator.h"
#include "practiceprefs.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"



#include "../../libkdeedu/keduvocdocument/keduvocexpression.h"
#include "../../libkdeedu/keduvocdocument/keduvoctranslation.h"
#include "../../libkdeedu/keduvocdocument/keduvocdocument.h"

ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{

    //// Basic setup ////
    m_view = new QGraphicsView;
    setCentralWidget(m_view);

    QGraphicsScene* scene = new QGraphicsScene(this);
    m_view->setScene(scene);
    m_view->installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //KGameThemeSelector

    
     QGraphicsSvgItem * backgroundsvg = new QGraphicsSvgItem();
     KSvgRenderer * krenderer = new KSvgRenderer();
     KGameTheme kgtheme;
     kgtheme.load("parley/themes/default.desktop");
     krenderer->load(kgtheme.graphics());
     backgroundsvg->setSharedRenderer(krenderer);
     scene->addItem(backgroundsvg);
     m_backgroundRect = backgroundsvg->boundingRect();
     
    scene->setSceneRect(m_backgroundRect);
    m_view->setSceneRect(scene->sceneRect());
    
    //// Loading the Document -- temporary ////
    KEduVocDocument * doc = new KEduVocDocument(this);
    //KUrl url = KFileDialog::getOpenUrl(KUrl::fromPath("~"), KEduVocDocument::pattern(KEduVocDocument::Reading), this, i18n("Open Vocabulary Document"));


    // for the fun of it - use parleyrc
    kDebug() << "open file from parleyrc";
    KConfig parleyConfig("parleyrc");
    kDebug() << parleyConfig.groupList();
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
    // take the last file, but there are File1..n and Name1..n entries..
    QString sourceFile = recentFilesGroup.readEntry(recentFilesGroup.keyList().value(recentFilesGroup.keyList().count() / 2 - 1), QString());

    int code = doc->open(sourceFile); 
    kDebug() << code;

    // this is the only object/widget the window directly keeps track of (outside of the canvas, etc).
    m_manager = new TestEntryManager(this);
    m_manager->open(doc);


    ////// Graphical Widgets Setup //////

    TextualPrompt * prompt = new TextualPrompt();
    prompt->setMinimumSize(100, 30);
    QGraphicsProxyWidget * gprompt = scene->addWidget(prompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), prompt, SLOT(slotSetText(const QString&)));


    TextualInput * input = new TextualInput();
    input->setMinimumSize(100, 30);
    QGraphicsProxyWidget * ginput = scene->addWidget(input);

    Statistics * stats = new Statistics(this);
    SvgBarStatistics * barstats = new SvgBarStatistics(backgroundsvg->renderer());
    scene->addItem(barstats);
    connect(stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));
    connect(m_manager, SIGNAL(signalExpressionChanged(KEduVocExpression*)), stats, SLOT(slotSetExpression(KEduVocExpression*)));

    QGraphicsLinearLayout * promptAndInput = new QGraphicsLinearLayout();
    promptAndInput->setOrientation(Qt::Vertical);
    promptAndInput->addItem(gprompt);
    promptAndInput->addItem(ginput);

    QGraphicsWidget * gpromptAndInput = new QGraphicsWidget();
    gpromptAndInput->setLayout(promptAndInput);
    scene->addItem(gpromptAndInput);

    StdButton * stdbutton = new StdButton("Check Answer");
    QGraphicsProxyWidget * gstdbutton = scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    gstdbutton->setVisible(false); // disable for now
    

    //// Input and Validation Setup ////
    AnswerValidator * validator = new AnswerValidator(doc, this);
    validator->setLanguage(1); // TODO do this for real...
    connect(input, SIGNAL(signalInput(const QString&)), this, SLOT(slotGetInput(const QString&)));
    connect(this, SIGNAL(signalCheckInput(const QString&, const QString&)), validator, SLOT(checkUserAnswer(const QString&, const QString&)));
    connect(validator, SIGNAL(signalCorrection(float, Statistics::ErrorType)), stats, SLOT(slotCorrection(float, Statistics::ErrorType)));
    connect(validator, SIGNAL(signalCorrection(float, Statistics::ErrorType)), input, SLOT(slotChangeAnswerColor(float)));

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

    KAction *prefsAct = KStandardAction::preferences(this, SLOT(slotCreatePreferencesDialog()),
                                       actionCollection());

    //// Final Graphics Setup ////

    gpromptAndInput->setPos(m_backgroundRect.width() / 2.0, m_backgroundRect.height() / 2.0);
    //gstdbutton->setPos(10, 200); // we'll ignore the button for now.

    //backgroundsvg->setVisible(false);

    setupGUI();

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
    KConfigDialog *dialog = new KConfigDialog(this, "settings", PracticePrefs::self());
    dialog->addPage(new KGameThemeSelector(dialog, PracticePrefs::self()), i18n("Theme"), "game_theme");
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

bool ParleyPracticeMainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Resize)
    {
        m_view->fitInView(m_backgroundRect, Qt::KeepAspectRatio);
        return QObject::eventFilter(obj, event);
    }
    return QObject::eventFilter(obj, event);
}


