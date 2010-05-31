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

#include <KLocalizedString>

#include <keduvocdocument.h>

#include "conjugationdata.h"
#include "documentsettings.h"

using namespace Practice;

ConjugationBackendMode::ConjugationBackendMode(const PracticeOptions& practiceOptions,
        AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc)
: AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_doc(doc)
{
    DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_practiceOptions.languageTo()));
    documentSettings.readConfig();
    m_tenses = documentSettings.conjugationTenses();
}

bool ConjugationBackendMode::setTestEntry(TestEntry* current)
{
    ConjugationData data;
    m_current = current;
    m_lastAnswers.clear();

    // FIXME tense selection
    QStringList possibleTenses;
    foreach(const QString& practiceTense, m_tenses) {
        if (m_current->entry()->translation(m_practiceOptions.languageTo())
            ->conjugationTenses().contains(practiceTense)) {
            possibleTenses.append(practiceTense);
        }
    }
    if (possibleTenses.isEmpty()) {
        kDebug() << "No valid practice tenses in entry: " << m_current->entry()->translation(m_practiceOptions.languageTo())->text()
            << m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses()
            << m_tenses;
        return false;
    }

    m_currentTense = possibleTenses.first();

    if (!m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses().contains(m_currentTense)) {
        kDebug() << "invalid tense for entry - " << m_currentTense;
        kDebug() << "tenses: "  << m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses();
    }

    data.tense = m_currentTense;
    m_conjugation = m_current->entry()->translation(m_practiceOptions.languageTo())->conjugation(m_currentTense);
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
    return true;
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

void ConjugationBackendMode::checkAnswer()
{
    QStringList answers = m_frontend->userInput().toStringList();

    bool allCorrect = true;
    int numRight = 0;
    int i=0;
    foreach(const KEduVocWordFlags& key, m_pronounFlags) {
        if (answers.at(i) == m_conjugation.conjugation(key).text()) {
            ++numRight;
        } else {
            kDebug() << "dec grade for " << m_conjugation.conjugation(key).text();
            allCorrect = false;
        }
        ++i;
    }

    kDebug() << "answers: " << answers;

    if (allCorrect) {
        m_frontend->setFeedback(i18n("All conjugation forms were right."));
        emit answerRight();
    } else {
        m_frontend->setFeedback(i18ncp("You did not get the conjugation forms right.", "You answered %1 conjugation form correctly.", "You answered %1 conjugation forms correctly.", numRight));

        if (answers == m_lastAnswers) {
            emit answerWrongShowSolution();
        } else {
            emit answerWrongRetry();
        }
        m_lastAnswers = answers;
    }
}

void ConjugationBackendMode::updateGrades()
{
    kDebug() << "Grading conjugations";
        QStringList answers = m_frontend->userInput().toStringList();

    bool allCorrect = true;
    int numRight = 0;
    int i=0;
    foreach(const KEduVocWordFlags& key, m_pronounFlags) {
        if (answers.at(i) == m_conjugation.conjugation(key).text()) {
            m_current->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation(key).incGrade();
            m_current->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation(key).incPracticeCount();
            ++numRight;
        } else {
            m_current->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation(key).setGrade(1);
            m_current->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation(key).incPracticeCount();
            kDebug() << "dec grade for " << m_conjugation.conjugation(key).text();
            allCorrect = false;
        }
        m_current->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation(key).setPracticeDate( QDateTime::currentDateTime() );
        ++i;
    }

}

void ConjugationBackendMode::hintAction()
{
    // FIXME
}

#include "conjugationbackendmode.moc"
