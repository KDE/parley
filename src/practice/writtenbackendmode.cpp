/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "writtenbackendmode.h"

#include <KLocalizedString>

using namespace Practice;

WrittenBackendMode::WrittenBackendMode(AbstractFrontend* frontend, QObject* parent,
                                       SessionManagerBase* sessionManager, KEduVocDocument* doc)
    : AbstractBackendMode(frontend, parent)
    , m_sessionManager(sessionManager)
    // FIXME: Used to be m_practiceOptions.languageTo()
    , m_validator(new WrittenPracticeValidator(Prefs::learningLanguage(), doc))
    , m_doc(doc)
{

}

bool WrittenBackendMode::setTestEntry(TestEntry* current)
{
    AbstractBackendMode::setTestEntry(current);
    m_firstAttempt = true;
    m_frontend->showQuestion();
    m_lastAnswer.clear();
    m_synonyms.clear();
    m_currentHint.clear();
    m_validator->setEntry(current);
    return true;
}

void WrittenBackendMode::checkAnswer()
{
    qDebug() << "check";
    QString answer = m_frontend->userInput().toString();

    // move on, the user has not changed anything or pressed enter with no answer
    bool answerUnchanged = (answer == m_lastAnswer) || answer.isEmpty();
    m_lastAnswer = answer;

    m_validator->validateAnswer(answer);

    bool isCorrect = m_current->lastErrors() & TestEntry::Correct;
    bool isSynonym = m_current->lastErrors() & TestEntry::Synonym;

    QString feedbackString = getFeedbackString(m_current->lastErrors());
    m_frontend->setFeedback(feedbackString);

    m_firstAttempt = m_firstAttempt && isSynonym; // don't count the answer as wrong if you only enter valid synonyms

    // first handle synonyms as they may be correct or not
    if (isSynonym) {
        addSynonym(answer);
        m_frontend->setSynonym(answer);
        m_frontend->showSynonym();

        if (Prefs::countSynonymsAsCorrect()) {
            // any of the synonyms is automatically counted as right
            // otherwise do nothing to give the user another chance
            emit answerRight();
        }
        return;
    }

    if (isCorrect) {
        emit answerRight();
    } else {
        if (answerUnchanged) {
            m_frontend->setFeedback(i18n("Your answer was wrong."));
            emit answerWrongShowSolution();
        } else {
            m_current->addUserAnswer(answer);
            emit answerWrongRetry();
        }
    }
}

QString WrittenBackendMode::getFeedbackString(TestEntry::ErrorTypes error)
{
    // The user entered a synonym
    if (error & TestEntry::Synonym) {
        if (!Prefs::countSynonymsAsCorrect()) {
            return i18n("Your answer was a synonym. Please enter another word with the same translation.");
        }
        const QString answer = m_validator->getCorrectedAnswer();
        if (m_synonyms.contains(answer)) {
            return i18n("Your answer was an already entered synonym.");
        } else {
            if (error & TestEntry::PunctuationMistake) {
                return i18n("Your answer was a synonym and your punctuation was wrong.");
            }else if (error & TestEntry::CapitalizationMistake) {
                return i18n("Your answer was a synonym and your capitalization was wrong.");
            } else if (error & TestEntry::AccentMistake) {
                return i18n("Your answer was a synonym and accents were wrong.");
            } else {
                return i18n("Your answer was a synonym.");
            }
        }
    }

    // The answer was wrong
    if (error & TestEntry::Wrong) {
        if ((error & TestEntry::PunctuationMistake) && !Prefs::ignorePunctuationMistakes()) {
            return i18n("Your answer was wrong as punctuation mistakes are not accepted. Please try again.");
        } else if ((error & TestEntry::CapitalizationMistake) && !Prefs::ignoreCapitalizationMistakes()) {
            return i18n("Your answer was wrong as capitalization mistakes are not accepted. Please try again.");
        } else if ((error & TestEntry::AccentMistake) && !Prefs::ignoreAccentMistakes()) {
            return i18n("Your answer was wrong as accent mistakes are not accepted. Please try again.");
        } else {
            return i18n("Your answer was wrong. Please try again.");
        }
    }

    // The answer was right
    if (m_firstAttempt) {
        if ((error & TestEntry::PunctuationMistake)) {
            return i18n("Your answer was right, but your punctuation was wrong.");
        }else if ((error & TestEntry::CapitalizationMistake)) {
            return i18n("Your answer was right, but your capitalization was wrong.");
        } else if ((error & TestEntry::AccentMistake)) {
            return i18n("Your answer was right, but accents were wrong.");
        } else {
            return i18n("Your answer was right.");
        }
    } else {
        if ((error & TestEntry::PunctuationMistake)) {
            return i18n("Your answer was right... but not on the first try and your punctuation was wrong.");
        }else if ((error & TestEntry::CapitalizationMistake)) {
            return i18n("Your answer was right... but not on the first try and your capitalization was wrong.");
        } else if ((error & TestEntry::AccentMistake)) {
            return i18n("Your answer was right... but not on the first try and accents were wrong.");
        } else {
            return i18n("Your answer was right... but not on the first try.");
        }
    }
}

void WrittenBackendMode::hintAction()
{
    QString solution = m_current->entry()->translation(m_current->languageTo())->text();
    m_currentHint = solution.left(m_currentHint.size() + 1);
    if (m_currentHint.size() == solution.size()) {
        // show solution
        m_frontend->setFeedback(i18n("You revealed the answer by using too many hints."));
        m_frontend->setResultState(AbstractFrontend::AnswerWrong);
        if (m_frontend->userInput().toString() == m_current->entry()->translation(m_current->languageTo())->text()) {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerCorrect);
        } else {
            m_frontend->setFeedbackState(AbstractFrontend::AnswerWrong);
        }
        m_frontend->showSolution();
    } else {
        m_frontend->setHint(i18n("The solution starts with: %1", m_currentHint));
    }

    m_lastAnswer = m_frontend->userInput().toString();
}
