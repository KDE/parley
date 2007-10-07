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

#include <keduvocgrade.h>
#include <keduvocexpression.h>



int TestEntry::m_gradeFrom = 0;
int TestEntry::m_gradeTo = 1;

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

int TestEntry::statisticSkipKnown()
{
    return m_statisticSkipKnown;
}

int TestEntry::statisticSkipUnknown()
{
    return m_statisticSkipUnknown;
}

int TestEntry::statisticTimeout()
{
    return m_statisticTimeout;
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
        exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incGrade();
        m_correctAtFirstAttempt = true;
    } else {
        // alt learn: 3 times right
        if ( answeredCorrectInSequence() == 3  && m_statisticBadCount == 0 ) {
            exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incGrade();
            m_correctAtFirstAttempt = true;
        }
    }
}

void TestEntry::incSkipKnown()
{
    update();
    m_statisticSkipKnown++;
    m_answeredCorrectInSequence++;
    // increase grade, if first time:
    if ( !Prefs::altLearn() && m_statisticBadCount == 0 ) {
        exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incGrade();
    } else {
        // alt learn: 3 times right
        if ( answeredCorrectInSequence() == 3  && m_statisticBadCount == 0 ) {
            exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incGrade();
        }
    }
}

void TestEntry::incBadCount()
{
    update();
    m_statisticBadCount++;
    m_answeredCorrectInSequence = 0;
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).decGrade();
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incBadCount();
}

void TestEntry::incTimeout()
{
    update();
    m_statisticTimeout++;
    m_answeredCorrectInSequence = 0;
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).decGrade();
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incBadCount();
}

void TestEntry::incSkipUnknown()
{
    update();
    m_statisticSkipUnknown++;
    m_answeredCorrectInSequence = 0;
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).decGrade();
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incBadCount();
}


void TestEntry::update()
{
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).incQueryCount();
    m_statisticCount++;
    exp->translation(m_gradeTo).gradeFrom(m_gradeFrom).setQueryDate( QDateTime::currentDateTime() );
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




