/*
    SPDX-FileCopyrightText: 2010 Benjamin Schleinzer <ben-kde@schleinzer.eu>
    SPDX-FileCopyrightText: 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "writtenpracticevalidator.h"
#include "prefs.h"

#include <KEduVocTranslation>
#include <QDebug>

/// temporary namespace for string manipulation functions
/// could move into KStringHandler eventually
namespace ParleyStringHandlerOld
{
QString stripAccents(const QString& original)
{
    QString noAccents;
    QString decomposed = original.normalized(QString::NormalizationForm_D);
    for (int i = 0; i < decomposed.length(); ++i) {
        if (decomposed[i].category() != 1) {
            noAccents.append(decomposed[i]);
        }
    }
    qDebug() << original << " without accents: " << noAccents;
    return noAccents;
}
}

using namespace Practice;

WrittenPracticeValidator::WrittenPracticeValidator(int translation, KEduVocDocument* doc)
    : m_entry(0),
      m_doc(doc),
      m_error(0),
      m_speller(0),
      m_spellerAvailable(false)
{
    setLanguage(translation);
}

WrittenPracticeValidator::~WrittenPracticeValidator()
{
    delete m_speller;
}

void WrittenPracticeValidator::setEntry(TestEntry* entry)
{
    m_entry = entry;
}

void WrittenPracticeValidator::setLanguage(int translation)
{
    m_translation = translation;

    // default: try locale
    if (!m_speller) {
        m_speller = new Sonnet::Speller(m_doc->identifier(translation).locale());
    } else {
        m_speller->setLanguage(m_doc->identifier(translation).locale());
    }

    // we might succeed with language name instead.
    if (!m_speller->isValid()) {
        m_speller->setLanguage(m_doc->identifier(translation).name());
    }

    if (!m_speller->isValid()) {
        qDebug() << "No spellchecker for current language found: " << m_doc->identifier(m_translation).locale();
        qDebug() << "Available dictionaries: " << m_speller->availableLanguages()
                 << "\n names: " << m_speller->availableLanguageNames()
                 << "\n backends: " << m_speller->availableBackends();
        m_spellerAvailable = false;
    } else {
        m_spellerAvailable = true;
    }

}

bool WrittenPracticeValidator::spellcheckerAvailable()
{
    return m_spellerAvailable;
}

void WrittenPracticeValidator::validateAnswer(const QString& answer)
{
    if (m_entry == 0) {
        qCritical() << "No entry set, cannot verify answer.";
        return;
    }

    QString correct = m_entry->entry()->translation(m_entry->languageTo())->text();

    qDebug() << "Correct answer should be: " << correct;
    m_error = {};

    //Check for empty answers and valid answers first
    if (answer.isEmpty()) {
        m_error |= TestEntry::Wrong;
        qDebug() << "Empty answer ";
    } else if (isCorrect(correct, answer)) {
        m_error |= TestEntry::Correct;
    } else {
        //Check for all valid errors to build a list of
        //possible mistakes. This provides us with useful information
        //that we can use to give feedback to the user.
        if (isPunctuationMistake(correct, answer)) {
            m_error |= TestEntry::Correct;
        } else if (isCapitalizationMistake(correct, answer)) {
            m_error |= TestEntry::Correct;
        } else if (isAccentMistake(correct, answer)) {
            m_error |= TestEntry::Correct;
        } else if (isSynonymMistake(answer)) {
            m_error |= TestEntry::Correct;
        } else {
            m_error |= TestEntry::Wrong;
            qDebug() << "Wrong answer: " << answer;
        }
    }
    qDebug() << "Error code " << m_error;

    m_entry->setLastErrors(m_error);
}

QString WrittenPracticeValidator::getCorrectedAnswer()
{
    return m_correctedAnswer;
}

bool WrittenPracticeValidator::isCorrect(const QString& correct, const QString& answer)
{
    if (answer == correct ) {
        qDebug() << "Correct answer was given";
        return true;
    }
    return false;
}

bool WrittenPracticeValidator::isSynonymMistake(const QString& answer)
{
    foreach(KEduVocTranslation * synonym, m_entry->entry()->translation(m_entry->languageTo())->synonyms()) {
        if (synonym->text() == answer ||
                (Prefs::ignoreCapitalizationMistakes() && isCapitalizationMistake(synonym->text(), answer)) ||
                (Prefs::ignoreAccentMistakes() && isAccentMistake(synonym->text(), answer)) ||
                (Prefs::ignorePunctuationMistakes() && isPunctuationMistake(synonym->text(), answer))) {
            qDebug() << "Synonym entered: " << synonym->text() << " answer: " << answer;
            m_correctedAnswer = synonym->text();
            m_error |= TestEntry::Synonym;
            //only return true if accept these kinds of mistakes
            //otherwise just set the error flag
            if (Prefs::countSynonymsAsCorrect()) {
                return true;
            }
        }
    }
    return false;
}

bool WrittenPracticeValidator::isCapitalizationMistake(const QString& original, const QString& answer)
{
    if (answer.toLower() == original.toLower() ||
            (Prefs::ignorePunctuationMistakes() && isPunctuationMistake(original.toLower(),answer.toLower()))) {
        qDebug() << "CapitalizationMistake: " << original << " answer: " << answer;
        m_error |= TestEntry::CapitalizationMistake;
        m_correctedAnswer = answer;
        //only return true if accept these kinds of mistakes
        //otherwise just set the error flag
        if (Prefs::ignoreCapitalizationMistakes())
            return true;
    }
    return false;
}

bool WrittenPracticeValidator::isPunctuationMistake(const QString& original, const QString& answer)
{
    QString ans = answer;
    QString orig = original;
    if (ans.remove(QRegExp(QStringLiteral("[^a-zA-ZƒŠŒŽšœžŸÀ-ÿ\\s]"))) == orig.remove(QRegExp(QStringLiteral("[^a-zA-ZƒŠŒŽšœžŸÀ-ÿ\\s]")))) {
        qDebug() << "PunctuationMistake: " << original << " answer: " << answer;
        m_error |= TestEntry::PunctuationMistake;
        m_correctedAnswer = answer;
        //only return true if accept these kinds of mistakes
        //otherwise just set the error flag
        if (Prefs::ignorePunctuationMistakes())
            return true;
    }
    return false;
}

bool WrittenPracticeValidator::isAccentMistake(const QString& original, const QString& answer)
{
    QString stripedOriginal = ParleyStringHandlerOld::stripAccents(original);
    QString stripedAnswer = ParleyStringHandlerOld::stripAccents(answer);
    if (stripedOriginal == stripedAnswer ||
            (Prefs::ignoreCapitalizationMistakes() && isCapitalizationMistake(stripedOriginal, stripedAnswer)) ||
            (Prefs::ignorePunctuationMistakes() && isPunctuationMistake(stripedOriginal, stripedAnswer))) {
        qDebug() << "AccentMistake: " << original << " answer: " << answer;
        m_error |= TestEntry::AccentMistake;
        m_correctedAnswer = answer;
        //only return true if accept these kinds of mistakes
        //otherwise just set the error flag
        if (Prefs::ignoreAccentMistakes()) {
            return true;
        }
    }
    return false;
}
