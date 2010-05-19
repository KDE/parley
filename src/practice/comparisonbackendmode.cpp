/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "comparisonbackendmode.h"

#include <KLocalizedString>

#include "defaultbackend.h"

#include "documentsettings.h"

using namespace Practice;

ComparisonBackendMode::ComparisonBackendMode(const PracticeOptions& practiceOptions,
        AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc)
: AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_doc(doc)
{
}

void ComparisonBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;

    int languageTo = m_practiceOptions.languageTo();
    int languageFrom = m_practiceOptions.languageFrom();

    m_frontend->setQuestion(m_current->entry()->translation(languageFrom)->text());
    QStringList answers;
    answers.append(m_current->entry()->translation(languageTo)->text());
    answers.append(m_current->entry()->translation(languageTo)->comparative());
    answers.append(m_current->entry()->translation(languageTo)->superlative());
    m_frontend->setSolution(answers);

    m_frontend->setQuestionSound(m_current->entry()->translation(m_practiceOptions.languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_practiceOptions.languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_practiceOptions.languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_practiceOptions.languageTo())->pronunciation());
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
}

void ComparisonBackendMode::continueAction()
{
    if (m_frontend->resultState() == AbstractFrontend::QuestionState) {
        checkAnswer();
        m_frontend->showSolution();
    } else {
        emit currentEntryFinished();
    }
}

void ComparisonBackendMode::checkAnswer()
{
    QStringList answers = m_frontend->userInput().toStringList();

    bool absoluteCorrect = answers.at(0) == m_current->entry()->translation(Prefs::solutionLanguage())->text();
    bool comparativeCorrect = answers.at(1) == m_current->entry()->translation(Prefs::solutionLanguage())->comparative();
    bool superlativeCorrect = answers.at(2) == m_current->entry()->translation(Prefs::solutionLanguage())->superlative();

    // FIXME grades
    //m_current->entry()->translation(Prefs::solutionLanguage())->incGrade();
    //m_current->entry()->translation(Prefs::solutionLanguage())->incPracticeCount();
    //m_current->entry()->translation(Prefs::solutionLanguage())->comparison(m_currentTense).comparison(key).setPracticeDate( QDateTime::currentDateTime() );

    if (absoluteCorrect && comparativeCorrect && superlativeCorrect) {
        m_frontend->setFeedbackState(Practice::AbstractFrontend::AnswerCorrect);
        m_frontend->setResultState(Practice::AbstractFrontend::AnswerCorrect);
        m_frontend->setFeedback(i18n("All comparison forms were right."));
    } else {
        m_frontend->setFeedbackState(Practice::AbstractFrontend::AnswerWrong);
        m_frontend->setResultState(Practice::AbstractFrontend::AnswerWrong);

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
    }
}

void ComparisonBackendMode::hintAction()
{
    // FIXME
}

#include "comparisonbackendmode.moc"
