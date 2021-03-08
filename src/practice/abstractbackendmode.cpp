/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractbackendmode.h"
#include "prefs.h"
#include <QDebug>

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
    if (Prefs::practiceSoundEnabled() == true) {
        m_frontend->setQuestionSound(
            m_current->entry()-> translation(m_current->languageFrom())->soundUrl());
        m_frontend->setSolutionSound(
            m_current->entry()->translation(m_current->languageTo())->soundUrl());
    }
    m_frontend->setQuestionPronunciation(
        m_current->entry()->translation(m_current->languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(
        m_current->entry()->translation(m_current->languageTo())->pronunciation());

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
    //qDebug() << "Update Grades Default Implementation: " << m_frontend->resultState()
    //         << " for " << translation->text()
    //         << " grade: " << m_current->entry()->translation(m_current->languageTo())->grade();

    translation->incPracticeCount();
    translation->setPracticeDate(QDateTime::currentDateTime());

    updateGrade(*translation, m_frontend->resultState() == AbstractFrontend::AnswerCorrect,
                m_current->statisticBadCount() == 0);

    //qDebug() << "new grade: " << m_current->entry()->translation(m_current->languageTo())->grade();
}


// ----------------------------------------------------------------
//                         protected methods


void Practice::AbstractBackendMode::updateGrade(KEduVocText &text, bool isCorrectAnswer,
                                                bool hasNoPreviousBadAnswers)
{
    if (isCorrectAnswer) {
        if (hasNoPreviousBadAnswers) {
            if (text.grade() == KV_NORM_GRADE) {
                if (text.preGrade()  == KV_NORM_GRADE) {
                    // If the word was new and correct answer, then
                    // set it to grade, pregrade 2, 0.  This is so
                    // that the user doesn't have to go through all
                    // the pregrade levels with words that s/he
                    // already knows well.
                    text.setPreGrade(KV_NORM_GRADE);
                    text.setGrade(KV_LEV2_GRADE);
                }
                else if (text.preGrade() == KV_MAX_GRADE) {
                    // If correct answer and last pregrade level, then
                    // set new grade, pregrade to 1, 0.
                    text.setPreGrade(KV_NORM_GRADE);
                    text.setGrade(KV_LEV1_GRADE);
                }
                else {
                    // otherwise just increase the pregrade.
                    text.setPreGrade(text.preGrade() + 1); // FIXME: Implement incPreGrade() in the library.
                }
            }
            else {
                // If grade was > 0 then just increase it.
                text.incGrade();
            }
        }
    } else {
        // If the answer was wrong, reset the grade, pregrade to 0, 1.
        text.setPreGrade(KV_LEV1_GRADE);
        text.setGrade(KV_NORM_GRADE);
        text.incBadCount();
    }

    //qDebug() << "new pregrade, grade: " << text.preGrade() << text.grade();
}
