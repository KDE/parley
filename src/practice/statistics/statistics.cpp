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
#include "../practiceentry.h"

#include <KDebug>
#include <KLocalizedString>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>

#include <KStandardDirs>

Statistics::Statistics(PracticeEntryManager* manager, QObject * parent)
    : QObject(parent),
      m_manager(manager)
{
    m_tainted = false;
    m_answerChecked = false;
    m_attempted = 0;
    m_correct = 0;
    m_cycles = 0;
    m_tainted = 0;
    m_taintedCorrect = 0;
    m_taintedIncorrect = 0;
    m_skipped = 0;
    m_streakLength = 0;
    m_entry = 0;
}

Statistics::~Statistics() {}

void Statistics::slotCorrection(float grade, ErrorType error, const QString& userAnswer)
{
    // this is true when the answer supplied was correct.
    if ((grade == 1.0) && (error == Correct))
        slotCorrect();
    else
        slotIncorrect(error, userAnswer);
}

void Statistics::slotIncorrect(ErrorType error, const QString& incorrectAnswer)
{
    ++m_attempted;

    incErrorReason(error);

    m_tainted = false;

    m_answerChecked = true;

    m_streakLength = 0;

    m_entry->answeredIncorrectly(error, incorrectAnswer, m_tainted);

    m_manager->appendEntryToList(m_entry);
    emit signalUpdateDisplay(this);
}

void Statistics::slotCorrect()
{
    ++m_attempted;
    if (m_tainted)
    {
        ++m_taintedCorrect;
        m_tainted = false;
        m_streakLength = 0;
    }
    else
    {
        ++m_correct;
        ++m_streakLength;
    }

    m_answerChecked = true;

    m_entry->answeredCorrectly(m_tainted);

    emit signalUpdateDisplay(this);
}

void Statistics::slotSolutionShown()
{
    if (m_answerChecked) // the answer has already been checked and counted.
        return; // do nothing
    else // the user requested that the answer be shown before they provided an answer
        slotIncorrect(SolutionShown, "");
}

void Statistics::incErrorReason(Statistics::ErrorType error)
{
    if (error & SpellingMistake) ++m_errorReasons[0];
    if (error & CapitalizationMistake) ++m_errorReasons[1];
    if (error & AccentMistake) ++m_errorReasons[2];
    if (error & ArticleWrong) ++m_errorReasons[3];
    if (error & ArticleMissing) ++m_errorReasons[4];
    if (error & FalseFriend) ++m_errorReasons[5];
    if (error & Synonym) ++m_errorReasons[6];
    if (error & Empty) ++m_errorReasons[7];
    if (error & UnrelatedWord) ++m_errorReasons[8];
    if (error & Incomplete) ++m_errorReasons[9];
    if (error & Correct) ++m_errorReasons[10]; // this should never be used...
    if (error & SolutionShown) ++m_errorReasons[11];
    if (error & TimedOut) ++m_errorReasons[12];
    if (error & UnknownMistake) ++m_errorReasons[13];
}


void Statistics::slotSkippedKnown()
{
    ++m_skipReasons[Known];
    m_entry->incSkippedReason(Known);
}

void Statistics::slotSkippedUnknown()
{
    ++m_attempted;
    m_tainted = false;
    m_streakLength = 0;
    ++m_skipReasons[Unknown];

    m_entry->incSkippedReason(Unknown);
    emit signalUpdateDisplay(this);
}

void Statistics::slotTaintAnswer(TaintReason reason)
{
    ++m_taintReasons[reason];
    m_entry->incTaintReason(reason);
}


void Statistics::slotSetFinished()
{
    // TODO do something here ;)
    emit signalQuit();
}


void Statistics::slotQuestionTimedOut()
{
    slotIncorrect(TimedOut, "");
}

void Statistics::slotSetTimedOut()
{
    // TODO do something here
    slotSetFinished();
}

const QString Statistics::gradeToString(int i)
{
    switch (i)
    {

        case KV_NORM_GRADE:
            return i18n(KV_NORM_TEXT);
            break;

        case KV_LEV1_GRADE:
            return i18n(KV_LEV1_TEXT);
            break;

        case KV_LEV2_GRADE:
            return i18n(KV_LEV2_TEXT);
            break;

        case KV_LEV3_GRADE:
            return i18n(KV_LEV3_TEXT);
            break;

        case KV_LEV4_GRADE:
            return i18n(KV_LEV4_TEXT);
            break;

        case KV_LEV5_GRADE:
            return i18n(KV_LEV5_TEXT);
            break;

        case KV_LEV6_GRADE:
            return i18n(KV_LEV6_TEXT);
            break;

        case KV_LEV7_GRADE:
            return i18n(KV_LEV7_TEXT);
            break;

        default:
            return i18n(KV_LEV1_TEXT);
            break;
    }
}

