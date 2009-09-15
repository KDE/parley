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
    m_state = WaitForFirstAnswer;
    Practice::AbstractBackendMode::setTestEntry(current);
    m_frontend->setQuestion(m_current->entry()->translation(m_practiceOptions.languageFrom())->text());
    m_frontend->setSolution(m_current->entry()->translation(m_practiceOptions.languageTo())->text());
    m_frontend->showQuestion();
}

void WrittenBackendMode::continueAction()
{
    switch (m_state) {
        case WaitForFirstAnswer:
        case WrongAnswer:
            checkAnswer();
            break;
        case ShowSolution:
            emit nextEntry();
            break;
    }
}

void WrittenBackendMode::checkAnswer()
{
    QString answer = m_frontend->userInput().toString();
    if (answer == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
        m_state = ShowSolution;
        m_frontend->showSolution();
        m_frontend->setFeedback(i18n("Your answer was right on the first attempt :D"));
    } else {
        m_state = WrongAnswer;
        m_frontend->setFeedback(i18n("Wrong. Idiot."));
    }
}

void WrittenBackendMode::hintAction()
{
    m_frontend->setHint(i18n("This is a hint...\nThe solution might be: %1", m_current->entry()->translation(m_practiceOptions.languageTo())->text()));
}


#include "writtenbackendmode.moc"
