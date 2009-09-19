/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "multiplechoicebackendmode.h"
#include "defaultbackend.h"

#include <KDebug>

using namespace Practice;
 

MultipleChoiceBackendMode::MultipleChoiceBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager)
:AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_randomSequence(QDateTime::currentDateTime().toTime_t())
{
    kDebug() << "Created MultipleChoiceBackendMode";
    m_numberOfChoices = practiceOptions.numberMultipleChoiceAnswers();
}

void MultipleChoiceBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    
    QStringList answers;
    answers = m_testEntryManager->randomMultipleChoiceAnswers(m_numberOfChoices-1);
    m_correctAnswer = m_randomSequence.getLong(m_numberOfChoices);
    answers.insert(m_correctAnswer, m_current->entry()->translation(m_practiceOptions.languageTo())->text());
    kDebug() << answers;
    
    m_frontend->setQuestion(m_current->entry()->translation(m_practiceOptions.languageFrom())->text());
    m_frontend->setSolution(answers);
    m_frontend->setQuestionSound(m_current->entry()->translation(m_practiceOptions.languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_practiceOptions.languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_practiceOptions.languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_practiceOptions.languageTo())->pronunciation());
    m_solutionVisible = false;
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
}

void MultipleChoiceBackendMode::continueAction()
{
    kDebug() << "cont";
    if (m_solutionVisible) {
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            emit currentEntryFinished();
        }        
        emit nextEntry();
        return;
    }
    if (m_frontend->userInput().toInt() == m_correctAnswer) {
        m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
    } else {
        m_frontend->setResultState(AbstractFrontend::AnswerWrong);
    }
    m_frontend->showSolution();
    m_solutionVisible = true;
}

void MultipleChoiceBackendMode::hintAction()
{
    // TODO: get rid of this or make it do something useful
    m_frontend->setHint("This is a hint.");
}

#include "multiplechoicebackendmode.moc"
