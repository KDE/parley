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

#include "multiple/multipletextualinput.h"
#include "multiple/multipletextualprompt.h"

#include "statistics/statistics.h"
#include "statistics/svgbarstatistics.h"
#include "stdbuttons.h"
#include "practiceentrymanager.h"
#include "answervalidator.h"
#include "prefs.h"
#include "timer.h"
#include "feedback.h"
#include "activearea.h"
#include "messagebox.h"

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>

#include <QPointer>

void ParleyPracticeMainWindow::setupWrittenTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, area, "question_text_box");
    scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    TextualInput * input = new TextualInput(m_renderer, area, "answer_text_box");
    scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(this, SIGNAL(signalShowHint(const QString&)), input, SLOT(slotShowHint(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
     connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, area, "continue_button");
    scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));


}

void ParleyPracticeMainWindow::setupFlashCardTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, area, "flashcard_text_box");
    scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), tprompt, SLOT(slotSetText(const QString&)));

    // flip the card on click
    //connect(this, SIGNAL(clicked()), actionCollection()->action("show solution"), SIGNAL(triggered()));

    PracticeActionButton * knownButton = new PracticeActionButton(i18n("I Know It"), m_renderer, area, "known_button");
    scene->addWidget(knownButton);
    connect(knownButton, SIGNAL(clicked()), this, SLOT(slotForceCorrect()));

    PracticeActionButton * unknownButton = new PracticeActionButton(i18n("I Don't Know It"), m_renderer, area, "unknown_button");
    connect(unknownButton, SIGNAL(clicked()), this, SLOT(slotForceIncorrect()));
    scene->addWidget(unknownButton);

    PracticeActionButton * showSolutionButton = new PracticeActionButton(i18n("Check"), m_renderer, area, "show_solution_button");
    connect(showSolutionButton, SIGNAL(clicked()), this, SLOT(slotShowSolution()));
    scene->addWidget(showSolutionButton);
}


void ParleyPracticeMainWindow::setupMultipleChoiceTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, area, "question_text_box");
    scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    MCInput * input = new MCInput(m_renderer, area, "answer_mc_box");
    scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(m_manager, SIGNAL(signalNewChoices(const QStringList&)), input, SLOT(slotSetChoices(const QStringList&)));
    connect(this, SIGNAL(signalShowHint(const QString&)), input, SLOT(slotShowHint(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));


    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, area, "continue_button");
    scene->addWidget(stdbutton);
    connect(input, SIGNAL(triggered()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
    stdbutton->setVisible(true); // enable for now

    // if practice is stopped and started again, this will revert to 0.
    static QPointer<QSignalMapper> mapper = 0;

    // we once want to do this once
    if (!mapper)
    {
        // setup shortcuts for multiple choice input
        mapper = new QSignalMapper(this);
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
    }

    connect(mapper, SIGNAL(mapped(int)), input, SLOT(slotShortcutTriggered(int)));
}


void ParleyPracticeMainWindow::setupMixedLettersTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();

    MixedLettersPrompt * mixed = new MixedLettersPrompt(m_renderer, area, "question_mixed_letters_box");
    connect(m_manager, SIGNAL(signalNewSolution(const QString&)), mixed, SLOT(slotSetText(const QString&)));

    TextualPrompt * tprompt = new TextualPrompt(m_renderer, area, "question_text_box");
    scene->addItem(tprompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), tprompt, SLOT(slotSetText(const QString&)));

    TextualInput * input = new TextualInput(m_renderer, area, "answer_text_box");
    scene->addWidget(input);
    connect(input, SIGNAL(signalAnswer(const QString&)), this, SLOT(slotCheckAnswer(const QString&)));
    connect(input, SIGNAL(signalAnswerChanged(const QString&)), mixed, SLOT(slotAnswerChanged(const QString&)));
    connect(this, SIGNAL(signalShowSolution(const QString&, int)), input, SLOT(slotShowSolution(const QString&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), input, SLOT(slotEmitAnswer()));
    connect(actionCollection()->action("continue"), SIGNAL(triggered()), input, SLOT(slotClear()));
    connect(this, SIGNAL(signalShowHint(const QString&)), input, SLOT(slotShowHint(const QString&)));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, area, "continue_button");
    scene->addWidget(stdbutton);
    connect(input, SIGNAL(returnPressed()), stdbutton, SLOT(slotActivated()));
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
}

void ParleyPracticeMainWindow::setupComparisonTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();


    MessageBox* message1 = new MessageBox(m_renderer, area, "absolute_instruction_box", i18n("Absolute:"));
    scene->addItem(message1);

    MessageBox* message2 = new MessageBox(m_renderer, area, "comparative_instruction_box", i18n("Comparative:"));
    scene->addItem(message2);

    MessageBox* message3 = new MessageBox(m_renderer, area, "superlative_instruction_box", i18n("Superlative:"));
    scene->addItem(message3);

    QStringList qsl;
    qsl << "absolute_text_box" << "comparative_text_box" << "superlative_text_box";
    // it adds itself to the scene
    MultipleTextualInput * minput = new MultipleTextualInput(m_renderer, area, qsl);
    connect(m_manager, SIGNAL(signalNewChoices(const QStringList&)), minput, SLOT(slotSetChoices(const QStringList&)));
    connect(minput, SIGNAL(signalAnswer(const QStringList&)), this, SLOT(slotCheckAnswer(const QStringList&)));
    connect(this, SIGNAL(signalShowSolution(const QStringList&, int)), minput, SLOT(slotShowSolution(const QStringList&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), minput, SLOT(slotEmitAnswer()));
    //connect(actionCollection()->action("continue"), SIGNAL(triggered()), minput, SLOT(slotClear()));
    connect(this, SIGNAL(signalShowHint(const QString&)), minput, SLOT(slotShowHint()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, area, "continue_button");
    scene->addWidget(stdbutton);
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
}


void ParleyPracticeMainWindow::setupConjugationTemplate(ActiveArea * area)
{
    QGraphicsScene * scene = area->scene();

    QStringList questions, answers;
    questions << "question_text_box_infinitive"  << "question_text_box_1" << "question_text_box_2" <<  "question_text_box_3";
    answers << "answer_text_box_infinitive"  << "answer_text_box_1" << "answer_text_box_2" <<  "answer_text_box_3";

    MultipleTextualPrompt * mprompt = new MultipleTextualPrompt(m_renderer, area, questions);
    connect(m_manager, SIGNAL(signalNewText(const QStringList&)), mprompt, SLOT(slotSetText(const QStringList&)));

    // it adds itself to the scene
    MultipleTextualInput * minput = new MultipleTextualInput(m_renderer, area, answers);
    connect(minput, SIGNAL(signalAnswer(const QStringList&)), this, SLOT(slotCheckAnswer(const QStringList&)));
    connect(m_manager, SIGNAL(signalNewChoices(const QStringList&)), minput, SLOT(slotSetChoices(const QStringList&)));
    connect(this, SIGNAL(signalShowSolution(const QStringList&, int)), minput, SLOT(slotShowSolution(const QStringList&)));
    connect(actionCollection()->action("check answer"), SIGNAL(triggered()), minput, SLOT(slotEmitAnswer()));
    connect(this, SIGNAL(signalShowHint(const QString&)), minput, SLOT(slotShowHint()));

    StdButton * stdbutton = new StdButton(i18n("Check Answer"), m_renderer, area, "continue_button");
    scene->addWidget(stdbutton);
    connect(this, SIGNAL(signalCheckAnswerContinueActionsToggled(int)), stdbutton, SLOT(slotToggleText(int)));
    connect(stdbutton, SIGNAL(signalCheckAnswer()), actionCollection()->action("check answer"), SIGNAL(triggered()));
    connect(stdbutton, SIGNAL(signalContinue()), actionCollection()->action("continue"), SIGNAL(triggered()));
}
