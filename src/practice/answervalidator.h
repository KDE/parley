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

class KEduVocExpression;
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
        Correct,        /** < no error, solution was right */
        Spelling,       /** < misspelled */
        WrongArticle,   /** < solution is correct with the article interchanged */
        FalseFriend,    /** < a false friend */
        Synonym,        /** < a synonym (may be correct) */
        Wrong           /** < no idea */
    };

    AnswerValidator(KEduVocDocument* doc);

    ~AnswerValidator();

    bool spellcheckerAvailable();

    /**
     * Sets the right solution.
     * @param expression
     */
    void setSolution(KEduVocExpression* expression, int translation);

    /**
     * Checks the user answer. If the answer was correct, 1.0 is returned,
     * 0.0 means no relation between answer and solution whatsoever.
     * check for type of mistake using typeOfMistake afterwards.
     * @param userAnswer
     * @return true if the answer is correct
     */
    double checkUserAnswer(const QString& userAnswer);

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

    /// document for language and articles
    KEduVocDocument* m_doc;

    /// the entry to be checked
    KEduVocExpression* m_expression;
    int m_translation;

    /// the answer provided by the user
    QString m_userAnswer;

    ErrorType m_errorType;
    double m_grade;

    /// field used by the LevenshteinDistance to store temp data
    QByteArray m_d;

    /// spellchecker
    Sonnet::Speller* m_speller;
    bool m_spellerAvailable;
};

#endif
