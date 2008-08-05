//
// C++ Implementation: modeindependent
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
/***************************************************************************
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
#include <KAction>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KApplication>
#include <QShortcut>
#include <QSignalMapper>

#include "practiceview.h"

#include "statistics/statistics.h"
#include "statistics/svgbarstatistics.h"

#include "hint.h"
#include "practiceentrymanager.h"
#include "answervalidator.h"
#include "prefs.h"
#include "timer.h"
#include "feedback.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>

void ParleyPracticeMainWindow::setupBase(const QString& desktopFileName, KEduVocDocument * doc)
{
    m_state = CheckAnswer;
    m_mode = Prefs::testType();

    m_view = new PracticeView;
    setCentralWidget(m_view);

    m_scene = new QGraphicsScene(this);
    m_view->setScene(m_scene);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     m_layout = new QGraphicsSvgItem();
     m_renderer = new KSvgRenderer();
     KGameTheme kgtheme;
     // TODO use the kgametheme theme
     kDebug() << "kgametheme valid:" << kgtheme.load("parley/themes/" + desktopFileName);
     kDebug() << "graphics svg path:" << kgtheme.graphics();
     m_renderer->load(kgtheme.graphics());
     m_layout->setSharedRenderer(m_renderer);
     m_layout->setElementId("main");
     m_scene->addItem(m_layout);
     m_layout->setZValue(-10);

    m_scene->setSceneRect(m_layout->boundingRect());
    m_view->setSceneRect(m_scene->sceneRect());

    m_manager = new PracticeEntryManager(this);

    m_stats = new Statistics(m_manager, this);
    connect(m_manager, SIGNAL(signalEntryChanged(PracticeEntry*, QList<PracticeEntry*>)), m_stats, SLOT(slotSetEntry(PracticeEntry*)));
    connect(m_manager, SIGNAL(signalSetFinished()), m_stats, SLOT(slotSetFinished()));
    connect(m_manager, SIGNAL(signalSetFinished()), this, SIGNAL(signalPracticeFinished()));

    KEduVocDocument * local_doc;
    if (doc != 0)
    {
        local_doc = doc;
    }
    else // m_doc will be non-null if we used the doc-giving constructor
    {
        local_doc = new KEduVocDocument(this);
        // for the fun of it - use parleyrc
        kDebug() << "open file from parleyrc";
        KConfig parleyConfig("parleyrc");
        kDebug() << "groupList" << parleyConfig.groupList();
        KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
        // take the last file, but there are File1..n and Name1..n entries..
        QString sourceFile = recentFilesGroup.readEntry(recentFilesGroup.keyList().value(recentFilesGroup.keyList().count() / 2 - 1), QString());

        int code = local_doc->open(sourceFile);
        kDebug() << "file open code:" << code;
    }

    m_manager->open(local_doc);

    m_validator = new AnswerValidator(doc, this);
    m_validator->setLanguage(Prefs::solutionLanguage());
    connect(m_validator, SIGNAL(signalCorrection(float, Statistics::ErrorType, const QString&)), m_stats, SLOT(slotCorrection(float, Statistics::ErrorType, const QString&)));

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

    KStandardAction::quit(this, SLOT(queryClose()), actionCollection());
}

void ParleyPracticeMainWindow::setupModeIndependent()
{
    Feedback * feedback = new Feedback(m_renderer, m_area, "feedback_box");
    m_scene->addItem(feedback);
    connect(m_validator, SIGNAL(signalFeedback(const QString&)), feedback, SLOT(slotSetText(const QString&)));
    connect(m_manager, SIGNAL(signalNewEntry()), feedback, SLOT(slotClear()));

    SvgBarStatistics * barstats = new SvgBarStatistics(m_renderer, m_area, "progress_bar", "progress_bar_background");
    m_scene->addItem(barstats);
    connect(m_stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));

    if (Prefs::practiceTimeout() && Prefs::practiceTimeoutTimePerAnswer()) // timeout can't be 0
    {
        m_timer = new Timer(this);
        m_timer->makeGUI(m_renderer, m_area);
        m_timer->setLength(Prefs::practiceTimeoutTimePerAnswer()*1000); // seconds -> milliseconds

        // when the timer triggers, it will assume their current input is their answer
        connect(m_timer, SIGNAL(signalTimeout()), actionCollection()->action("check answer"), SIGNAL(triggered()));
        connect(m_manager, SIGNAL(signalNewEntry()), m_timer, SLOT(slotStart()));
        connect(actionCollection()->action("check answer"), SIGNAL(triggered()), m_timer, SLOT(slotStop()));
    }
    else
    {
        m_timer = new Timer(false);
    }

}
