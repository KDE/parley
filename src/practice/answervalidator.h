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
#ifndef ANSWERVALIDATOR_H
#define ANSWERVALIDATOR_H

#include <QtCore/QString>
#include <QtCore/QPair>

class TestEntry;
class KEduVocDocument;
namespace Sonnet {
    class Speller;
}

/**
Provides different comparison methods to determine the quality of the answer.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class AnswerValidator{
public:
    enum ErrorType {
        Correct,                /** < no error, solution was right */
        SpellingMistake,        /** < misspelled */
        CapitalizationMistake,  /** < capitalization error (whAt) */
        AccentMistake,          /** < an accent is missing or wrong (é) */
        WrongArticle,  /** < solution is correct with the article interchanged */
        FalseFriend,            /** < a false friend */
        Synonym,                /** < a synonym (may be correct) */
        Empty,                  /** < empty answer string */
        UnrelatedWord, /** < a valid word but no connection to the solution */
        UnknownMistake          /** < no idea */
    };

    AnswerValidator(KEduVocDocument* doc);

    ~AnswerValidator();

    bool spellcheckerAvailable();

    /**
     * Sets the current entry.
     * @param expression
     */
    void setTestEntry(TestEntry* entry, int translation);

    /**
     * Checks the user answer. If the answer was correct, 1.0 is returned,
     * 0.0 means no relation between answer and solution whatsoever.
     * check for type of mistake using typeOfMistake afterwards.
     * @param userAnswer
     * @return true if the answer is correct
     */
    double checkUserAnswer(const QString& userAnswer);
    double checkUserAnswer(const QString& solution, const QString& userAnswer, const QString& language = QString());

    QString correctedAnswerHtml();

    /**
     * Returns the type of mistake. Of course this is just an estimation.
     * @return
     */
    ErrorType typeOfMistake();


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
    int levenshteinDistance(QString s, QString t);

    bool spellcheckerMisspelled(QString s);

    bool spellcheckerInSuggestionList(QString solution, QString userAnswer);

    bool articleWrong();

    /**
     * Evaluate the closeness of two words. This is probably the most common case.
     * Should be used for single words. Will not work good for more complex cases.
     * @param word1
     * @param word2
     * @param errorType
     */
    ErrorType wordCompare(const QString & solution, const QString & userWord, double& grade, QString& htmlCorrection);

    /**
     * Split up a scentence and try to work magic with it...
     */
    void scentenceAnalysis();

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

    /// last type of mistake @see ErrorType
    ErrorType m_errorType;

    /// the last grade (0.0 .. 1.0)
    double m_grade;

    QString m_htmlCorrection;

    /// field used by the LevenshteinDistance to store temp data
    QByteArray m_d;

    /// spellchecker
    Sonnet::Speller* m_speller;
    bool m_spellerAvailable;

    static const double LEVENSHTEIN_THRESHOLD;
    static const double UNRELATED_WORD_GRADE;
    static const double FALSE_FRIEND_GRADE;
    static const double SPELLING_MISTAKE_PER_LETTER_PUNISHMENT;
    static const double CAPITALIZATION_MISTAKE_PUNISHMENT;
    static const double WRONG_ARTICLE_PUNISHMENT;
};

#endif
