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
    kDebug() << "Created WrittenBackendMode";
}

void WrittenBackendMode::setTestEntry(TestEntry* current)
{
    AbstractBackendMode::setTestEntry(current);
    m_state = NotAnswered;
    m_frontend->showQuestion();
    m_lastAnswer.clear();
}

void WrittenBackendMode::continueAction()
{    
    kDebug() << "cont -- state:" <<  m_state;
    switch (m_state) {
        case NotAnswered:
        case AnswerWasWrong:
            checkAnswer();
            break;
        case SolutionShown:
            if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
                m_current->incGoodCount();
                emit currentEntryFinished();
            }
            emit nextEntry();
            break;
    }
}

void WrittenBackendMode::checkAnswer()
{
    QString answer = m_frontend->userInput().toString();
    
    switch(m_state) {
        case NotAnswered:
            if (answer == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
                m_frontend->setFeedback(i18n("Your answer was right on the first attempt :D"));
                m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
                m_frontend->showSolution();
                m_state = SolutionShown;
            } else {
                if (answer == m_lastAnswer) {
                    m_frontend->setFeedback(i18n("You did not answer correctly."));
                    m_state = SolutionShown;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                    m_frontend->showSolution();
                } else {
                    m_frontend->setFeedback(i18n("Try again - I fear that was not right..."));
                    m_state = AnswerWasWrong;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                }
            }
            break;
        case AnswerWasWrong:
            if (answer == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
                m_frontend->setFeedback(i18n("Your answer was right... but not on the first try."));
                m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
                m_frontend->showSolution();
                m_state = SolutionShown;                
            } else {
                if (answer == m_lastAnswer) {
                    m_frontend->setFeedback(i18n("You did not answer correctly."));
                    m_state = SolutionShown;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
                    m_frontend->showSolution();
                } else {
                    m_frontend->setFeedback(i18n("Wrong. Idiot."));
                    m_state = AnswerWasWrong;
                    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
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
    m_frontend->setHint(i18n("This is a hint...\nThe solution might be: %1", m_current->entry()->translation(m_practiceOptions.languageTo())->text()));
}


#include "writtenbackendmode.moc"
