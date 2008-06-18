/***************************************************************************

                            manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TESTENTRYMANAGER_H
#define TESTENTRYMANAGER_H

#include "testentry.h"
#include "prefs.h"

#include <keduvocexpression.h>
#include <QVector>

class KEduVocDocument;
class KRandomSequence;
class PracticeDialog;


class TestEntryManager
    :public QObject
{
Q_OBJECT
public:
    // should go...
    enum Result { Correct, SkipKnown, SkipUnknown, Wrong, Timeout, StopPractice };

    /**
     * Create a collection of entries to be practiced.
     * @param doc
     */
    explicit TestEntryManager(KEduVocDocument *doc, QWidget * parent = 0);
    /**
     * Default ctor
     */
    ~TestEntryManager();


    /**
     * Select appropriate entries for the practice (respect blocking settings etc)
     * m_allTestEntries will be filled by this.
     */
    void filterTestEntries();

    void startPractice();

    /**
     * The number of entries in the practice
     * @return
     */
    int totalEntryCount();

    /**
     * The number of entries that are still to be practiced
     * @return
     */
    int activeEntryCount();


    /// grade int to string (?)
    static QString gradeStr(int i);

    /**
     * Puts some grades on the shell
     */
    void printStatistics();

    int statisticTotalCorrectFirstAttempt();
    int statisticTotalWrong();
    int statisticTotalUnanswered();
    int statisticTotalSkipKnown();
    int statisticTotalSkipUnknown();

///@todo can this be private? we only connect from within
private slots:
    /**
     * The user has answered with this result, internal update.
     * @param result the result (PracticeDialog)
     */
    void slotResult(TestEntryManager::Result res);

    /**
     * This is called to get the next entry to show to the user.
     * @return
     */
    void setNextEntry();

private:
    KEduVocDocument *m_doc;
    QWidget *m_parent;
    int m_fromTranslation;
    int m_toTranslation;
    int m_testType;

    /// after x timeouts we pause the whole show
    int m_practiceTimeoutCounter;


    /// All entries in the test.
    QList<TestEntry*> m_allTestEntries;
    /// All entries that have not been asked.
    QList<TestEntry*> m_notAskedTestEntries;
    /// The list of entries that are being asked. If one of these is done, it can be deleted and an new one from m_notAskedTestEntries taken.
    QList<TestEntry*> m_currentEntries;
    int m_currentEntry;

    KRandomSequence* m_randomSequence;

    PracticeDialog* m_practiceDialog;


friend class EntryFilter;
};


#endif // kvtquery_included
