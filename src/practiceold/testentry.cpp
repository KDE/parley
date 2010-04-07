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

#include "testentry.h"

#include "prefs.h"

int TestEntry::m_gradeFrom = 0;
int TestEntry::m_gradeTo = 1;

TestEntry::TestEntry(KEduVocExpression *entry)
    :m_entry(entry)
    ,m_statisticCount(0)
    ,m_statisticGoodCount(0)
    ,m_statisticBadCount(0)
    ,m_answeredCorrectInSequence(0)
    ,m_correctAtFirstAttempt(false)
    ,m_lastPercentage(0.0)
    ,m_lastError(UnknownMistake)
{}

void TestEntry::setGradeTo(int to)
{
    m_gradeTo = to;
}

void TestEntry::setGradeFrom(int from)
{
    m_gradeFrom = from;
}

int TestEntry::answeredCorrectInSequence()
{
    return m_answeredCorrectInSequence;
}

int TestEntry::statisticCount()
{
    return m_statisticCount;
}

int TestEntry::statisticBadCount()
{
    return m_statisticBadCount;
}

int TestEntry::statisticGoodCount()
{
    return m_statisticGoodCount;
}

void TestEntry::incGoodCount()
{
    update();
    m_statisticGoodCount++;
    m_answeredCorrectInSequence++;
    // increase grade, if first time:
    if ( !Prefs::altLearn() && m_statisticBadCount == 0 ) {
        m_entry->translation(m_gradeTo)->incGrade();
        m_correctAtFirstAttempt = true;
    } else {
        // alt learn: 3 times right
        if ( answeredCorrectInSequence() == 3  && m_statisticBadCount == 0 ) {
            m_entry->translation(m_gradeTo)->incGrade();
            m_correctAtFirstAttempt = true;
        }
    }
}

void TestEntry::incBadCount()
{
    update();

    // pretty hard, decrease the grade every single time
    // could be changed, but maybe it's a good idea even...
    m_entry->translation(m_gradeTo)->decGrade();
    m_entry->translation(m_gradeTo)->incBadCount();

    m_statisticBadCount++;
    m_answeredCorrectInSequence = 0;
}

void TestEntry::update()
{
    m_entry->translation(m_gradeTo)->incPracticeCount();
    m_statisticCount++;
    m_entry->translation(m_gradeTo)->setPracticeDate( QDateTime::currentDateTime() );
}

int TestEntry::gradeFrom()
{
    return m_gradeFrom;
}

int TestEntry::gradeTo()
{
    return m_gradeTo;
}

bool TestEntry::statisticCorrectAtFirstAttempt()
{
    return m_correctAtFirstAttempt;
}


void TestEntry::setLastErrors(ErrorTypes errorTypes)
{
    m_lastError = errorTypes;
}

TestEntry::ErrorTypes TestEntry::lastErrors()
{
    return m_lastError;
}

void TestEntry::setLastPercentage(double percent)
{
    m_lastPercentage = percent;
}

double TestEntry::lastPercentage()
{
    return m_lastPercentage;
}

KEduVocExpression * TestEntry::entry()
{
    return m_entry;
}
