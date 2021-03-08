/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "conjugationbackendmode.h"

#include <KLocalizedString>

#include <KEduVocDocument>

#include "conjugationdata.h"
#include "documentsettings.h"

using namespace Practice;

ConjugationBackendMode::ConjugationBackendMode(AbstractFrontend* frontend, QObject* parent,
                                               Practice::SessionManagerBase* sessionManager,
                                               KEduVocDocument* doc)
    : AbstractBackendMode(frontend, parent)
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

    if (!m_current->entry()->translation(m_current->languageTo())->conjugationTenses().contains(m_currentTense)) {
        qDebug() << "invalid tense for entry - " << m_currentTense;
        qDebug() << "tenses: "  << m_current->entry()->translation(m_current->languageTo())->conjugationTenses();
    }

    data.tense = m_currentTense;
    m_conjugation = m_current->entry()->translation(m_current->languageTo())->getConjugation(m_currentTense);
    m_pronounFlags = current->conjugationPronouns();

    data.questionInfinitive = m_current->entry()->translation(m_current->languageFrom())->text();
    data.solutionInfinitive = m_current->entry()->translation(m_current->languageTo())->text();

    data.personalPronouns = validPersonalPronouns();

    m_frontend->setQuestion(QVariant::fromValue<ConjugationData>(data));
    QStringList answers;
    foreach(const KEduVocWordFlags & key, m_pronounFlags) {
        answers.append(m_conjugation.conjugation(key).text());
    }
    m_frontend->setSolution(answers);

    m_frontend->setQuestionSound(m_current->entry()->translation(m_current->languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_current->languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_current->languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_current->languageTo())->pronunciation());
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
    return true;
}

QStringList ConjugationBackendMode::validPersonalPronouns()
{
    QStringList pp;
    foreach(const KEduVocWordFlags & person, m_pronounFlags) {
        // FIXME: Used to be m_practiceOptions.languageTo()
        pp.append(m_doc->identifier(Prefs::learningLanguage()).personalPronouns().personalPronoun(person));
    }
    qDebug() << "PP: " << pp.size() << pp;
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
            qDebug() << "dec grade for " << m_conjugation.conjugation(key).text();
            allCorrect = false;
        }
        ++i;
    }

    qDebug() << "answers: " << answers;

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

grade_t ConjugationBackendMode::currentPreGradeForEntry() const
{
    grade_t min_preGrade = KV_MAX_GRADE;
    foreach(KEduVocWordFlags key, m_pronounFlags) {
        min_preGrade = qMin(m_conjugation.conjugation(key).preGrade(), min_preGrade);
    }

    return min_preGrade;
}

grade_t ConjugationBackendMode::currentGradeForEntry() const
{
    grade_t min_grade = KV_MAX_GRADE;
    foreach(KEduVocWordFlags key, m_pronounFlags) {
        min_grade = qMin(m_conjugation.conjugation(key).grade(), min_grade);
    }

    return min_grade;
}

void ConjugationBackendMode::updateGrades()
{
    qDebug() << "Grading conjugations";

    foreach(const KEduVocWordFlags & key, m_pronounFlags) {
        KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageTo());
        if (translation) {
            KEduVocConjugation conjugationToUpdate = translation->getConjugation(m_currentTense);
            conjugationToUpdate.conjugation(key).incPracticeCount();
            conjugationToUpdate.conjugation(key).setPracticeDate(QDateTime::currentDateTime());

            updateGrade(conjugationToUpdate.conjugation(key),
                        m_frontend->resultState() == AbstractFrontend::AnswerCorrect,
                        m_current->statisticBadCount() == 0);

            translation->setConjugation(m_currentTense, conjugationToUpdate);
        }
    }
}

void ConjugationBackendMode::hintAction()
{
    // FIXME
}
