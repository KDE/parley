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

#include "documentsettings.h"

using namespace Practice;

ComparisonBackendMode::ComparisonBackendMode(const PracticeOptions& practiceOptions,
        AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc)
: AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_doc(doc)
{
}

bool ComparisonBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    m_lastAnswers.clear();

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
    return true;
}

void ComparisonBackendMode::checkAnswer()
{
    QStringList answers = m_frontend->userInput().toStringList();

    if (answers == m_lastAnswers) {
        emit answerWrongShowSolution();
        return;
    }

    bool absoluteCorrect = answers.at(0) == m_current->entry()->translation(Prefs::solutionLanguage())->text();
    bool comparativeCorrect = answers.at(1) == m_current->entry()->translation(Prefs::solutionLanguage())->comparative();
    bool superlativeCorrect = answers.at(2) == m_current->entry()->translation(Prefs::solutionLanguage())->superlative();

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

    bool absoluteCorrect = answers.at(0) == m_current->entry()->translation(Prefs::solutionLanguage())->text();
    bool comparativeCorrect = answers.at(1) == m_current->entry()->translation(Prefs::solutionLanguage())->comparative();
    bool superlativeCorrect = answers.at(2) == m_current->entry()->translation(Prefs::solutionLanguage())->superlative();

    // TODO way too much duplicated code here

    KEduVocTranslation* translation = m_current->entry()->translation(m_practiceOptions.languageTo());

    translation->incPracticeCount();
    translation->setPracticeDate( QDateTime::currentDateTime() );

    if (absoluteCorrect) {
        if (m_current->statisticBadCount() == 0) {
            translation->incGrade();
        }
    } else {
        translation->setGrade(KV_LEV1_GRADE);
        translation->incBadCount();
    }

    KEduVocText comp = translation->comparativeForm();

    comp.incPracticeCount();
    comp.setPracticeDate( QDateTime::currentDateTime() );

    if (comparativeCorrect) {
        if (m_current->statisticBadCount() == 0) {
            comp.incGrade();
        }
    } else {
        comp.setGrade(KV_LEV1_GRADE);
        comp.incBadCount();
    }
    translation->setComparativeForm(comp);

    KEduVocText super = translation->superlativeForm();

    super.incPracticeCount();
    super.setPracticeDate( QDateTime::currentDateTime() );

    if (superlativeCorrect) {
        if (m_current->statisticBadCount() == 0) {
            super.incGrade();
        }
    } else {
        super.setGrade(KV_LEV1_GRADE);
        super.incBadCount();
    }
    translation->setSuperlativeForm(super);
}



#include "comparisonbackendmode.moc"
