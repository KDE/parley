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


#include "writtenbackendmode.h"

#include <KLocalizedString>

using namespace Practice;

WrittenBackendMode::WrittenBackendMode(const Practice::PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent, TestEntryManager* testEntryManager, KEduVocDocument* doc)
:AbstractBackendMode(practiceOptions, frontend, parent)
,m_testEntryManager(testEntryManager)
,m_doc(doc)
{
    m_validator = new WrittenPracticeValidator(m_practiceOptions.languageTo(),doc);
}

bool WrittenBackendMode::setTestEntry(TestEntry* current)
{
    AbstractBackendMode::setTestEntry(current);
    m_frontend->showQuestion();
    m_lastAnswer.clear();
    m_synonyms.clear();
    m_validator->setEntry(current);
    return true;
}

void WrittenBackendMode::checkAnswer()
{
    kDebug() << "check";
    QString answer = m_frontend->userInput().toString();

    // if the state is correct, this must be the first attempt
    bool isFirstAttempt = m_lastAnswer.isEmpty();

    // move on, the user has not changed anything or pressed enter with no answer
    bool answerUnchanged = (answer == m_lastAnswer) || answer.isEmpty();
    m_lastAnswer = answer;

    m_validator->validateAnswer(answer);

    bool isCorrect = m_current->lastErrors() & TestEntry::Correct;
    bool isSynonym = m_current->lastErrors() & TestEntry::Synonym;

    QString feedbackString = getFeedbackString(isFirstAttempt, m_current->lastErrors());
    m_frontend->setFeedback(feedbackString);

    if (isCorrect) {
        m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
        if (isFirstAttempt) {
            m_frontend->setResultState(AbstractFrontend::AnswerCorrect);
        } else {
            m_frontend->setResultState(AbstractFrontend::AnswerWrong);
        }
        emit answerRight();
    } else {
        m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
        m_current->addUserAnswer(answer);

        if(answerUnchanged) {
            //User gave an empty answer or the same answer for a second time so we want to drop out.
            m_frontend->setResultState(AbstractFrontend::AnswerWrong);
            emit answerWrongShowSolution();
        } else {
            emit answerWrongRetry();
        }
    }

    if (isSynonym) {
        m_frontend->setSynonym(answer);
        m_frontend->showSynonym();

        // FIXME that function was nonsense: markSynonymCorrect(answer);
        m_frontend->setResultState(AbstractFrontend::AnswerSynonym);
        if (!Prefs::countSynonymsAsCorrect()) {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
        }
        addSynonym(answer);
    }
}

QString WrittenBackendMode::getFeedbackString(bool isFirstAttempt, TestEntry::ErrorTypes error)
{
    // The user entered a synonym
    if (error & TestEntry::Synonym) {
        if (!Prefs::countSynonymsAsCorrect()) {
            return i18n("Your answer was wrong as synonyms are not accepted. Please try again.");
        }
        const QString answer = m_validator->getCorrectedAnswer();
        if (m_synonyms.contains(answer)) {
            return i18n("Your answer was an already entered synonym.");
        } else {
            if (error & TestEntry::CapitalizationMistake){
                return i18n("Your answer was a synonym and your capitalization was wrong.");
            } else if (error & TestEntry::AccentMistake){
                return i18n("Your answer was a synonym and accents were wrong.");
            } else {
                return i18n("Your answer was a synonym.");
            }
        }
    }

    // The answer was wrong
    if (error & TestEntry::Wrong) {
        if ((error & TestEntry::CapitalizationMistake) && !Prefs::ignoreCapitalizationMistakes()) {
            return i18n("Your answer was wrong as capitalization mistakes are not accepted. Please try again.");
        } else if ((error & TestEntry::AccentMistake) && !Prefs::ignoreAccentMistakes()){
            return i18n("Your answer was wrong as accent mistakes are not accepted. Please try again.");
        } else {
            return i18n("Your answer was wrong. Please try again.");
        }
    }

    // The answer was right
    if (isFirstAttempt) {
        if ((error & TestEntry::CapitalizationMistake)){
            return i18n("Your answer was right on the first attempt, but your capitalization was wrong.");
        } else if ((error & TestEntry::AccentMistake)){
            return i18n("Your answer was right on the first attempt, but accents were wrong.");
        } else {
            return i18n("Your answer was right on the first attempt.");
        }
    } else {
        if ((error & TestEntry::CapitalizationMistake)){
            return i18n("Your answer was right... but not on the first try and your capitalization was wrong.");
        } else if ((error & TestEntry::AccentMistake)){
            return i18n("Your answer was right... but not on the first try and accents were wrong.");
        } else {
            return i18n("Your answer was right... but not on the first try.");
        }
    }
}

void WrittenBackendMode::hintAction()
{
    QString solution = m_current->entry()->translation(m_practiceOptions.languageTo())->text();
    m_currentHint = solution.left(m_currentHint.size() + 1);
    if (m_currentHint.size() == solution.size()) {
        // show solution
        m_frontend->setFeedback(i18n("You revealed the answer by using too many hints."));
        m_frontend->setResultState(AbstractFrontend::AnswerWrong);
        if (m_frontend->userInput().toString() == m_current->entry()->translation(m_practiceOptions.languageTo())->text()) {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
        } else {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
        }
        m_frontend->showSolution();
       // m_state = SolutionShown;
    } else {
        m_frontend->setHint(i18n("The solution starts with: %1", m_currentHint));
    }

    m_lastAnswer = m_frontend->userInput().toString();
}

#include "writtenbackendmode.moc"
