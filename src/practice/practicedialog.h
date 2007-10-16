/***************************************************************************

                   base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICEDIALOG_H
#define PRACTICEDIALOG_H

#include "testentrymanager.h"
#include "prefs.h"

#include <keduvocgrammar.h>
#include <KUrl>
#include <KDialog>
#include <QCloseEvent>
#include <QProgressBar>

class AnswerValidator;

class KEduVocExpression;
class KEduVocDocument;

class QLineEdit;
class QTextEdit;
class QRadioButton;
class QGraphicsView;

namespace Phonon
{
    class MediaObject;
}

class PracticeDialog : public KDialog
{
Q_OBJECT
public:

    enum Result { Correct, SkipKnown, SkipUnknown, Wrong, Timeout, StopIt };

    enum WidgetStyle {
        Default,        /**< Normal font, normal color (black) */
        PositiveResult, /**< Green text, bold */
        NegativeResult, /**< Red text, bold */
        HintStyle       /**< Blue text, bold */
    };

    /**
     * Default constructor for a test dialog.
     * @param caption the dialog title
     * @param doc the document to be used
     * @param parent parent widget
     */
    PracticeDialog(const QString & caption, KEduVocDocument *doc, QWidget *parent);

    void setTestEntryManager(TestEntryManager* testEntryManager);

    /**
     * Default dtor
     */
    virtual ~PracticeDialog();

    /**
     * Set an entry to be tested. Each dialog should reimplement this.
     * You must call this to get the timer started!
     * @param entry
     */
    virtual void setEntry(TestEntry* entry) = 0;

    /**
     * Informs the dialog of the number of entries to be tested.
     * @param current number of answered entries
     * @param total number of entries in the test
     */
    virtual void setProgressCounter(int current, int total) = 0;

public slots:
    /**
     * Let the dialog show the solution.
     */
    virtual void showSolution() = 0;

    /**
     * Informs the dialog that the time is up.
     */
    void timeoutReached();

    /**
     * Slot that starts the edit entry dialog.
     A practice dialog can call this to allow editing an entry.
     */
    void editEntry();

    int exec();

protected slots:
    /**
     * Call this when the user decided to skip this entry as KNOWN.
     */
    void skipKnown();

    /**
     * Call this when the user decided to skip this entry as UNKNOWN.
     */
    void skipUnknown();

    /**
     * Play the audio file associated with the question.
     */
    void audioPlayFromIdentifier();

    /**
     * Play the soundfile associated with the solution.
     */
    void audioPlayToIdentifier();

    /**
     * The continue button has been clicked - stop m_showSolutionTimer
     */
    void continueButtonClicked();

protected:
    /**
     * Set the next entry from m_entryManager or return if all vocabulary have been practiced.
     */
    void nextEntry();

    /**
     *
     * @param res
     */
    void slotResult(PracticeDialog::Result res);

    void resultCorrect();
    void resultWrong();

    virtual void setStatusText(const QString &status) = 0;
    virtual QProgressBar* timebar() = 0;

    void audioPlayFile(const KUrl& soundFile);

    void imageShowFile(QGraphicsView* view, const QString& url);
    void imageShowFromEntry(QGraphicsView* view, const TestEntry* entry);

    /** compare two strings with simplified applied first (pretty smart huh?) */
    bool smartCompare(const QString&, const QString&) const;

    /**
     * Use this to check if the answer is correct.
     * Use only to check the word itself, does not work on other properties like comparison forms etc.
     * @param userAnswer
     * @return a fuzzy answer where 1.0 is the correct solution and 0.0 completely wrong
     */
    double verifyAnswer(const QString& userAnswer);

    /**
     * Like verifyAnswer but for two strings, so the entry is not taken into consideration.
     * @param solution
     * @param userAnswer
     * @return
     */
    double verifyAnswer(const QString& solution, const QString& userAnswer);

    /**
     * Set the color scheme of a widget to WidgetStyle.
     * @param widget the widget to be modified
     * @param style WidgetStyle
     */
    void setWidgetStyle(QWidget * widget, WidgetStyle style = Default);
    // Show string after selecting known/unknown
    // depending on progress and randomness
    QString  getOKComment(int percent);
    QString  getNOKComment(int percent);
    QString  getTimeoutComment(int percent);

    virtual void closeEvent(QCloseEvent*e);

    KEduVocDocument   *m_doc;
    TestEntry* m_entry;
    int m_testType;

    /**
     * When a timeout per question is set in the prefs, this starts the timer.
     * Called by setQuery, so don't wory about it. Cann be called to restart the timer.
     */
    void startAnswerTimer();

    /**
     * When the solution is shown etc there's no point in counting any more...
     */
    void stopAnswerTimer();

    /**
     * Emits nextEntry() when the timeout for showing the solution is reached.
     */
    void startShowSolutionTimer();

    /**
     * When @p tainted is set true, a correct answer will be counted as wrong (for example show more button).
     * @param tainted
     */
    void setAnswerTainted(bool tainted = true);

    /**
     * Returns true if the answer is counted as wrong.
     * @return
     */
    bool answerTainted();

    /// The vocabulary will come from here
    TestEntryManager    *m_entryManager;

private:

    QTimer    *m_answerTimer;
    int        m_answerTimerCount;

    QTimer    *m_showSolutionTimer;

    /// true if the user entered a false answer or received help (show more button). we still let the user give input until the right one is selected.
    bool m_answerTainted;

    /// The sound player. Has to be initialized before using the first time.
    Phonon::MediaObject* m_player;

    /// Advanced string evaluation
    AnswerValidator* m_validator;

    /// after x timeouts we pause the whole show
    int num_practiceTimeout;

};


#endif
