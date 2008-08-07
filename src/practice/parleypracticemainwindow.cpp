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
#include <KAction>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KApplication>
#include <QShortcut>
#include <QSignalMapper>

#include "practiceview.h"

#include "input/textualinput.h"
#include "input/mcinput.h"

#include "statistics/statistics.h"
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
    setupActiveArea();
    setupActions();
    setupModeIndependent();
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
    setupActiveArea();
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

void ParleyPracticeMainWindow::slotCheckAnswer(const QStringList& input)
{
    m_validator->slotCheckAnswer(m_manager->currentSolutions(), input);
}

void ParleyPracticeMainWindow::slotShowSolution()
{
    emit signalShowSolution(m_manager->currentSolution(), m_state);
    if (m_mode != Prefs::EnumTestType::FlashCardsTest) // tainting and such doesn't exist in flashcard mode.
    {
        if (m_state == CheckAnswer) // only switch if they haven't already answered
        {
            emit signalCheckAnswerContinueActionsToggled(m_state);
            m_stats->slotSolutionShown();
        }
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

// here is where we'll add new modes
void ParleyPracticeMainWindow::setupModeSpecifics()
{
    switch (m_mode)
    {
       case Prefs::EnumTestType::ExampleTest:
       case Prefs::EnumTestType::ParaphraseTest:
       case Prefs::EnumTestType::WrittenTest:
        setupWrittenTemplate();
        break;

       case Prefs::EnumTestType::SynonymTest:
       case Prefs::EnumTestType::AntonymTest:
       case Prefs::EnumTestType::ArticleTest:
       case Prefs::EnumTestType::MultipleChoiceTest:
        setupMultipleChoiceTemplate();
        break;

       case Prefs::EnumTestType::MixedLettersTest:
        setupMixedLettersTemplate();
        break;

       case Prefs::EnumTestType::FlashCardsTest:
        setupFlashCardTemplate();
        break;

       case Prefs::EnumTestType::ComparisonTest:
        setupComparisonTemplate();
        break;

       default:
        kDebug() << "unhandled practice mode " << m_mode << " selected.";
        break;
    }
}

void ParleyPracticeMainWindow::setupActiveArea()
{
    switch (m_mode)
    {
       case Prefs::EnumTestType::ParaphraseTest:
            m_area = new ActiveArea(m_renderer, "paraphrase", "written");
            break;
       case Prefs::EnumTestType::ExampleTest:
            m_area = new ActiveArea(m_renderer, "example", "written");
            break;
       case Prefs::EnumTestType::WrittenTest:
            m_area = new ActiveArea(m_renderer, "written");
            break;
       case Prefs::EnumTestType::MultipleChoiceTest:
            m_area = new ActiveArea(m_renderer, "multiple_choice");
            break;
       case Prefs::EnumTestType::ArticleTest:
            m_area = new ActiveArea(m_renderer, "article", "multiple_choice");
            break;
       case Prefs::EnumTestType::MixedLettersTest:
            m_area = new ActiveArea(m_renderer, "mixed_letters");
            break;
       case Prefs::EnumTestType::FlashCardsTest:
            m_area = new ActiveArea(m_renderer, "flashcard");
            break;
       case Prefs::EnumTestType::ComparisonTest:
            m_area = new ActiveArea(m_renderer, "comparison");
            break;
       case Prefs::EnumTestType::SynonymTest:
            m_area = new ActiveArea(m_renderer, "synonym", "multiple_choice");
            break;
       case Prefs::EnumTestType::AntonymTest:
            m_area = new ActiveArea(m_renderer, "antonym", "multiple_choice");
            break;
       default:
        kDebug() << "unhandled practice mode " << m_mode << " selected.";
        break;
    }
    m_scene->addItem(m_area);
}


bool ParleyPracticeMainWindow::queryClose()
{
    emit signalPracticeFinished();
    return true;
}
