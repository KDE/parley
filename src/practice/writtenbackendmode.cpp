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


#include "writtenbackendmode.h"

#include "defaultbackend.h"

#include <KLocalizedString>

using namespace Practice;


WrittenBackendMode::WrittenBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent)
:AbstractBackendMode(practiceOptions, frontend, parent)
{

}

void WrittenBackendMode::setTestEntry(TestEntry* current)
{
    AbstractBackendMode::setTestEntry(current);
    m_state = NotAnswered;
    m_frontend->showQuestion();
    m_lastAnswer.clear();
    m_synonyms.clear();
}

void WrittenBackendMode::continueAction()
{    
    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
        m_state = SolutionShown; // the user manually toggled the state, so we just count is as correct without showing the solution
    }
    switch (m_state) {
        case NotAnswered:
        case AnswerWasWrong:
        case AnswerWasSynonym:
            checkAnswer();
            break;
        case SolutionShown:
            if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
                m_current->incGoodCount();
                emit currentEntryFinished();
            } else {
                m_current->incBadCount();
            }
            m_currentHint = QString();
            emit nextEntry();
            break;
    }
}

void WrittenBackendMode::checkAnswer()
{
    QString answer = m_frontend->userInput().toString();
    
    if(m_state == NotAnswered && answer.isEmpty())
        m_state = AnswerWasWrong;

    switch(m_state) {
        // for now right/wrong is only counted on first attempt - when state is NotAnswered.
        case NotAnswered:
        case AnswerWasSynonym:
            if (answer == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
                m_frontend->setFeedback(i18n("Your answer was right on the first attempt."));
                m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
                m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
                m_frontend->showSolution();

                m_state = SolutionShown;
            } else if (isSynonym(answer)) {
                if (m_synonyms.contains(answer)) {
                    m_frontend->setFeedback(i18n("Your answer was an already entered synonym."));
                } else {
                    m_frontend->setFeedback(i18n("Your answer was a synonym."));
                    AbstractBackendMode::addSynonym(answer);
                    m_frontend->setSynonym(answer);
                    m_frontend->showSynonym();
                }
                m_frontend->setResultState(AbstractFrontend::AnswerSynonym);
                m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);

                m_state = AnswerWasSynonym;
            } else {
                m_frontend->setFeedback(i18n("Your answer was wrong. Please try again."));
                m_state = AnswerWasWrong;
                m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                m_current->addUserAnswer(answer);
            }
            break;
        case AnswerWasWrong:
            if (answer == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
                m_frontend->setFeedback(i18n("Your answer was right... but not on the first try."));
                m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
                m_frontend->showSolution();
                m_state = SolutionShown;                
            } else {
                if (answer == m_lastAnswer) {
                    m_frontend->setFeedback(i18n("You did not answer correctly."));
                    m_state = SolutionShown;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                    m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
                    m_frontend->showSolution();
                } else {
                    m_frontend->setFeedback(i18n("Your answer was wrong. Please try again."));
                    m_state = AnswerWasWrong;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                    m_current->addUserAnswer(answer);
                }
            }
            break;
        case SolutionShown:
            break;
    }
    m_lastAnswer = answer;
}

void WrittenBackendMode::hintAction()
{
    QString solution = m_current->entry()->translation(m_practiceOptions.languageTo())->text();
    m_currentHint = solution.left(m_currentHint.size() + 1);
    if (m_currentHint.size() == solution.size()) {
        // show solution
        m_frontend->setFeedback(i18n("You revealed the answer by using too many hints."));
        m_frontend->setResultState(AbstractFrontend::AnswerWrong);
        if (m_frontend->userInput().toString() == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
        } else {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
        }
        m_frontend->showSolution();
        m_state = SolutionShown;
    } else {
        m_frontend->setHint(i18n("The solution starts with: %1", m_currentHint));
    }
}

bool WrittenBackendMode::isSynonym(QString& answer)
{
    foreach(KEduVocTranslation *synonym, m_current->entry()->translation(m_practiceOptions.languageTo())->synonyms()) {
        kDebug() << "Synonym" << synonym->text() << " answer: " << answer;
        if (synonym->text() == answer) {
            return true;
        }
    }
    return false;
}
#include "writtenbackendmode.moc"
