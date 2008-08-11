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
#include <QGraphicsSvgItem>
#include <KSvgRenderer>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KApplication>
#include <KActionCollection>
#include <KAction>
#include <KUrl>

#include "kgametheme/kgamethemeselector.h"
#include "kgametheme/kgametheme.h"

#include "practiceview.h"
#include "activearea.h"
#include "practiceentrymanager.h"
#include "answervalidator.h"

ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{
    setupBase("default.desktop");
    setupActiveArea();
    setupActions();
    setupModeIndependent(m_normalView->activeArea());
    if (m_imageView->activeArea()->valid())
        setupModeIndependent(m_imageView->activeArea());
    setupModeSpecifics();

    setupGUI(Default, QString::fromLatin1("parleypracticeui.rc"));

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();
}

ParleyPracticeMainWindow::ParleyPracticeMainWindow(KEduVocDocument* doc, QWidget *parent)
        : KXmlGuiWindow(parent)
{
    setupBase("default.desktop", doc);
    setupActiveArea();
    setupActions();
    setupModeIndependent(m_normalView->activeArea());
    if (m_imageView->activeArea()->valid())
        setupModeIndependent(m_imageView->activeArea());
    setupModeSpecifics();

    setupGUI(Default, QString::fromLatin1("parleypracticeui.rc"));

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();

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
    else
    {
        m_manager->slotEmitImage(true);
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
        setupWrittenTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupWrittenTemplate(m_imageView->activeArea());
        break;

       case Prefs::EnumTestType::SynonymTest:
       case Prefs::EnumTestType::AntonymTest:
       case Prefs::EnumTestType::ArticleTest:
       case Prefs::EnumTestType::MultipleChoiceTest:
        setupMultipleChoiceTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupMultipleChoiceTemplate(m_imageView->activeArea());
        break;

       case Prefs::EnumTestType::MixedLettersTest:
        setupMixedLettersTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupMixedLettersTemplate(m_imageView->activeArea());
        break;

       case Prefs::EnumTestType::FlashCardsTest:
        setupFlashCardTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupFlashCardTemplate(m_imageView->activeArea());
        break;

       case Prefs::EnumTestType::ComparisonTest:
        setupComparisonTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupComparisonTemplate(m_imageView->activeArea());
        break;
       case Prefs::EnumTestType::ConjugationTest:
        setupConjugationTemplate(m_normalView->activeArea());
        if (m_imageView->activeArea()->valid())
            setupConjugationTemplate(m_imageView->activeArea());
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
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "paraphrase", "written"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "paraphrase_image", "written_image"));
            break;
       case Prefs::EnumTestType::ExampleTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "example", "written"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "example_image", "written_image"));
            break;
       case Prefs::EnumTestType::WrittenTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "written"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "written_image"));
            break;
       case Prefs::EnumTestType::MultipleChoiceTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "multiple_choice"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "multiple_choice_image"));
            break;
       case Prefs::EnumTestType::ArticleTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "article", "multiple_choice"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "article_image", "multiple_choice_image"));
            break;
       case Prefs::EnumTestType::MixedLettersTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "mixed_letters"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "mixed_letters_image"));
            break;
       case Prefs::EnumTestType::FlashCardsTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "flashcard"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "flashcard_image"));
            break;
       case Prefs::EnumTestType::ComparisonTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "comparison"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "comparison_image"));
            break;
       case Prefs::EnumTestType::ConjugationTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "conjugation"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "conjugation_image"));
            break;
       case Prefs::EnumTestType::SynonymTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "synonym", "multiple_choice"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "synonym_image", "multiple_choice_image"));
            break;
       case Prefs::EnumTestType::AntonymTest:
            m_normalView->setActiveArea(new ActiveArea(m_renderer, "antonym", "multiple_choice"));
            m_imageView->setActiveArea(new ActiveArea(m_renderer, "antonym_image", "multiple_choice_image"));
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

void ParleyPracticeMainWindow::slotShowImageView(const KUrl& url, bool backsideOfCard)
{
    bool show = url.url().isEmpty();

    bool fc = false;
    if (m_mode == Prefs::EnumTestType::FlashCardsTest)
    {
       if (!backsideOfCard && Prefs::flashcardsFrontImage())
       {
        fc = true;
       }
       else if (backsideOfCard && Prefs::flashcardsBackImage())
       {
        fc = true;
       }
       else
       {
        fc = false;
       }
    }
    if (!show && m_imageView->activeArea()->valid() && fc)
    {
        m_normalView->activeArea()->setActive(false);
        m_imageView->activeArea()->setActive(true);
        m_normalView->hide();
        setCentralWidget(m_imageView);
        m_imageView->show();
    }
    else
    {
        m_normalView->activeArea()->setActive(true);
        m_imageView->activeArea()->setActive(false);
        m_imageView->hide();
        setCentralWidget(m_normalView);
        m_normalView->show();
    }
}
