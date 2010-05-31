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


#ifndef TESTENTRY_H
#define TESTENTRY_H

#include <QFlags>

#include <keduvocexpression.h>

class TestEntry
{
public:
    enum ErrorType {
        SpellingMistake       =   0x1, /** < misspelled */
        CapitalizationMistake =   0x2, /** < capitalization error (whAt) */
        AccentMistake         =   0x4, /** < an accent is missing or wrong (é) */
        ArticleWrong          =   0x8, /** < solution is correct with the article interchanged */
        ArticleMissing        =   0x10, /** < solution is correct with the article missing*/
        FalseFriend           =  0x20, /** < a false friend */
        Synonym               =  0x40, /** < a synonym (may be correct) */
        Empty                 =  0x80, /** < empty answer string */
        UnrelatedWord         = 0x100, /** < a valid word but no connection to the solution */
        Incomplete            = 0x200, /** < the part that was entered is right, but not complete */
        Correct               = 0x400, /** < no error, solution was right */
        Wrong                 = 0x800  /** < some error, solution was somehow wrong check other bits */
    };

    Q_DECLARE_FLAGS(ErrorTypes, ErrorType)

    TestEntry(KEduVocExpression *entry);

    /// update the internal statistics for this practice with a right result
    void updateStatisticsRightAnswer();

    /// update the internal statistics for this practice with a wrong result
    void updateStatisticsWrongAnswer();

    /**
     check if the entry was finished and the practice backend
     may update the grades that will be saved to the file persistently
     @return whether the entry is done
    */
    bool changeGrades();

    int answeredCorrectInSequence();
    int statisticCount();
    int statisticGoodCount();
    int statisticBadCount();
    bool correctAtFirstAttempt();

    /**
     * in conjugation mode, use this tense for the entry
     */
    QString conjugationTense() const;
    void setConjugationTense(const QString& tense);

    void setLastErrors(ErrorTypes errorTypes);
    ErrorTypes lastErrors();

    void setLastPercentage(double percent);
    double lastPercentage();

    void addUserAnswer(const QString& answer) { if (!answer.isEmpty()) m_userAnswers.append(answer); }
    QStringList userAnswers() { return m_userAnswers; }

    int totalEntries();

    static void setGradeFrom(int from);
    static void setGradeTo(int to);
    static int gradeFrom();
    static int gradeTo();

    KEduVocExpression *entry();

private:
    /// the entry itself
    KEduVocExpression *m_entry;
    QString m_conjugationTense;

    // these are for the CURRENT query only, so we can display statistics.
    int m_statisticCount;
    int m_statisticGoodCount;
    int m_statisticBadCount;
    int m_answeredCorrectInSequence;
    bool m_correctAtFirstAttempt;
    bool m_practiceFinished;
    bool m_changeGrades;

    double m_lastPercentage;
    ErrorTypes m_lastError;

    QStringList m_userAnswers;

    static int m_gradeTo;
    static int m_gradeFrom;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TestEntry::ErrorTypes)

#endif
