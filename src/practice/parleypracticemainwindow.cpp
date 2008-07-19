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
#include <QShortcut>
#include <QSignalMapper>

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
#include "feedback.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>


ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{
    setupBase("default.desktop");
    setupActions();
    setupModeSpecifics();

    setupGUI(Default, QString::fromLatin1("parleypracticeui.rc"));

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();

    m_view->show();
}

ParleyPracticeMainWindow::ParleyPracticeMainWindow(KEduVocDocument* doc, QWidget *parent)
        : KXmlGuiWindow(parent)
{
    setupBase("default.desktop", doc);
    setupActions();
    setupModeIndependent();
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
    emit signalShowSolution(m_manager->currentSolution(), m_state);
    if (m_state == CheckAnswer) // only switch if they haven't already answered
    {
        emit signalCheckAnswerContinueActionsToggled(m_state);
        m_stats->slotSolutionShown();
    }
}

void ParleyPracticeMainWindow::slotCreatePreferencesDialog()
{
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Prefs::self());
    dialog->addPage(new KGameThemeSelector(dialog, Prefs::self()), i18n("Theme"), "game_theme");
    dialog->show();
}

// this one is a mouthful...
void ParleyPracticeMainWindow::slotToggleCheckAnswerContinueActions()
{
    if (m_state == Continue)
    {
        actionCollection()->action("continue")->setVisible(false);
        actionCollection()->action("check answer")->setVisible(true);
        m_state = CheckAnswer;
    }
    else
    {
        actionCollection()->action("continue")->setVisible(true);
        actionCollection()->action("check answer")->setVisible(false);
        m_state = Continue;
    }

    emit signalCheckAnswerContinueActionsToggled(m_state);
}

void ParleyPracticeMainWindow::slotForceCorrect()
{
    m_stats->slotCorrection(1.0, Statistics::Correct, QString());
    m_manager->slotNewEntry();
}

void ParleyPracticeMainWindow::slotForceIncorrect()
{
    m_stats->slotCorrection(0.0f, Statistics::ForcedIncorrect, QString());
    m_manager->slotNewEntry();
}

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

     QGraphicsSvgItem * backgroundsvg = new QGraphicsSvgItem();
     m_renderer = new KSvgRenderer();
     KGameTheme kgtheme;
     kDebug() << "kgametheme valid:" << kgtheme.load("parley/themes/" + desktopFileName);
     kDebug() << "graphics svg path" << kgtheme.graphics();
     m_renderer->load(kgtheme.graphics());
     backgroundsvg->setSharedRenderer(m_renderer);
     m_scene->addItem(backgroundsvg);

    m_scene->setSceneRect(backgroundsvg->boundingRect());
    m_view->setSceneRect(m_scene->sceneRect());

    m_manager = new PracticeEntryManager(this);

    m_stats = new Statistics(m_manager, this);
    connect(m_manager, SIGNAL(signalEntryChanged(PracticeEntry*, QList<PracticeEntry*>)), m_stats, SLOT(slotSetEntry(PracticeEntry*)));
    connect(m_manager, SIGNAL(signalSetFinished()), m_stats, SLOT(slotSetFinished()));
    connect(m_manager, SIGNAL(signalSetFinished()), this, SIGNAL(queryClose()));

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
    Feedback * feedback = new Feedback(m_renderer, "feedback_box");
    m_scene->addItem(feedback);
    connect(m_validator, SIGNAL(signalFeedback(const QString&)), feedback, SLOT(slotSetText(const QString&)));
    connect(m_manager, SIGNAL(signalNewEntry()), feedback, SLOT(slotClear()));

    SvgBarStatistics * barstats = new SvgBarStatistics(m_renderer, "bar", "bar_background");
    m_scene->addItem(barstats);
    connect(m_stats, SIGNAL(signalUpdateDisplay(Statistics*)), barstats, SLOT(slotUpdateDisplay(Statistics*)));

    if (Prefs::practiceTimeout() && Prefs::practiceTimeoutTimePerAnswer()) // timeout can't be 0
    {
        m_timer = new Timer(this);
        m_timer->makeGUI(m_renderer, m_scene);
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

// here is where we'll add new modes
void ParleyPracticeMainWindow::setupModeSpecifics()
{
    switch (m_mode)
    {
       case Prefs::EnumTestType::WrittenTest:
        setupWritten();
        break;
       case Prefs::EnumTestType::MultipleChoiceTest:
        setupMultipleChoice();
        break;
       case Prefs::EnumTestType::ArticleTest:
        setupArticle();
        break;
       case Prefs::EnumTestType::MixedLettersTest:
        setupMixedLetters();
        break;
       case Prefs::EnumTestType::FlashCardsTest:
        setupFlashCards();
        break;
       case Prefs::EnumTestType::ParaphraseTest:
        setupParaphrase();
        break;
       default:
        kDebug() << "unhandled practice mode " << m_mode << " selected.";
        break;
    }
}


bool ParleyPracticeMainWindow::queryClose()
{
    emit signalPracticeFinished();
    return true;
}

void ParleyPracticeMainWindow::setupWritten()
{

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_view, "image_box");
        m_scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_view, "sound_box");
        m_scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    TextualInput * input = new TextualInput(m_renderer, m_view, "practice_text_translation_background");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_view, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
    stdbutton->setVisible(true); // enable for now


    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));
}


void ParleyPracticeMainWindow::setupFlashCards()
{

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "flashcard_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    // flip the card on click
    connect(this, SIGNAL(clicked()), actionCollection()->action("show solution"), SIGNAL(triggered()));

    PracticeActionButton * knownButton = new PracticeActionButton("Known", m_renderer, "known_button");
    connect(knownButton, SIGNAL(clicked()), this, SLOT(slotForceCorrect()));
    PracticeActionButton * unknownButton = new PracticeActionButton("Not Known", m_renderer, "unknown_button");
    connect(unknownButton, SIGNAL(clicked()), this, SLOT(slotForceIncorrect()));

}


void ParleyPracticeMainWindow::setupMultipleChoice()
{

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_view, "image_box");
        m_scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_view, "sound_box");
        m_scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    MultipleChoiceMCInput * input = new MultipleChoiceMCInput(m_renderer, m_view, "practice_text_translation_background");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(m_manager, SIGNAL(signalEntryChanged(PracticeEntry*, QList<PracticeEntry*>)), input, SLOT(slotSetAnswers(PracticeEntry*, QList<PracticeEntry*>)));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_view, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(triggered()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
    stdbutton->setVisible(true); // enable for now


    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));

    // setup shortcuts for multiple choice input
    QSignalMapper * mapper = new QSignalMapper(this);
    KAction * shortcut;
     for(int n = 1; n < 10; ++n)
     {
        shortcut = new KAction(this);
        shortcut->setText(i18n("Select Option %1", n));
        actionCollection()->addAction(QString("select option %1").arg(n), shortcut);
        shortcut->setShortcut(KShortcut(QString("%1; Alt+%1").arg(n)));
        mapper->setMapping(shortcut, n);
        connect(shortcut, SIGNAL(triggered()), mapper, SLOT(map()));
        if (n > Prefs::numberMultipleChoiceAnswers())
            shortcut->setVisible(false); // disable non-relevent shortcuts
     }


    // enter/return triggers shortcut 0, which means use the currently selected option
    // if no option is selected, this is ignored.
    QShortcut* accelerator = new QShortcut(Qt::Key_Enter, this);
    accelerator->setAutoRepeat(false);
    mapper->setMapping(accelerator, 0);
    connect(accelerator, SIGNAL(activated()), mapper, SLOT(map()));

    accelerator = new QShortcut(Qt::Key_Return, this);
    accelerator->setAutoRepeat(false);
    mapper->setMapping(accelerator, 0);
    connect(accelerator, SIGNAL(activated()), mapper, SLOT(map()));

    connect(mapper, SIGNAL(mapped(int)), input, SLOT(slotShortcutTriggered(int)));
}


void ParleyPracticeMainWindow::setupArticle()
{

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_view, "image_box");
        m_scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_view, "image_box");
        m_scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    ArticleMCInput * input = new ArticleMCInput(m_renderer, m_view, "practice_text_translation_background", m_manager->document());
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(m_manager, SIGNAL(signalEntryChanged(PracticeEntry*, QList<PracticeEntry*>)), input, SLOT(slotSetAnswers(PracticeEntry*)));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_view, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(triggered()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
    stdbutton->setVisible(true); // enable for now


    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));


    // setup shortcuts for multiple choice input
    QSignalMapper * mapper = new QSignalMapper(this);
    KAction * shortcut;
     for(int n = 1; n < 10; ++n)
     {
        shortcut = new KAction(this);
        shortcut->setText(i18n("Select Option %1", n));
        actionCollection()->addAction(QString("select option %1").arg(n), shortcut);
        shortcut->setShortcut(KShortcut(QString("%1; Alt+%1").arg(n)));
        mapper->setMapping(shortcut, n);
        connect(shortcut, SIGNAL(triggered()), mapper, SLOT(map()));
        if (n > Prefs::numberMultipleChoiceAnswers())
            shortcut->setVisible(false); // disable non-relevent shortcuts
     }


    // enter/return triggers shortcut 0, which means use the currently selected option
    // if no option is selected, this is ignored.
    QShortcut* accelerator = new QShortcut(Qt::Key_Enter, this);
    accelerator->setAutoRepeat(false);
    mapper->setMapping(accelerator, 0);
    connect(accelerator, SIGNAL(activated()), mapper, SLOT(map()));

    accelerator = new QShortcut(Qt::Key_Return, this);
    accelerator->setAutoRepeat(false);
    mapper->setMapping(accelerator, 0);
    connect(accelerator, SIGNAL(activated()), mapper, SLOT(map()));

    connect(mapper, SIGNAL(mapped(int)), input, SLOT(slotShortcutTriggered(int)));
}


void ParleyPracticeMainWindow::setupMixedLetters()
{

    MixedLettersPrompt * mixed = new MixedLettersPrompt(m_renderer, m_view, "image_box");
    connect(m_manager, SIGNAL(signalNewSolution(const QString&)), mixed, SLOT(slotSetText(const QString&)));

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    TextualInput * input = new TextualInput(m_renderer, m_view, "practice_text_translation_background");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(input, SIGNAL(signalAnswerChanged(const QString&)), mixed, SLOT(slotAnswerChanged(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));


    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_view, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
    stdbutton->setVisible(true); // enable for now


    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));
}

void ParleyPracticeMainWindow::setupParaphrase()
{

    TextualPrompt * prompt = new TextualPrompt(m_renderer, "practice_text_background");
    m_scene->addItem(prompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), prompt, SLOT(slotSetText(const QString&)));

    TextualInput * input = new TextualInput(m_renderer, m_view, "practice_text_translation_background");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_view, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));

    Hint * hint = new Hint(this);
    connect(actionCollection()->action("hint"), SIGNAL(triggered()), hint, SLOT(slotShowHint()));
    // this is the hint for now :)
    connect(hint, SIGNAL(signalShowHint()), actionCollection()->action("show solution"), SIGNAL(triggered()));
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), m_stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));
}
