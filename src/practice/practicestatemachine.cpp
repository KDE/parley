/***************************************************************************
    Copyright 2009-2010 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicestatemachine.h"

#include "parleydocument.h"

#include "abstractbackendmode.h"
#include "comparisonbackendmode.h"
#include "conjugationbackendmode.h"
#include "examplesentencebackendmode.h"
#include "flashcardbackendmode.h"
#include "genderbackendmode.h"
#include "multiplechoicebackendmode.h"
#include "writtenbackendmode.h"

using namespace Practice;

PracticeStateMachine::PracticeStateMachine(AbstractFrontend* frontend, ParleyDocument* doc, const PracticeOptions& options, TestEntryManager* testEntryManager,  QObject* parent)
:QObject(parent)
,m_frontend(frontend)
,m_document(doc)
,m_options(options)
,m_current(0)
,m_testEntryManager(testEntryManager)
{
    createPracticeMode();

    // To allow to skip an an entry
    connect(m_frontend, SIGNAL(skipAction()), this, SLOT(nextEntry()));
    connect(m_frontend, SIGNAL(stopPractice()), this, SIGNAL(practiceFinished()));
    connect(m_frontend, SIGNAL(hintAction()), m_mode, SLOT(hintAction()));

    connect(m_frontend, SIGNAL(continueAction()), this, SLOT(continueAction()));

    connect(m_mode, SIGNAL(answerRight()), this, SLOT(answerRight()));
    connect(m_mode, SIGNAL(answerWrongRetry()), this, SLOT(answerWrongRetry()));
    connect(m_mode, SIGNAL(answerWrongShowSolution()), this, SLOT(answerWrongShowSolution()));
}

void PracticeStateMachine::createPracticeMode()
{
    switch(m_options.mode()) {
        case Prefs::EnumPracticeMode::FlashCardsPractice:
            kDebug() << "Create Flash Card Practice backend";
            m_frontend->setMode(AbstractFrontend::FlashCard);
            m_mode = new FlashCardBackendMode(m_options, m_frontend, this);
            break;
        case Prefs::EnumPracticeMode::MultipleChoicePractice:
            kDebug() << "Create MultipleChoice Practice backend";
            m_frontend->setMode(AbstractFrontend::MultipleChoice);
            m_mode = new MultipleChoiceBackendMode(m_options, m_frontend, this, m_testEntryManager);
            break;
        case Prefs::EnumPracticeMode::MixedLettersPractice:
            kDebug() << "Create Mixed Letters Practice backend";
            m_frontend->setMode(AbstractFrontend::MixedLetters);
            m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::WrittenPractice:
            kDebug() << "Create Written Practice backend";
            m_frontend->setMode(AbstractFrontend::Written);
            m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::ExampleSentencesPractice:
            kDebug() << "Create Written Practice backend";
            m_frontend->setMode(AbstractFrontend::Written);
            m_mode = new ExampleSentenceBackendMode(m_options, m_frontend, this,m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::GenderPractice:
            m_frontend->setMode(AbstractFrontend::MultipleChoice);
            m_mode = new GenderBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::ConjugationPractice:
            m_frontend->setMode(AbstractFrontend::Conjugation);
            m_mode = new ConjugationBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::ComparisonPractice:
            m_frontend->setMode(AbstractFrontend::Comparison);
            m_mode = new ComparisonBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;

        default:
            Q_ASSERT("Implement selected practice mode" == 0);
    }
}

void Practice::PracticeStateMachine::start()
{
    kDebug() << "Start practice";
    nextEntry();
}

void PracticeStateMachine::nextEntry()
{
    m_state = NotAnswered;
    m_current = m_testEntryManager->getNextEntry();

    kDebug() << "GETTING ENTRY - " << m_current;

    //after going through all words, or at the start of practice
    if (m_current == 0) {
        emit practiceFinished();
        return;
    }
    if (m_mode->setTestEntry(m_current)) {
        updateFrontend();
    } else {
        // this is just a fall back, if an invalid entry slipped through
        currentEntryFinished();
        nextEntry();
    }
}

void PracticeStateMachine::currentEntryFinished()
{
    m_testEntryManager->removeCurrentEntryFromPractice();
}

void PracticeStateMachine::continueAction()
{
    kDebug() << "continue" << m_state;
    switch (m_state) {
        // on continue, we check the answer, if in NotAnsweredState or AnswerWasWrongState
        case NotAnswered:
        case AnswerWasWrong:
            m_mode->checkAnswer();
            break;

        case SolutionShown:
            gradeEntryAndContinue();
            break;
    }
}

void PracticeStateMachine::answerRight()
{
    kDebug() << "ans right";
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::answerWrongRetry()
{
    kDebug() << "wrong retr";
    m_state = AnswerWasWrong;
}

void PracticeStateMachine::answerWrongShowSolution()
{
    kDebug() << "wrong sol";
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::updateFrontend()
{
    m_frontend->setFeedbackState(AbstractFrontend::QuestionState);
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->setLessonName(m_current->entry()->lesson()->name());
    m_frontend->setFinishedWordsTotalWords(
        m_testEntryManager->totalEntryCount() - m_testEntryManager->activeEntryCount(),
        m_testEntryManager->totalEntryCount());

    int grade = m_current->entry()->translation(m_options.languageTo())->grade();
    m_frontend->setBoxes(grade, qMin(grade+1, KV_MAX_GRADE), KV_LEV1_GRADE);

    QString imgUrl = m_current->entry()->translation(m_options.languageFrom())->imageUrl().url();
    m_frontend->setQuestionImage(imgUrl);
}

void PracticeStateMachine::gradeEntryAndContinue()
{
    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
        m_current->updateStatisticsRightAnswer();
    } else {
        m_current->updateStatisticsWrongAnswer();
    }

    kDebug() << "entry finished: " << m_frontend->resultState() << " change grades? " << m_current->changeGrades();
    if (m_current->changeGrades()) {
        m_mode->updateGrades();
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            currentEntryFinished();
        }
    }
    emit nextEntry();
}

#include "practicestatemachine.moc"
