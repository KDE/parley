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


#include "conjugationbackendmode.h"
#include "defaultbackend.h"

#include <klocale.h>
#include "conjugationdata.h"

using namespace Practice;

ConjugationBackendMode::ConjugationBackendMode(const PracticeOptions& practiceOptions,
        AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc)
: AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_doc(doc)
{
}

void ConjugationBackendMode::setTestEntry(TestEntry* current)
{
    ConjugationData data;
    m_current = current;

    // FIXME tense selection
    m_tense = m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses().first();
    kDebug() << "tenses: "  << m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses() << "using tense: " << m_tense;
    data.tense = m_tense;
    m_conjugation = m_current->entry()->translation(m_practiceOptions.languageTo())->conjugation(m_tense);
    updatePronounFlags();

    data.questionInfinitive = m_current->entry()->translation(m_practiceOptions.languageFrom())->text();
    data.solutionInfinitive = m_current->entry()->translation(m_practiceOptions.languageTo())->text();

    data.personalPronouns = validPersonalPronouns();

    m_frontend->setQuestion(qVariantFromValue<ConjugationData>(data));
    QStringList answers;
    foreach (const KEduVocWordFlags& key, m_pronounFlags) {
        answers.append(m_conjugation.conjugation(key).text());
    }
    m_frontend->setSolution(answers);

    m_frontend->setQuestionSound(m_current->entry()->translation(m_practiceOptions.languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_practiceOptions.languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_practiceOptions.languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_practiceOptions.languageTo())->pronunciation());
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
}

void ConjugationBackendMode::updatePronounFlags()
{
    m_pronounFlags.clear();
    for (int num = KEduVocWordFlag::Singular; num <= KEduVocWordFlag::Plural; num += KEduVocWordFlag::Singular) {
        for (int pers = KEduVocWordFlag::First; pers <= KEduVocWordFlag::Third; pers += KEduVocWordFlag::First) {
            if (m_conjugation.keys().contains(KEduVocWordFlags(num|pers))) {
                kDebug() << "no gender " << num << pers;
                m_pronounFlags.append(KEduVocWordFlags(num|pers));
            }
            for (int gender = KEduVocWordFlag::Masculine; gender <= KEduVocWordFlag::Neuter; gender += KEduVocWordFlag::Masculine) {
                if (m_conjugation.keys().contains(KEduVocWordFlags(num|pers|gender))) {
                    kDebug() << "gender " << num << pers << gender;
                    m_pronounFlags.append(KEduVocWordFlags(num|pers|gender));
                }
            }
        }
    }
}

QStringList ConjugationBackendMode::validPersonalPronouns()
{
    QStringList pp;
    foreach (const KEduVocWordFlags& person, m_pronounFlags) {
        pp.append(m_doc->identifier(m_practiceOptions.languageTo()).personalPronouns().personalPronoun(person));
    }
    kDebug() << "PP: " << pp.size() << pp;
    return pp;
}

void ConjugationBackendMode::continueAction()
{
    if (m_frontend->resultState() == AbstractFrontend::QuestionState) {
        checkAnswer();
        m_frontend->showSolution();
    } else {
        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            emit currentEntryFinished();
        }
        emit nextEntry();
    }
}

void ConjugationBackendMode::checkAnswer()
{
    QStringList answers = m_frontend->userInput().toStringList();

    bool allCorrect = true;
    
    int i=0;
    foreach(const KEduVocWordFlags& key, m_pronounFlags) {
        if (answers.at(i) == m_conjugation.conjugation(key).text()) {
            m_conjugation.conjugation(key).incGrade();
            m_conjugation.conjugation(key).incPracticeCount();
        } else {
            m_conjugation.conjugation(key).incBadCount();
            m_conjugation.conjugation(key).incPracticeCount();
            allCorrect = false;
        }
        ++i;
    }

    kDebug() << "answers: " << answers;

    if (allCorrect) {
        m_frontend->setFeedbackState(Practice::AbstractFrontend::AnswerCorrect);
        m_frontend->setResultState(Practice::AbstractFrontend::AnswerCorrect);
    } else {
        m_frontend->setFeedbackState(Practice::AbstractFrontend::AnswerWrong);
        m_frontend->setResultState(Practice::AbstractFrontend::AnswerWrong);
    }
}

void ConjugationBackendMode::hintAction()
{
    // FIXME
}

#include "conjugationbackendmode.moc"
