/***************************************************************************
    Copyright 2008 David Capel <wot.narg@gmail.com>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practiceentry.h"
#include "statistics.h"


PracticeEntry(KEduVocExpression* expr)
: m_expression(expr)
{}

void PracticeEntry::answeredCorrectly(bool tainted)
{
    if (!tainted)
    {
        ++m_attempted;
        ++m_correct;
        incGrade();
    }
    else
    {
        ++m_attempted;
        ++m_taintedCorrect;
        decGrade();
    }
};

void answeredIncorrectly(Statistics::ErrorType, const QString& incorrectAnswer, bool tainted)
{
    if (tainted)
        ++m_taintedIncorrect;

    ++m_attempted;
    decGrade();
    addIncorrectAnswer(incorrectAnswer);
}

void incErrorReason(Statistics::ErrorType error)
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
    if (error & UnknownMistake) ++m_errorReasons[11];
}


void PracticeEntry::incGrade()
{
    // TODO do this once fregl finishes leitner
}

void PracticeEntry::decGrade()
{
    // TODO do this once fregl finishes leitner
}

void appendIncorrectAnswer(const QString& incorrectAnswer)
{
    m_incorrectAnswers.append(incorrectAnswer);
}