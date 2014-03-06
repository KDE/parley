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

#include "prefs.h"
#include "abstractbackendmode.h"
#include "comparisonbackendmode.h"
#include "conjugationbackendmode.h"
#include "examplesentencebackendmode.h"
#include "flashcardbackendmode.h"
#include "genderbackendmode.h"
#include "multiplechoicebackendmode.h"
#include "writtenbackendmode.h"

using namespace Practice;

PracticeStateMachine::PracticeStateMachine(AbstractFrontend* frontend, ParleyDocument* doc,
                                           const PracticeOptions& options,
                                           SessionManager* sessionManager,  QObject* parent)
    : QObject(parent)
    , m_frontend(frontend)
    , m_document(doc)
    , m_options(options)
    , m_current(0)
    , m_sessionManager(sessionManager)
{
    createPracticeMode();

    // To allow to skip an an entry
    connect(m_frontend, SIGNAL(skipAction()), this, SLOT(nextEntry()));
    connect(m_frontend, SIGNAL(stopPractice()), this, SLOT(slotPracticeFinished()));
    connect(m_frontend, SIGNAL(hintAction()), m_mode, SLOT(hintAction()));

    connect(m_frontend, SIGNAL(continueAction()), this, SLOT(continueAction()));

    connect(m_mode, SIGNAL(answerRight()), this, SLOT(answerRight()));
    connect(m_mode, SIGNAL(answerWrongRetry()), this, SLOT(answerWrongRetry()));
    connect(m_mode, SIGNAL(answerWrongShowSolution()), this, SLOT(answerWrongShowSolution()));
    connect(m_mode, SIGNAL(showSolution()), this, SLOT(showSolution()));
}

void PracticeStateMachine::createPracticeMode()
{
    switch (m_options.mode()) {
    case Prefs::EnumPracticeMode::FlashCardsPractice:
        kDebug() << "Create Flash Card Practice backend";
        m_frontend->setMode(AbstractFrontend::FlashCard);
        m_mode = new FlashCardBackendMode(m_options, m_frontend, this);
        break;
    case Prefs::EnumPracticeMode::MultipleChoicePractice:
        kDebug() << "Create MultipleChoice Practice backend";
        m_frontend->setMode(AbstractFrontend::MultipleChoice);
        m_mode = new MultipleChoiceBackendMode(m_options, m_frontend, this, m_sessionManager);
        break;
    case Prefs::EnumPracticeMode::MixedLettersPractice:
        kDebug() << "Create Mixed Letters Practice backend";
        m_frontend->setMode(AbstractFrontend::MixedLetters);
        m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;
    case Prefs::EnumPracticeMode::WrittenPractice:
        kDebug() << "Create Written Practice backend";
        m_frontend->setMode(AbstractFrontend::Written);
        m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;
    case Prefs::EnumPracticeMode::ExampleSentencesPractice:
        kDebug() << "Create Written Practice backend";
        m_frontend->setMode(AbstractFrontend::ExampleSentence);
        m_mode = new ExampleSentenceBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;
    case Prefs::EnumPracticeMode::GenderPractice:
        m_frontend->setMode(AbstractFrontend::MultipleChoice);
        m_mode = new GenderBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;
    case Prefs::EnumPracticeMode::ConjugationPractice:
        m_frontend->setMode(AbstractFrontend::Conjugation);
        m_mode = new ConjugationBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;
    case Prefs::EnumPracticeMode::ComparisonPractice:
        m_frontend->setMode(AbstractFrontend::Comparison);
        m_mode = new ComparisonBackendMode(m_options, m_frontend, this, m_sessionManager, m_document->document());
        break;

    default:
        Q_ASSERT("Implement selected practice mode" == 0);
    }
}

void Practice::PracticeStateMachine::start()
{
    kDebug() << "Start practice";
    m_sessionManager->practiceStarted();
    nextEntry();
}

void PracticeStateMachine::nextEntry()
{
    m_state = NotAnswered;
    m_current = m_sessionManager->getNextEntry();

    kDebug() << "GETTING ENTRY - " << m_current;

    //after going through all words, or at the start of practice
    if (m_current == 0) {
        slotPracticeFinished();
        return;
    }
    if (!m_mode->setTestEntry(m_current)) {
        // this is just a fall back, if an invalid entry slipped through
        currentEntryFinished();
        nextEntry();
    }
    updateFrontend();
}

void PracticeStateMachine::slotPracticeFinished()
{
    kDebug() << "Stop practice";
    m_sessionManager->practiceFinished();
    emit practiceFinished();
}

void PracticeStateMachine::currentEntryFinished()
{
    m_sessionManager->removeCurrentEntryFromPractice();
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

    m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
    if (m_state == NotAnswered) {
        m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
    } else {
        m_frontend->setResultState(AbstractFrontend::AnswerWrong);
    }

    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::answerWrongRetry()
{
    kDebug() << "wrong retr";
    m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
    m_state = AnswerWasWrong;
}

void PracticeStateMachine::answerWrongShowSolution()
{
    kDebug() << "wrong sol";
    m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
    //User gave an empty answer or the same answer for a second time so we want to drop out.
    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::showSolution()
{
    kDebug() << "show solution";
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::updateFrontend()
{
    m_frontend->setFeedbackState(AbstractFrontend::QuestionState);
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->setLessonName(m_current->entry()->lesson()->name());

    // show the word that is currently practiced in the progress bar
    m_frontend->setFinishedWordsTotalWords(
        m_sessionManager->totalEntryCount() - m_sessionManager->activeEntryCount(),
        m_sessionManager->totalEntryCount());

    grade_t grade = m_mode->currentGradeForEntry();
    grade_t goodGrade = qMax(grade, grade_t(KV_LEV1_GRADE)); // if the word hasn't been practiced yet, use grade 1 as a base

    // Normal mode: check if current word was not answered wrong -> if yes, mark next grade as achievable
    // Alternative mode (3 consecutive answers): check if word was not answered wrong and two consecutive times correct -> if yes, mark next grade as achievable
    if ((!(Prefs::altLearn()) && m_current->statisticBadCount() == 0) ||
        (m_current->statisticBadCount() == 0 && m_current->answeredCorrectInSequence() == 2 && Prefs::altLearn())) {
        goodGrade = qMax(KV_LEV2_GRADE, qMin(grade + 1, KV_MAX_GRADE));
    }

    m_frontend->setBoxes(grade, goodGrade, KV_LEV1_GRADE);

    QString imgFrom = m_current->entry()->translation(m_options.languageFrom())->imageUrl().url();
    QString imgTo = m_current->entry()->translation(m_options.languageTo())->imageUrl().url();
    if (imgFrom.isEmpty()) {
        imgFrom = imgTo;
    }
    if (imgTo.isEmpty()) {
        imgTo = imgFrom;
    }
    if (Prefs::flashcardsFrontImage()) {
        m_frontend->setQuestionImage(imgFrom);
    } else {
        m_frontend->setQuestionImage(QString());
    }
    if (Prefs::flashcardsBackImage()) {
        m_frontend->setSolutionImage(imgTo);
    } else {
        m_frontend->setSolutionImage(QString());
    }
    m_frontend->showQuestion();
}

void PracticeStateMachine::gradeEntryAndContinue()
{
    grade_t currentGrade = m_mode->currentGradeForEntry();

    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
	m_current->updateStatisticsRightAnswer(currentGrade);
    } else {
        m_current->updateStatisticsWrongAnswer(currentGrade);
    }

    kDebug() << "entry finished: " << m_frontend->resultState() << " change grades? " << m_current->changeGrades();
    if (m_current->isUnseenQuestion()) {
	m_mode->updateGrades();
    }
    if (m_current->changeGrades()) {
        m_mode->updateGrades();
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            currentEntryFinished();
        }
    }
    emit nextEntry();
}

#include "practicestatemachine.moc"
