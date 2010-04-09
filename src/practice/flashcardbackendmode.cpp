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


#include "flashcardbackendmode.h"
#include "defaultbackend.h"

using namespace Practice;
 
 
FlashCardBackendMode::FlashCardBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent)
:AbstractBackendMode(practiceOptions, frontend, parent)
{

}

void FlashCardBackendMode::setTestEntry(TestEntry* current)
{
    Practice::AbstractBackendMode::setTestEntry(current);
    m_current = current;
    m_solutionVisible = false;
    m_frontend->showQuestion();
}

void FlashCardBackendMode::continueAction()
{
    if (m_solutionVisible) {        
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            emit currentEntryFinished();
            m_current->incGoodCount();
        } else {
            m_current->incBadCount();
        }
        emit nextEntry();
        return;
    }
    m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
    m_frontend->showSolution();
    m_solutionVisible = true;
}

void FlashCardBackendMode::hintAction()
{
    // TODO: get rid of this or make it do something useful
    m_frontend->setHint("This is a hint.");
}

#include "flashcardbackendmode.moc"
