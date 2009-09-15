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

#include <KDebug>

using namespace Practice;
 
 
FlashCardBackendMode::FlashCardBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent)
:AbstractBackendMode(practiceOptions, frontend, parent)
{
    kDebug() << "Created FlashCardBackendMode";
}

void FlashCardBackendMode::setTestEntry(TestEntry* current)
{
    Practice::AbstractBackendMode::setTestEntry(current);
    m_solutionVisible = false;
    m_frontend->showQuestion();
}

void FlashCardBackendMode::continueAction()
{
    kDebug() << "cont";
    if (m_solutionVisible) {        
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            emit currentEntryFinished();
        }        
        emit nextEntry();
        return;
    }
    m_frontend->setResultState(AbstractFrontend::AnswerWrong);
    m_frontend->showSolution();
    m_solutionVisible = true;
}

void FlashCardBackendMode::hintAction()
{
    m_frontend->setHint("This is a hint :P");
}

#include "flashcardbackendmode.moc"
