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

#include <kdebug.h>

using namespace Practice;

AbstractBackendMode::AbstractBackendMode(Practice::AbstractFrontend* frontend, QObject *parent)
    : QObject(parent)
    , m_frontend(frontend)
    , m_current(0)
{
}

bool AbstractBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    // this default implementation sets up the frontend with the normal word
    KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageFrom());
    if (!translation->text().isEmpty()) {
        m_frontend->setQuestion(translation->text());
    } else if (!translation->imageUrl().isEmpty()) {
        m_frontend->setQuestion(translation->imageUrl());
    }
    m_frontend->setSolution(m_current->entry()->translation(m_current->languageTo())->text());
    m_frontend->setQuestionSound(m_current->entry()->translation(m_current->languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_current->languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_current->languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_current->languageTo())->pronunciation());

    return true;
}

grade_t Practice::AbstractBackendMode::currentPreGradeForEntry() const
{
    return m_current->entry()->translation(m_current->languageTo())->preGrade();
}

grade_t Practice::AbstractBackendMode::currentGradeForEntry() const
{
    return m_current->entry()->translation(m_current->languageTo())->grade();
}

void Practice::AbstractBackendMode::updateGrades()
{
    KEduVocTranslation* translation = m_current->entry()->translation(m_current->languageTo());
    //kDebug() << "Update Grades Default Implementation: " << m_frontend->resultState()
    //         << " for " << translation->text()
    //         << " grade: " << m_current->entry()->translation(m_current->languageTo())->grade();

    translation->incPracticeCount();
    translation->setPracticeDate(QDateTime::currentDateTime());

    updateGrade(*translation, m_frontend->resultState() == AbstractFrontend::AnswerCorrect,
                m_current->statisticBadCount() == 0);

    //kDebug() << "new grade: " << m_current->entry()->translation(m_current->languageTo())->grade();
}


// ----------------------------------------------------------------
//                         protected methods


void Practice::AbstractBackendMode::updateGrade(KEduVocText &text, bool isCorrectAnswer,
                                                bool hasNoPreviousBadAnswers)
{
    if (isCorrectAnswer) {
        if (hasNoPreviousBadAnswers) {
            if (text.grade() == KV_NORM_GRADE) {
                if (text.preGrade() == KV_MAX_GRADE) {
                    text.setPreGrade(KV_NORM_GRADE);
                    text.setGrade(KV_LEV1_GRADE);
                }
                else {
                    text.setPreGrade(text.preGrade() + 1); // FIXME: Implement incPreGrade()
                }
            }
            else {
                text.incGrade();
            }
        }
    } else {
        text.setPreGrade(KV_LEV1_GRADE);
        text.setGrade(KV_NORM_GRADE);
        text.incBadCount();
    }

    //kDebug() << "new pregrade, grade: " << text.preGrade() << text.grade();
}



#include "abstractbackendmode.moc"
