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
 

void FlashCardBackendMode::setTestEntry(TestEntry* current)
{
    Practice::AbstractBackendMode::setTestEntry(current);
    m_solutionVisible = false;
    
    m_frontend->setQuestion("Oink Oink!");
    m_frontend->showQuestion();
    
}
 
qreal FlashCardBackendMode::verifyAnswer ( const QVariant& answer )
{
    return 0.0;
}

QVariant FlashCardBackendMode::solution()
{
//    return m_current->entry()->translation(m_backend->options()->languageTo())->text();
}

QVariant FlashCardBackendMode::question()
{
//    return m_current->entry()->translation(m_backend->options()->languageFrom())->text();
}

void FlashCardBackendMode::continueAction()
{
    if (m_solutionVisible) {
        // TODO: evaluate the grade
        kDebug() << "Answer was " << m_frontend->userInput();
        emit nextEntry();
        return;
    }
    m_frontend->showSolution();
}

#include "flashcardbackendmode.moc"
