//
// C++ Implementation: modespecifics
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "parleypracticemainwindow.h"

#include <KDebug>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>
#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <QShortcut>
#include <QSignalMapper>
#include <KLocalizedString>

#include "practiceview.h"

#include "input/textualinput.h"
#include "input/mcinput.h"

#include "prompt/mixedlettersprompt.h"
#include "prompt/soundprompt.h"
#include "prompt/imageprompt.h"
#include "prompt/textualprompt.h"

#include "multiple/multiplewidgetparent.h"
#include "multiple/mwpcomparison.h"

#include "statistics/statistics.h"
#include "statistics/svgbarstatistics.h"
#include "stdbuttons.h"
#include "hint.h"
#include "practiceentrymanager.h"
#include "answervalidator.h"
#include "prefs.h"
#include "timer.h"
#include "feedback.h"
#include "activearea.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>



void ParleyPracticeMainWindow::setupWrittenTemplate()
{
    TextualPrompt * tprompt = new TextualPrompt(m_renderer, m_area, "question_text_box");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_area, "image_box");
        m_scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_area, "sound_box");
        m_scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    TextualInput * input = new TextualInput(m_renderer, m_area, "answer_text_box");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
     connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_area, "check_answer_and_continue_button");
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

void ParleyPracticeMainWindow::setupFlashCardTemplate()
{
    TextualPrompt * tprompt = new TextualPrompt(m_renderer, m_area, "flashcard_text_box");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), tprompt, SLOT(slotSetText(const QString&)));

    // flip the card on click
    connect(this, SIGNAL(clicked()), actionCollection()->action("show solution"), SIGNAL(triggered()));

    PracticeActionButton * knownButton = new PracticeActionButton(i18n("I Know It"), m_renderer, m_area, "known_button");
    m_scene->addWidget(knownButton);
    connect(knownButton, SIGNAL(clicked()), this, SLOT(slotForceCorrect()));

    PracticeActionButton * unknownButton = new PracticeActionButton(i18n("I Don't Know It"), m_renderer, m_area, "unknown_button");
    connect(unknownButton, SIGNAL(clicked()), this, SLOT(slotForceIncorrect()));
    m_scene->addWidget(unknownButton);

    PracticeActionButton * showSolutionButton = new PracticeActionButton(i18n("Check"), m_renderer, m_area, "show_solution_button");
    connect(showSolutionButton, SIGNAL(clicked()), this, SLOT(slotShowSolution()));
    m_scene->addWidget(showSolutionButton);
}


void ParleyPracticeMainWindow::setupMultipleChoiceTemplate()
{
    TextualPrompt * tprompt = new TextualPrompt(m_renderer, m_area, "question_text_box");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    if (Prefs::practiceImagesEnabled())
    {
        ImagePrompt * iprompt = new ImagePrompt(m_renderer, m_area, "image_box");
        m_scene->addWidget(iprompt);
        connect(m_manager, SIGNAL(signalNewImage(const KUrl&)), iprompt, SLOT(slotSetImage(const KUrl&)));
    }

    if (Prefs::practiceSoundEnabled())
    {
        SoundPrompt * sprompt = new SoundPrompt(m_renderer, m_area, "sound_box");
        m_scene->addWidget(sprompt);
        connect(m_manager, SIGNAL(signalNewSound(const KUrl&)), sprompt, SLOT(slotSetSound(const KUrl&)));
    }

    MCInput * input = new MCInput(m_renderer, m_area, "answer_mc_box");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(m_manager, SIGNAL(signalNewChoices(const QStringList&)), input, SLOT(slotSetChoices(const QStringList&)));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_area, "check_answer_and_continue_button");
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


void ParleyPracticeMainWindow::setupMixedLettersTemplate()
{
    MixedLettersPrompt * mixed = new MixedLettersPrompt(m_renderer, m_area, "question_mixed_letters_box");
    connect(m_manager, SIGNAL(signalNewSolution(const QString&)), mixed, SLOT(slotSetText(const QString&)));

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, m_area, "question_text_box");
    m_scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    TextualInput * input = new TextualInput(m_renderer, m_area, "answer_text_box");
    m_scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(input, SIGNAL(signalAnswerChanged(const QString&)), mixed, SLOT(slotAnswerChanged(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));


    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_area, "check_answer_and_continue_button");
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

void ParleyPracticeMainWindow::setupComparisonTemplate()
{
    QStringList qsl;
    qsl << "absolute" << "comparative" << "superlative";
    // it adds itself to the scene
    MWPComparison * logic = new MWPComparison();
    MultipleWidgetParent * minput = new MultipleWidgetParent(m_renderer, m_area, m_scene, qsl, logic);
    connect(m_manager, SIGNAL(signalNewChoices(const QStringList&)), minput, SLOT(slotSetChoices(const QStringList&)));
    connect(minput, SIGNAL(signalAnswer(const QStringList&)), this, SLOT(slotCheckAnswer(const QStringList&)));
    // connect(this, SIGNAL(signalShowSolution(const QStringList&, int)), minput, SLOT(slotShowSolution(const QStringList&))); // TODO do this
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), minput, SLOT(slotEmitAnswer()));
    //
    //connect(actionCollection()->action("continue"), SIGNAL(triggered()), minput, SLOT(slotClear()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, m_area, "check_answer_and_continue_button");
    m_scene->addWidget(stdbutton);
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
}