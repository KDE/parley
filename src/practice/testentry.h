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

class KEduVocExpression;


class TestEntry
{
public:
    TestEntry(KEduVocExpression *_exp, int _nr)
    {
        exp = _exp;
        m_index = _nr;
        m_answeredCorrectInSequence = 0;
        m_statisticCount = 0;
        m_statisticGoodCount = 0;
        m_statisticBadCount = 0;
        m_statisticTimeout = 0;
        m_statisticSkipUnknown = 0;
        m_statisticSkipKnown = 0;
        m_canSwitchDirection = false;
        m_correctAtFirstAttempt = false;
    }

    void incGoodCount();
    void incBadCount();
    void incTimeout();
    void incSkipUnknown();
    void incSkipKnown();

    int answeredCorrectInSequence();
    int statisticCount();
    int statisticGoodCount();
    int statisticBadCount();
    int statisticSkipKnown();
    int statisticSkipUnknown();
    int statisticTimeout();
    bool statisticCorrectAtFirstAttempt();
//     statisticWrong
//     statisticUnanswered
//     statisticSkipKnown
//     statisticSkipUnknown

    int totalEntries();

    ///@todo make these two members private
    KEduVocExpression *exp;
    int m_index;

    static void setGradeFrom(int from);
    static void setGradeTo(int to);
    static int gradeFrom();
    static int gradeTo();

private:
    void update();

    // these are for the CURRENT query only, so we can display statistics.
    int m_statisticCount;
    int m_statisticGoodCount;
    int m_statisticBadCount;
    int m_statisticTimeout;
    int m_statisticSkipUnknown;
    int m_statisticSkipKnown;
    int m_answeredCorrectInSequence;
    bool m_canSwitchDirection;
    bool m_correctAtFirstAttempt;

    static int m_gradeTo;
    static int m_gradeFrom;
};


#endif
