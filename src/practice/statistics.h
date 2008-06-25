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
#include <QGraphicsSvgItem>

#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <keduvocdocument.h>

#include "practiceentrymanager.h"

class PracticeEntry;

/**
* @class Statistics
* @author David Capel <wot.narg@gmail.com>
* @brief This class keeps track of various statistics about the practice.
* Depending on the practice mode, this may or may not have a visible widget representation.
*/
class Statistics : public QObject
{
    Q_OBJECT

    public:
        Statistics(PracticeEntryManager* manager, QObject * parent = 0);
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
            ArticleMissing        =  0x10, ///< solution is correct with the article missing
            FalseFriend           =  0x20, ///< a false friend
            Synonym               =  0x40, ///< a synonym (may be correct)
            Empty                 =  0x80, ///< empty answer string
            UnrelatedWord         = 0x100, ///< a valid word but no connection to the solution
            Incomplete            = 0x200, ///< the part that was entered is right, but not complete
            Correct               = 0x400, ///< no error, solution was right
            SolutionShown         = 0x800, ///< the answer was shown before the user provided an answer
            TimedOut              =0x1000, ///< there was a per-question time limit and it ran out
            UnknownMistake        =0x2000,  ///< no idea
            NumberIncorrectReasons = 14
        };

    public slots:
        /// Called by AnswerValidator when a correction has been completed.
        /// @param grade is a 'correctness score', a number from 0.0 to 1.0 denoting how close
        /// the supplied answer was to the solution. 1.0 means it was correct, 0.0 means it was entirely
        /// wrong.
        /// @param error contains the errors the user made. If only Correct is set, the answer is correct.
        /// userAnswer is stored if it is incorrect (something about learning from mistakes.... ;) )
        void slotCorrection(float grade, Statistics::ErrorType error, const QString& userAnswer);
        /// Called when the answer is correct.
        void slotCorrect();
        /// Called when the answer is incorrect; ErrorType denotes why it was incorrect.
        void slotIncorrect(Statistics::ErrorType error, const QString& userAnswer);
        /// Called when a known question was skipped.
        void slotSkippedKnown();
        /// Called when an unknown question was skipped.
        void slotSkippedUnknown();
        /// Called when the answer is tainted (because of a hint or running out of time)
        void slotTaintAnswer(Statistics::TaintReason reason);
        /// Called to set the current prompt expression
        /// We use this to update grade, practice count, etc
        void slotSetEntry(PracticeEntry* e) { m_entry = e; m_answerChecked = false; };
        /// Called when the set of entries is exhausted.
        /// Most practice modes will want to show a statistical summery before it exits.
        void slotSetFinished();
        /// Called when the user has requested that the answer be shown.
        /// If the question has not been answered, it will be counted as incorrect.
        void slotSolutionShown();
        /// Called if a question timer is set and it runs out of time.
        void slotQuestionTimedOut();
        /// Called if a set of questions has a total-set time limit and it has run out.
        void slotSetTimedOut();

    public:
        static const QString gradeToString(int grade);


        /// Returns the percent of entries that have been answered correctly.
        /// This will return 0 when the numerator and/or denominator is 0,
        /// This practice is mathmatically incorrect (n/0 != 0/0 != 0), but works in practice
        float percentCorrect()
        { return m_attempted != 0 ? static_cast<float>(m_correct) / m_attempted : 0.0f; }
        bool answerChecked() const  { return m_answerChecked; }
        int attempted() const { return m_attempted; }
        int correct() const  { return m_correct; }
        int cycles() const { return m_cycles; }
        int skipped() const { return m_skipped; }
        int streakLength() const { return m_streakLength; }
        int taintedCorrect() const { return m_taintedCorrect; }
        int taintedIncorrect() const  { return m_taintedIncorrect; }
        /// Get how often entries were tainted for ErrorType @param error
        int errorReason(ErrorType error) const { return m_errorReasons[error]; }
        /// Get how often entries were tainted for TaintReason @param reason
        int taintReason(TaintReason reason) const { return m_taintReasons[reason]; }
        /// Get how often entries were skipped for SkipReason @param reason
        int skippedReason(SkipReason reason) const { return m_skipReasons[reason]; }
        PracticeEntryManager* manager() const { return m_manager; }


    signals:
        void signalUpdateDisplay(Statistics*);
        void signalQuit();

    protected:
        void incErrorReason(ErrorType error);

        /// Number of questions attempted.
        int m_attempted;
        /// Number of untainted questions correctly answered.
        int m_correct;
        /// Cycles ar}e iterations through a given practice set.
        int m_cycles;
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
        int m_errorReasons[NumberIncorrectReasons];
        /// Keeps track of how often a question is tainted for each reason.
        int m_taintReasons[NumberTaintReasons];
        /// Keeps track of how often a question is skipped for each reason.
        int m_skipReasons[NumberSkipReasons];

        PracticeEntry* m_entry;

        PracticeEntryManager* m_manager;

    // These are purely implementation details, and subclasses shouldn't play with them.
    private:
        /// This is set when an answer has been corrected and counted and is cleared when a new prompt has been displayed.
        /// This prevents double-counting of answers, especially when hints are involved.
        bool m_answerChecked;

        /// If the current question is tainted.
        bool m_tainted;


};

class LCDStatistics : public QLCDNumber
{
    Q_OBJECT

    public:
        LCDStatistics(QWidget* parent = 0);

    public slots:
        /// Refreshes the GUI display.
        void slotUpdateDisplay(Statistics*);
};

class SvgBarStatistics : public QGraphicsSvgItem
{
    Q_OBJECT

    public:
        SvgBarStatistics(QSvgRenderer * renderer, const QString& foregroundElementId, const QString& backgroundElementId, QGraphicsItem * parent = 0);
        ~SvgBarStatistics();

    public slots:
        /// Refreshes the GUI display.
        void slotUpdateDisplay(Statistics*);


    private:
        QRectF m_backgroundRect;
};
#endif
