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
    ,m_current(0)
{
}

bool AbstractBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    // this default implementation sets up the frontend with the normal word
    m_frontend->setQuestion(m_current->entry()->translation(m_practiceOptions.languageFrom())->text());
    m_frontend->setSolution(m_current->entry()->translation(m_practiceOptions.languageTo())->text());
    m_frontend->setQuestionSound(m_current->entry()->translation(m_practiceOptions.languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_practiceOptions.languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_practiceOptions.languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_practiceOptions.languageTo())->pronunciation());

    return true;
}

grade_t Practice::AbstractBackendMode::currentGradeForEntry()
{
    return m_current->entry()->translation(m_practiceOptions.languageTo())->grade();
}

void Practice::AbstractBackendMode::updateGrades()
{
    KEduVocTranslation* translation = m_current->entry()->translation(m_practiceOptions.languageTo());
    kDebug() << "Update Grades Default Implementation: " << m_frontend->resultState() << " for " << translation->text()
        << " grade: " << m_current->entry()->translation(m_practiceOptions.languageTo())->grade();

    translation->incPracticeCount();
    translation->setPracticeDate( QDateTime::currentDateTime() );

    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
        if (m_current->statisticBadCount() == 0) {
            translation->incGrade();
        }
    } else {
        translation->setGrade(KV_LEV1_GRADE);
        translation->incBadCount();
    }
    kDebug() << "new grade: " << m_current->entry()->translation(m_practiceOptions.languageTo())->grade();
}

#include "abstractbackendmode.moc"
