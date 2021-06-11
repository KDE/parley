/*
    SPDX-FileCopyrightText: 2009-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "practicestatemachine.h"

#include "parleydocument.h"

#include "prefs.h"
#include "comparisonbackendmode.h"
#include "conjugationbackendmode.h"
#include "examplesentencebackendmode.h"
#include "flashcardbackendmode.h"
#include "genderbackendmode.h"
#include "multiplechoicebackendmode.h"
#include "writtenbackendmode.h"

using namespace Practice;

PracticeStateMachine::PracticeStateMachine(AbstractFrontend* frontend, ParleyDocument* doc,
                                           SessionManagerBase* sessionManager,  QObject* parent)
    : QObject(parent)
    , m_frontend(frontend)
    , m_document(doc)
    , m_current(0)
    , m_sessionManager(sessionManager)
{
    createPracticeMode();

    // To allow to skip an an entry
    connect(m_frontend, &AbstractFrontend::skipAction, this, &PracticeStateMachine::nextEntry);
    connect(m_frontend, &AbstractFrontend::stopPractice, this, &PracticeStateMachine::slotPracticeFinished);
    connect(m_frontend, &AbstractFrontend::hintAction, m_mode, &AbstractBackendMode::hintAction);

    connect(m_frontend, &AbstractFrontend::continueAction, this, &PracticeStateMachine::continueAction);

    connect(m_mode, &AbstractBackendMode::answerRight, this, &PracticeStateMachine::answerRight);
    connect(m_mode, &AbstractBackendMode::answerWrongRetry, this, &PracticeStateMachine::answerWrongRetry);
    connect(m_mode, &AbstractBackendMode::answerWrongShowSolution, this, &PracticeStateMachine::answerWrongShowSolution);
    connect(m_mode, &AbstractBackendMode::showSolution, this, &PracticeStateMachine::showSolution);
}

void PracticeStateMachine::createPracticeMode()
{
    switch (Prefs::practiceMode()) {
    case Prefs::EnumPracticeMode::FlashCardsPractice:
        qDebug() << "Create Flash Card Practice backend";
        m_frontend->setMode(AbstractFrontend::FlashCard);
        m_mode = new FlashCardBackendMode(m_frontend, this);
        break;
    case Prefs::EnumPracticeMode::MultipleChoicePractice:
        qDebug() << "Create MultipleChoice Practice backend";
        m_frontend->setMode(AbstractFrontend::MultipleChoice);
        m_mode = new MultipleChoiceBackendMode(m_frontend, this, m_sessionManager);
        break;
    case Prefs::EnumPracticeMode::MixedLettersPractice:
        qDebug() << "Create Mixed Letters Practice backend";
        m_frontend->setMode(AbstractFrontend::MixedLetters);
        m_mode = new WrittenBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;
    case Prefs::EnumPracticeMode::WrittenPractice:
        qDebug() << "Create Written Practice backend";
        m_frontend->setMode(AbstractFrontend::Written);
        m_mode = new WrittenBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;
    case Prefs::EnumPracticeMode::ExampleSentencesPractice:
        qDebug() << "Create Written Practice backend";
        m_frontend->setMode(AbstractFrontend::ExampleSentence);
        m_mode = new ExampleSentenceBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;
    case Prefs::EnumPracticeMode::GenderPractice:
        m_frontend->setMode(AbstractFrontend::MultipleChoice);
        m_mode = new GenderBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;
    case Prefs::EnumPracticeMode::ConjugationPractice:
        m_frontend->setMode(AbstractFrontend::Conjugation);
        m_mode = new ConjugationBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;
    case Prefs::EnumPracticeMode::ComparisonPractice:
        m_frontend->setMode(AbstractFrontend::Comparison);
        m_mode = new ComparisonBackendMode(m_frontend, this, m_sessionManager, m_document->document().get());
        break;

    default:
        Q_ASSERT("Implement selected practice mode" == 0);
    }
}

void Practice::PracticeStateMachine::start()
{
    qDebug() << "Start practice";
    m_sessionManager->practiceStarted();
    nextEntry();
}

void PracticeStateMachine::nextEntry()
{
    m_state = NotAnswered;
    m_current = m_sessionManager->nextTrainingEntry();

    //qDebug() << "GETTING ENTRY - " << m_current;

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
    qDebug() << "Stop practice";
    m_sessionManager->practiceFinished();
    emit practiceFinished();
}

void PracticeStateMachine::currentEntryFinished()
{
    m_sessionManager->removeCurrentEntryFromPractice();
}

void PracticeStateMachine::continueAction()
{
    //qDebug() << "continue" << m_state;
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
    //qDebug() << "ans right";

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
    //qDebug() << "wrong retr";
    m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
    m_state = AnswerWasWrong;
}

void PracticeStateMachine::answerWrongShowSolution()
{
    //qDebug() << "wrong sol";
    m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
    //User gave an empty answer or the same answer for a second time so we want to drop out.
    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::showSolution()
{
    //qDebug() << "show solution";
    m_state = SolutionShown;
    m_frontend->showSolution();
}

void PracticeStateMachine::updateFrontend()
{
    m_frontend->setFeedbackState(AbstractFrontend::QuestionState);
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->setLessonName(m_current->entry()->lesson()->name());
    grade_t grade = m_mode->currentGradeForEntry();
    m_frontend->showGrade(m_mode->currentPreGradeForEntry(), grade);

    // show the word that is currently practiced in the progress bar
    m_frontend->setFinishedWordsTotalWords(
        m_sessionManager->allEntryCount() - m_sessionManager->activeEntryCount(),
        m_sessionManager->allEntryCount());

    // Set fonts
    m_frontend->setQuestionFont((m_current->languageFrom() == Prefs::learningLanguage())
                                ? m_frontend->learningLangFont()
                                : m_frontend->knownLangFont());
    m_frontend->setSolutionFont((m_current->languageTo() == Prefs::learningLanguage())
                                ? m_frontend->learningLangFont()
                                : m_frontend->knownLangFont());

    grade_t goodGrade = qMax(grade, grade_t(KV_LEV1_GRADE)); // if the word hasn't been practiced yet, use grade 1 as a base

    if (m_current->statisticBadCount() == 0) {
        goodGrade = qMax(KV_LEV2_GRADE, qMin(grade + 1, KV_MAX_GRADE));
    }

    m_frontend->setBoxes(grade, goodGrade, KV_LEV1_GRADE);

    QUrl imgFrom = m_current->entry()->translation(m_current->languageFrom())->imageUrl();
    QUrl imgTo = m_current->entry()->translation(m_current->languageTo())->imageUrl();
    if (imgFrom.isEmpty()) {
        imgFrom = imgTo;
    }
    if (imgTo.isEmpty()) {
        imgTo = imgFrom;
    }
    if (Prefs::flashcardsFrontImage()) {
        m_frontend->setQuestionImage(imgFrom);
    } else {
        m_frontend->setQuestionImage(QUrl());
    }
    if (Prefs::flashcardsBackImage()) {
        m_frontend->setSolutionImage(imgTo);
    } else {
        m_frontend->setSolutionImage(QUrl());
    }
    m_frontend->showQuestion();
}

void PracticeStateMachine::gradeEntryAndContinue()
{
    grade_t currentPreGrade = m_mode->currentPreGradeForEntry();
    grade_t currentGrade    = m_mode->currentGradeForEntry();

    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
   m_current->updateStatisticsRightAnswer(currentPreGrade, currentGrade);
    } else {
        m_current->updateStatisticsWrongAnswer(currentPreGrade, currentGrade);
    }

    if (m_current->shouldChangeGrades()) {
        m_mode->updateGrades();
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            currentEntryFinished();
        }
    }
    emit nextEntry();
}
