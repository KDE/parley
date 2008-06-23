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

#ifndef PRACTICEENTRY_H
#define PRACTICEENTRY_H

#include <keduvocexpression.h>

#include <QString>
#include <QStringList>

#include "statistics.h"

/// @class PracticeEntry
/// @brief A wrapper class for @c KEduVocExpression
class PracticeEntry
{
    public:
        PracticeEntry(KEduVocExpression*);

        int attempted() const { return m_attempted; }
        int correct() const  { return m_correct; }
        int skipped() const { return m_skipped; }
        int taintedCorrect() const { return m_taintedCorrect; }
        int taintedIncorrect() const  { return m_taintedIncorrect; }
        /// Gets a list of incorrect answers that were supplied for this entry.
        QStringList incorrectAnswers() { return m_incorrectAnswers; }
        /// Get how often entry was tainted for ErrorType @param error
        int errorReason(Statistics::ErrorType error) const { return m_errorReasons[error]; }
        /// Get how often they entry was tainted for TaintReason @param reason
        int taintReason(Statistics::TaintReason reason) const { return m_taintReasons[reason]; }
        /// Get how often the entry was skipped for SkipReason @param reason
        int skippedReason(Statistics::SkipReason reason) const { return m_skipReasons[reason]; }

        KEduVocExpression* expression() const { return m_expression; }



        /// Call when the entry was correctly answered.
        void answeredCorrectly(bool tainted = false);
        /// Call when the entry was incorrectly answered.
        void answeredIncorrectly(Statistics::ErrorType, const QString& incorrectAnswer, bool tainted = false);

        /// Increment the Leitner Grade.
        void incGrade();
        /// Decrement the Leitner Grade.
        void decGrade();

        void addIncorrectAnswer(const QString& incorrectAnswer);

        /// Increment how often they entry was tainted for TaintReason @param reason
        void incTaintReason(Statistics::TaintReason reason) { ++m_taintReasons[reason]; }
        /// Increment how often the entry was skipped for SkipReason @param reason
        void incSkippedReason(Statistics::SkipReason reason) { ++m_skipReasons[reason]; }
    protected:
        /// Increment how often entry was tainted for ErrorType @param error
        void incErrorReason(Statistics::ErrorType error);

        /// Number of times attempted.
        int m_attempted;
        /// Number of time it was untaintedly and correctly answered.
        int m_correct;
        /// How many answers were correct after they were tainted.
        int m_taintedCorrect;
        /// How many answers were incorrect after they were tainted.
        int m_taintedIncorrect;

        /// Number of times skipped.
        int m_skipped;

        /// Keeps track of how often a question is incorrect for each error.
        /// Note that multiple errors can be incremented at once.
        int m_errorReasons[NumberIncorrectReasons];
        /// Keeps track of how often a question is tainted for each reason.
        int m_taintReasons[NumberTaintReasons];
        /// Keeps track of how often a question is skipped for each reason.
        int m_skipReasons[NumberSkipReasons];

        KEduVocExpression* m_expression;

        QStringList m_incorrectAnswers;
};


#endif