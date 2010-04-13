/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ANSWERVALIDATOROLD_H
#define ANSWERVALIDATOROLD_H

#include "testentry.h"

#include <QtCore/QString>
#include <QtCore/QPair>

class KEduVocDocument;
namespace Sonnet {
    class Speller;
}

/**
Provides different comparison methods to determine the quality of the answer.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class AnswerValidatorOld{
public:
    AnswerValidatorOld(KEduVocDocument* doc);

    ~AnswerValidatorOld();

    bool spellcheckerAvailable();

    /**
     * Sets the current entry. This has to be set before trying to validate! It will store the results.
     * @param expression
     */
    void setTestEntry(TestEntry* entry);

    /**
     * Set the language for the spell checker. (Identifierindex)
     * @param translation 
     */
    void setLanguage(int translation);

    /**
     * Checks the user answer. If the answer was correct, 1.0 is set as lastPercentage for the TestEntry,
     * 0.0 means no relation between answer and solution whatsoever.
     * The type of mistake is also set in the TestEntry.
     * @param userAnswer
     */
    void checkUserAnswer(const QString& userAnswer);
    void checkUserAnswer(const QString& solution, const QString& userAnswer, const QString& language = QString());

private:
    /**
     * Default correction mechanism.
     */
    void defaultCorrector();

    /**
     * Old primitive working corrector.
     */
    void simpleCorrector();

    /**
     * Levenshtein Distance is the minimum number of
     * substitution, insertion and deletion operations necessary
     * to transform two strings into each other.
     * @param s
     * @param t
     * @return the Levenshtein Distance measure
     */
    int levenshteinDistance(const QString& s, const QString& t);

    bool spellcheckerMisspelled(const QString& s);

    bool spellcheckerInSuggestionList(const QString& solution, const QString& userAnswer);

    /**
     * Evaluate the closeness of two words. This is probably the most common case.
     * Should be used for single words. Will not work good for more complex cases.
     * @param word1
     * @param word2
     * @param errorType
     */
    void wordCompare(const QString& solution, const QString& userWord, double& grade, TestEntry::ErrorTypes& errorTypes);

    /**
     * Split up a sentence and try to work magic with it...
     */
    void sentenceAnalysis();

    /**
     * Try to make words into pairs that are most likely to match
     * @param
     * @param
     * @return
     */
    QList< QPair < QString, QString > > bestPairs(const QStringList& solutionWords , const QStringList& userAnswerWords );

    /// document for language and articles
    KEduVocDocument* m_doc;

    /// the entry to be checked, this provides meta info like word type etc
    TestEntry* m_entry;
    int m_translation;

    /// the solution - usually text of the translation
    QString m_solution;

    /// the answer provided by the user
    QString m_userAnswer;

    /// field used by the LevenshteinDistance to store temp data
    QByteArray m_d;

    /// spellchecker
    Sonnet::Speller* m_speller;
    bool m_spellerAvailable;

    static const double LEVENSHTEIN_THRESHOLD;
    static const double UNRELATED_WORD_GRADE;
    static const double FALSE_FRIEND_GRADE;
    static const double SPELLING_MISTAKE_PER_LETTER_PUNISHMENT;
    static const double ACCENT_MISTAKE_PUNISHMENT;
    static const double CAPITALIZATION_MISTAKE_PUNISHMENT;
    static const double WRONG_ARTICLE_PUNISHMENT;
};

#endif
