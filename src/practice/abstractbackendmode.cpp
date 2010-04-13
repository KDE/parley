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


#include "abstractbackendmode.h"

using namespace Practice;

AbstractBackendMode::AbstractBackendMode(const PracticeOptions& practiceOptions, Practice::AbstractFrontend* frontend, QObject *parent)
    :QObject(parent)
    ,m_practiceOptions(practiceOptions)
    ,m_frontend(frontend)
{}

void AbstractBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    m_frontend->setQuestion(m_current->entry()->translation(m_practiceOptions.languageFrom())->text());
    m_frontend->setSolution(m_current->entry()->translation(m_practiceOptions.languageTo())->text());
    m_frontend->setQuestionSound(m_current->entry()->translation(m_practiceOptions.languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_practiceOptions.languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_practiceOptions.languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_practiceOptions.languageTo())->pronunciation());

    m_frontend->setResultState(AbstractFrontend::QuestionState);
}

#include "abstractbackendmode.moc"
