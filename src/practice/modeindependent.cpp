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
#include <KMessageBox>

#include "practiceview.h"

#include "statistics/statistics.h"
#include "statistics/svgbarstatistics.h"

#include "practiceentrymanager.h"
#include "answervalidator.h"
#include "prefs.h"
#include "timer.h"
#include "feedback.h"
#include "stdbuttons.h"
#include "messagebox.h"

#include "prompt/imageprompt.h"
#include "prompt/soundprompt.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>

void ParleyPracticeMainWindow::setupBase(const QString& desktopFileName, KEduVocDocument * doc)
{
    m_state = CheckAnswer;

    m_renderer = new KSvgRenderer();
    KGameTheme kgtheme;
    bool valid = kgtheme.load("themes/" + Prefs::theme() +
    ".desktop");
    kDebug() << "kgametheme valid:" << valid;
    // if we're screwed, abort to avoid crashing messily.
    if (!valid)
    {
      KMessageBox::information(this, i18n("The selected theme is invalid and cannot be loaded."));
      slotClose();
      return;
    }
    kDebug() << "graphics svg path:" << kgtheme.graphics();
    m_renderer->load(kgtheme.graphics());

    setupActiveArea();

    QGraphicsSvgItem * layout = new QGraphicsSvgItem();
    layout->setSharedRenderer(m_renderer);
    layout->setElementId("main");
    layout->setZValue(-10);


    QGraphicsSvgItem * ilayout = new QGraphicsSvgItem();
    ilayout->setSharedRenderer(m_renderer);
    ilayout->setElementId("main");
    ilayout->setZValue(-10);



    QGraphicsScene* normalScene = new QGraphicsScene(this);
    normalScene->addItem(layout);
    normalScene->addItem(m_normalArea);
    normalScene->setSceneRect(layout->boundingRect());

    QGraphicsScene* imageScene = new QGraphicsScene(this);
    imageScene->addItem(ilayout);
    imageScene->addItem(m_imageArea);
    imageScene->setSceneRect(ilayout->boundingRect());


    m_view = new PracticeView(this);
    setCentralWidget(m_view);

    m_manager = new PracticeEntryManager(this);
    connect(m_manager, SIGNAL(signalNewImage(const KUrl&, bool)), this, SLOT(slotShowImageView(const KUrl&, bool)));


    m_stats = new Statistics(m_manager, this);
    connect(m_manager, SIGNAL(signalNewEntry(PracticeEntry*)), m_stats, SLOT(slotSetEntry(PracticeEntry*)));
    connect(m_manager, SIGNAL(signalSetFinished()), m_stats, SLOT(slotSetFinished()));
    connect(m_manager, SIGNAL(signalSetFinished()), this, SLOT(queryClose()));

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
    connect(hintAction, SIGNAL(triggered()), this, SLOT(slotShowHint()));
    if (!Prefs::showHints())
        hintAction->setVisible(false);


    KStandardAction::preferences(this, SLOT(slotCreatePreferencesDialog()),
                                       actionCollection());

    KStandardAction::quit(this, SLOT(queryClose()), actionCollection());
}

void ParleyPracticeMainWindow::setupModeIndependent(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();
    QString instruction;

    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::SynonymTest:
            instruction = i18n("Select Synonym:");
            break;
        case Prefs::EnumTestType::AntonymTest:
            instruction = i18n("Select Antonym:");
            break;
        case Prefs::EnumTestType::ArticleTest:
            instruction = i18n("Select the Correct Article:");
            break;
        case Prefs::EnumTestType::MultipleChoiceTest:
            instruction = i18n("Select Translation:");
            break;
        case Prefs::EnumTestType::ExampleTest:
            instruction = i18n("Enter Missing Word:");
            break;
        case Prefs::EnumTestType::ParaphraseTest:
            instruction = i18n("Enter An Matching Word:");
            break;
        case Prefs::EnumTestType::WrittenTest:
            instruction = i18n("Enter Translation:");
            break;
        case Prefs::EnumTestType::FlashCardsTest:
            instruction = i18n("What is the solution?");
            break;
        case Prefs::EnumTestType::MixedLettersTest:
            instruction = i18n("Unscramble:");
            break;
        case Prefs::EnumTestType::ConjugationTest:
            instruction = i18n("Conjugate:");
            break;
        case Prefs::EnumTestType::ComparisonTest:
            instruction = i18n("Enter Comparison Forms:");
            break;
    }

    MessageBox* messagebox = new MessageBox(m_renderer, area, "instruction_box", instruction);
    scene->addItem(messagebox);

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, area, "image_box");
        scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&, bool)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, area, "sound_box");
        scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    SvgBarStatistics * barstats = new SvgBarStatistics(m_renderer, area, "progress_bar", "progress_bar_background");
    scene->addItem(barstats);
    connect(m_stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));

    // these don't apply to flashcard mode
    if (Prefs::testType() != Prefs::EnumTestType::FlashCardsTest)
    {
        Feedback * feedback = new Feedback(m_renderer, area, "feedback_box");
        scene->addItem(feedback);
        connect(m_validator, SIGNAL(signalFeedback(const QString&)), feedback, SLOT(slotSetText(const QString&)));
        connect(m_manager, SIGNAL(signalNewEntry(PracticeEntry*)), feedback, SLOT(slotClear()));


        PracticeActionButton * knownButton = new PracticeActionButton(i18n("I Know It"), m_renderer, area, "skip_known_button");
        scene->addWidget(knownButton);
        connect(knownButton, SIGNAL(clicked()), actionCollection()->action("skip known"), SIGNAL(triggered()));

        PracticeActionButton * unknownButton = new PracticeActionButton(i18n("I Don't Know It"), m_renderer, area, "skip_unknown_button");
        connect(unknownButton, SIGNAL(clicked()), actionCollection()->action("skip unknown"), SIGNAL(triggered()));
        scene->addWidget(unknownButton);


        PracticeActionButton * showSolutionButton = new PracticeActionButton(i18n("Show Solution"), m_renderer, area, "show_solution_button");
        connect(showSolutionButton, SIGNAL(clicked()), actionCollection()->action("show solution"), SIGNAL(triggered()));
        scene->addWidget(showSolutionButton);
    }
    if (Prefs::practiceTimeout() && Prefs::practiceTimeoutTimePerAnswer()) // timeout can't be 0
    {
        m_timer = new Timer(this);
        m_timer->makeGUI(m_renderer, area);
        m_timer->setLength(Prefs::practiceTimeoutTimePerAnswer()*1000); // seconds -> milliseconds

        // when the timer triggers, it will assume their current input is their answer
        connect(m_timer, SIGNAL(signalTimeout()), actionCollection()->action("check answer"), SIGNAL(triggered()));
        connect(m_manager, SIGNAL(signalNewEntry(PracticeEntry*)), m_timer, SLOT(slotStart()));
        connect(actionCollection()->action("check answer"), SIGNAL(triggered()), m_timer, SLOT(slotStop()));
    }
    else
    {
        m_timer = new Timer(false);
    }

}
