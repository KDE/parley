//
// C++ Implementation: statistics
//
// Description: Implementation of the statistics-tracking eduwidget
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "statistics.h"

#include <KDebug>

Statistics::Statistics()
{
    m_percentCorrect = 0.0f;
    m_tainted = false;
    m_attempted = 0;
    m_correct = 0;
    m_cycles = 0;
    m_tainted = 0;
    m_taintedCorrect = 0;
    m_taintedIncorrect = 0;
    m_skipped = 0;
    m_streakLength = 0;
}

Statistics::~Statistics() {}

LCDStatistics::LCDStatistics(QWidget * parent)
    :  QLCDNumber(parent), Statistics()
{
    refresh();
}


void Statistics::slotCorrect()
{
    kDebug() << "here";
    ++m_attempted;

    if (m_tainted)
    {
        ++m_taintedCorrect;
        m_tainted = false;
    }
    else
    {
        ++m_correct;
        ++m_streakLength;
    }

    m_percentCorrect = static_cast<float>(m_correct) / m_attempted;

    kDebug() << m_correct << " / " << m_attempted << " = " << m_percentCorrect;

    refresh();
}

void Statistics::slotIncorrect(ErrorType error)
{
    ++m_attempted;
    // I don't know a better way to do this...

    if (error & SpellingMistake) ++errorReasons[0];

    if (error & CapitalizationMistake) ++errorReasons[1];

    if (error & AccentMistake) ++errorReasons[2];

    if (error & ArticleWrong) ++errorReasons[3];

    if (error & ArticleMissing) ++errorReasons[4];

    if (error & FalseFriend) ++errorReasons[5];

    if (error & Synonym) ++errorReasons[6];

    if (error & Empty) ++errorReasons[7];

    if (error & UnrelatedWord) ++errorReasons[8];

    if (error & Incomplete) ++errorReasons[9];

    if (error & Correct) ++errorReasons[10]; // this should never be used...

    if (error & UnknownMistake) ++errorReasons[11];

    // avoid a division by 0 error...
    m_percentCorrect = m_attempted != 0 ? static_cast<float>(m_correct) / m_attempted : 0.0f;

    m_tainted = false;

    refresh();
}

void Statistics::slotSkipped(SkipReason reason)
{
    ++skipReasons[reason];
}

void Statistics::slotTaintAnswer(TaintReason reason)
{
    ++taintReasons[reason];
}

void LCDStatistics::refresh()
{
    kDebug() << m_percentCorrect;
    display(m_percentCorrect*100);
}
