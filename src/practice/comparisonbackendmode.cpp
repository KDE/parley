/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "comparisonbackendmode.h"

#include <KLocalizedString>

#include "documentsettings.h"

using namespace Practice;

ComparisonBackendMode::ComparisonBackendMode(AbstractFrontend* frontend, QObject* parent,
                                             Practice::SessionManagerBase* sessionManager,
                                             KEduVocDocument* doc)
    : AbstractBackendMode(frontend, parent)
    , m_sessionManager(sessionManager)
    , m_doc(doc)
{
}

bool ComparisonBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    m_lastAnswers.clear();

    int languageTo = current->languageTo();
    int languageFrom = current->languageFrom();

    m_frontend->setQuestion(m_current->entry()->translation(languageFrom)->text());
    QStringList answers;
    answers.append(m_current->entry()->translation(languageTo)->text());
    answers.append(m_current->entry()->translation(languageTo)->comparativeForm().text());
    answers.append(m_current->entry()->translation(languageTo)->superlativeForm().text());
    m_frontend->setSolution(answers);

    m_frontend->setQuestionSound(m_current->entry()->translation(m_current->languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_current->languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_current->languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_current->languageTo())->pronunciation());
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
    return true;
}

void ComparisonBackendMode::checkAnswer()
{
    QStringList answers = m_frontend->userInput().toStringList();

    if (answers == m_lastAnswers) {
        emit answerWrongShowSolution();
        return;
    }

    bool absoluteCorrect = answers.at(0) == m_current->entry()->translation(m_current->languageTo())->text();
    bool comparativeCorrect = answers.at(1) == m_current->entry()->translation(m_current->languageTo())->comparativeForm().text();
    bool superlativeCorrect = answers.at(2) == m_current->entry()->translation(m_current->languageTo())->superlativeForm().text();

    if (absoluteCorrect && comparativeCorrect && superlativeCorrect) {
        m_frontend->setFeedback(i18n("All comparison forms were right."));
        emit answerRight();
    } else {
        if (!absoluteCorrect) {
            m_frontend->setFeedback(i18nc("the user entered the wrong absolute form when practicing comparison forms of adjectives (the base form of the adjective is wrong)",
                                          "\"%1\" is the wrong word.", answers.at(0)));
        } else {
            if ((!comparativeCorrect) && (!superlativeCorrect)) {
                m_frontend->setFeedback(i18nc("the user entered the wrong comparison forms when practicing comparison forms of adjectives (good, better, best)",
                                              "Both comparison forms (comparative and superlative) are wrong."));
            } else if (!comparativeCorrect) {
                m_frontend->setFeedback(i18nc("the user entered the wrong comparison forms when practicing comparison forms of adjectives (second form wrong - better)",
                                              "The comparative is wrong."));
            } else if (!superlativeCorrect) {
                m_frontend->setFeedback(i18nc("the user entered the wrong comparison forms when practicing comparison forms of adjectives (third form wrong - best)",
                                              "The superlative is wrong."));
            }
        }
        emit answerWrongRetry();
    }
    m_lastAnswers = answers;
}

void ComparisonBackendMode::hintAction()
{
    // FIXME
}

void ComparisonBackendMode::updateGrades()
{
    QStringList answers = m_frontend->userInput().toStringList();

    bool absoluteCorrect = answers.at(0) == m_current->entry()->translation(m_current->languageTo())->text();
    bool comparativeCorrect = answers.at(1) == m_current->entry()->translation(m_current->languageTo())->comparativeForm().text();
    bool superlativeCorrect = answers.at(2) == m_current->entry()->translation(m_current->languageTo())->superlativeForm().text();

    // TODO way too much duplicated code here

    KEduVocTranslation* translation = m_current->entry()->translation(m_current->languageTo());

    translation->incPracticeCount();
    translation->setPracticeDate(QDateTime::currentDateTime());

    updateGrade(*translation, absoluteCorrect, m_current->statisticBadCount() == 0);

    KEduVocText comp = translation->comparativeForm();

    comp.incPracticeCount();
    comp.setPracticeDate(QDateTime::currentDateTime());

    updateGrade(comp, comparativeCorrect, m_current->statisticBadCount() == 0);
    translation->setComparativeForm(comp);

    KEduVocText super = translation->superlativeForm();

    super.incPracticeCount();
    super.setPracticeDate(QDateTime::currentDateTime());

    updateGrade(super, superlativeCorrect, m_current->statisticBadCount() == 0);
    translation->setSuperlativeForm(super);
}

grade_t ComparisonBackendMode::currentPreGradeForEntry() const
{
    KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageTo());
    if (translation) {
        return qMin(translation->comparativeForm().preGrade(), translation->superlativeForm().preGrade());
    }
    return KV_NORM_GRADE;
}

grade_t ComparisonBackendMode::currentGradeForEntry() const
{
    KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageTo());
    if (translation) {
        return qMin(translation->comparativeForm().grade(), translation->superlativeForm().grade());
    }
    return KV_NORM_GRADE;
}
