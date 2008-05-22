//
// C++ Interface: statistics
//
// Description: Keeps track of statistics about the practice
//
///\file statistics.h
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

 /***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EDU_STATISTICS_H
#define EDU_STATISTICS_H

#include <QLCDNumber>

/**
* @class Statistics
* @author David Capel <wot.narg@gmail.com>
* @brief This class keeps track of various statistics about the practice.
* Depending on the practice mode, this may or may not have a visible widget representation.
*/
class Statistics
{
    public:
        Statistics();
        virtual ~Statistics();

        
        /// Tainting causes an answer to be counted as incorrect (even if it is
        /// correct), and the tainting frequency is statisticly tracked.
        /// These denote the reason that an answer is tainted.
        enum TaintReason
        {
            HintShown, ///< The hint function was used.
            OutOfTime, ///< The timer ran out, but the user entered an answer anyway.
            NumberTaintReasons
        };

        /// These denote the reason the question was skipped.
        enum SkipReason
        {
            Known, ///< The user knew the answer and skipped the question.
            Unknown, ///< The user didn't know the answer and skipped the question.
            NumberSkipReasons
        };

        /// These denote why the answer was incorrect.
        /// Multiple flags may be used with a bitwise OR.
        enum ErrorType
        {
            SpellingMistake       =   0x1, ///< misspelled
            CapitalizationMistake =   0x2, ///< capitalization error (whAt)
            AccentMistake         =   0x4, ///< an accent is missing or wrong (é)
            ArticleWrong          =   0x8, ///< solution is correct with the article interchanged
            ArticleMissing        =   0x10, ///< solution is correct with the article missing
            FalseFriend           =  0x20, ///< a false friend
            Synonym               =  0x40, ///< a synonym (may be correct)
            Empty                 =  0x80, ///< empty answer string
            UnrelatedWord         = 0x100, ///< a valid word but no connection to the solution
            Incomplete            = 0x200, ///< the part that was entered is right, but not complete
            Correct               = 0x400, ///< no error, solution was right
            UnknownMistake        = 0x800,  ///< no idea
            NumberIncorrectReasons = 12
        };

    public: //slots:
        /// Called when the answer is correct.
        virtual void slotCorrect();
        /// Called when the answer is incorrect; ErrorType denotes why it was incorrect.
        virtual void slotIncorrect(Statistics::ErrorType error);
        /// Called when the question was skipped. SkipReason denotes if the user knew the answer.
        virtual void slotSkipped(Statistics::SkipReason reason);
        /// Called when the answer is tainted (because of a hint or running out of time)
        virtual void slotTaintAnswer(Statistics::TaintReason reason);

        /// Update the GUI display of the statistics
        virtual void refresh() = 0;


    protected:
        /// Percent of questions correctly answered.
        float m_percentCorrect;
        /// Number of questions attempted.
        int m_attempted;
        /// Number of untainted questions correctly answered.
        int m_correct;
        /// Cycles ar}e iterations through a given practice set.
        int m_cycles;
        /// If the current question is tainted.
        bool m_tainted;
        /// How many answers were correct after they were tainted.
        int m_taintedCorrect;
        /// How many answers were incorrect after they were tainted.
        int m_taintedIncorrect;

        /// How many questions were skipped.
        int m_skipped;
        /// How many questions have been answered correct in a row
        int m_streakLength;

        /// Keeps track of how often a question is incorrect for each error.
        /// Note that multiple errors can be incremented at once.
        int errorReasons[NumberIncorrectReasons];
        /// Keeps track of how often a question is tainted for each reason.
        int taintReasons[NumberTaintReasons];
        /// Keeps track of how often a question is skipped for each reason.
        int skipReasons[NumberSkipReasons];
};

class LCDStatistics : public QLCDNumber, public Statistics
{
    Q_OBJECT

    public:
        LCDStatistics(QWidget* parent = 0);
            
    public slots:
        /// Called when the answer is correct.
        void slotCorrect() {Statistics::slotCorrect(); };
        /// Called when the answer is incorrect; ErrorType denotes why it was incorrect.
        void slotIncorrect(Statistics::ErrorType error) { Statistics::slotIncorrect(error); };
        /// Called when the question was skipped. SkipReason denotes if the user knew the answer.
        void slotSkipped(Statistics::SkipReason reason) { Statistics::slotSkipped(reason); };
        /// Called when the answer is tainted (because of a hint or running out of time)
        void slotTaintAnswer(Statistics::TaintReason reason) { Statistics::slotTaintAnswer(reason); };

        void refresh();
};

#endif
