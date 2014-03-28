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
                                               AbstractFrontend* frontend, QObject* parent,
                                               Practice::SessionManagerBase* sessionManager,
                                               KEduVocDocument* doc)
    : AbstractBackendMode(practiceOptions, frontend, parent)
    , m_sessionManager(sessionManager)
    , m_doc(doc)
{
}

bool ConjugationBackendMode::setTestEntry(TestEntry* current)
{
    ConjugationData data;
    m_current = current;
    m_lastAnswers.clear();

    m_currentTense = m_current->conjugationTense();

    if (!m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses().contains(m_currentTense)) {
        kDebug() << "invalid tense for entry - " << m_currentTense;
        kDebug() << "tenses: "  << m_current->entry()->translation(m_practiceOptions.languageTo())->conjugationTenses();
    }

    data.tense = m_currentTense;
    m_conjugation = m_current->entry()->translation(m_practiceOptions.languageTo())->conjugation(m_currentTense);
    m_pronounFlags = current->conjugationPronouns();

    data.questionInfinitive = m_current->entry()->translation(m_practiceOptions.languageFrom())->text();
    data.solutionInfinitive = m_current->entry()->translation(m_practiceOptions.languageTo())->text();

    data.personalPronouns = validPersonalPronouns();

    m_frontend->setQuestion(qVariantFromValue<ConjugationData>(data));
    QStringList answers;
    foreach(const KEduVocWordFlags & key, m_pronounFlags) {
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

QStringList ConjugationBackendMode::validPersonalPronouns()
{
    QStringList pp;
    foreach(const KEduVocWordFlags & person, m_pronounFlags) {
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
    int i = 0;
    foreach(const KEduVocWordFlags & key, m_pronounFlags) {
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

grade_t ConjugationBackendMode::currentGradeForEntry()
{
    Q_ASSERT(m_current != 0);
    KEduVocTranslation* trans = m_current->entry()->translation(m_practiceOptions.languageTo());
    KEduVocConjugation& conj = trans->conjugation(m_current->conjugationTense());
    QList<KEduVocWordFlags> keys = conj.keys();

    grade_t min_grade = KV_MAX_GRADE;
    foreach(KEduVocWordFlags key, keys) {
        min_grade = qMin(conj.conjugation(key).grade(), min_grade);
    }

    return min_grade;
}

void ConjugationBackendMode::updateGrades()
{
    kDebug() << "Grading conjugations";

    foreach(const KEduVocWordFlags & key, m_pronounFlags) {
        KEduVocText& text = m_current->entry()->translation(m_current->languageTo())->
                            conjugation(m_currentTense).conjugation(key);

        text.incPracticeCount();
        text.setPracticeDate(QDateTime::currentDateTime());

        if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
            if (m_current->statisticBadCount() == 0) {
                text.incGrade();
            }
        } else {
            text.setGrade(KV_LEV1_GRADE);
            text.incBadCount();
        }
    }
}

void ConjugationBackendMode::hintAction()
{
    // FIXME
}

#include "conjugationbackendmode.moc"
