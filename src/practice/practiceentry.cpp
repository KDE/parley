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
#include "statistics/statistics.h"


PracticeEntry::PracticeEntry(KEduVocExpression* expr)
: m_expression(expr)
{
    m_attempted = 0;
    m_correct = 0;
    m_taintedCorrect = 0;
    m_taintedIncorrect = 0;
    m_skipped = 0;

    /// @todo initialize
//     m_errorReasons[Statistics::NumberIncorrectReasons];
//     m_taintReasons[Statistics::NumberTaintReasons];
//     m_skipReasons[Statistics::NumberSkipReasons];

}

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
}

void PracticeEntry::answeredIncorrectly(Statistics::ErrorType, const QString& incorrectAnswer, bool tainted)
{
    if (tainted)
        ++m_taintedIncorrect;

    ++m_attempted;
    decGrade();
    addIncorrectAnswer(incorrectAnswer);
}

void PracticeEntry::incErrorReason(Statistics::ErrorType error)
{

    if (error & Statistics::SpellingMistake) ++m_errorReasons[0];
    if (error & Statistics::CapitalizationMistake) ++m_errorReasons[1];
    if (error & Statistics::AccentMistake) ++m_errorReasons[2];
    if (error & Statistics::ArticleWrong) ++m_errorReasons[3];
    if (error & Statistics::ArticleMissing) ++m_errorReasons[4];
    if (error & Statistics::FalseFriend) ++m_errorReasons[5];
    if (error & Statistics::Synonym) ++m_errorReasons[6];
    if (error & Statistics::Empty) ++m_errorReasons[7];
    if (error & Statistics::UnrelatedWord) ++m_errorReasons[8];
    if (error & Statistics::Incomplete) ++m_errorReasons[9];
    if (error & Statistics::Correct) ++m_errorReasons[10]; // this should never be used...
    if (error & Statistics::SolutionShown) ++m_errorReasons[11];
    if (error & Statistics::TimedOut) ++m_errorReasons[12];
    if (error & Statistics::UnknownMistake) ++m_errorReasons[13];
}


void PracticeEntry::incGrade()
{
    // TODO do this once fregl finishes leitner
}

void PracticeEntry::decGrade()
{
    // TODO do this once fregl finishes leitner
}

void PracticeEntry::addIncorrectAnswer(const QString& incorrectAnswer)
{
    m_incorrectAnswers.append(incorrectAnswer);
}
