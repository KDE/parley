/***************************************************************************
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
                    (C) 2014      Inge Wallin <inge@lysator.liu.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICESESSIONMANAGERBASE_H
#define PRACTICESESSIONMANAGERBASE_H

// Qt
#include <QVector>

// kdelibs
#include <KRandomSequence>

// kdeedulibs
#include <keduvocexpression.h>

// Parley
#include "testentry.h"
#include "prefs.h"


class KEduVocDocument;
class PracticeDialog;

namespace Practice
{

class SessionManagerBase
{
public:
    /**
     * Create a collection of entries to be practiced.
     * @param doc
     */
    explicit SessionManagerBase(QWidget *parent);

    /**
     * Default destructor
     */
    ~SessionManagerBase();

    /**
     * Prepare for practice using the entries in this document.
     */
    void setDocument(KEduVocDocument *doc);

    // Should be called when starting and ending the practice session respectively.
    void practiceStarted();
    void practiceFinished();

    /**
     * Get the next entry to show to the user.
     * @return TestEntry* the entry
     */
    TestEntry* nextTrainingEntry();

    /**
     * Get a list of all entries in the test - used by the summary dialog
     */
    QList<TestEntry*> allTestEntries() {
        return m_allTestEntries;
    }

    /**
     * The number of entries available for the practice session.
     * The actual session may be smaller.
     * @return
     */
    int allEntryCount();

    /**
     * The number of entries that are still to be practiced
     * @return
     */
    int activeEntryCount();

    /**
     * Puts some grades on the shell
     */
    void printStatistics();

    /** Finish the currently active entry */
    void removeCurrentEntryFromPractice();

    /** Finish the given entry */
    //void entryFinished(TestEntry* entry);

    int statisticTotalCorrectFirstAttempt();
    int statisticTotalWrong();
    int statisticTotalUnanswered();

    /** the time in seconds */
    int totalTime();

    QStringList multipleChoiceAnswers(int numberChoices);

    /**
    * Get a list of all unanswered entries in the test
    */
    QList<TestEntry*> allUnansweredTestEntries();
    QString currentConjugationTense();

    QString title() const;

private:
    /**
     * Select appropriate entries for the practice (respect blocking settings etc)
     * m_allTestEntries will be filled by this.
     */
    void filterTestEntries();

    /**
     * Find out if the given expression can be used as a multiple choice answer for the current entry
     * (i.e. if it's not the answer itself, not a synonym and has a different text)
     */
    bool isValidMultipleChoiceAnswer(KEduVocExpression *e);

    void setLanguages(int from, int to);

    KEduVocDocument *m_doc;
    QWidget *m_parent;
    int m_fromTranslation;
    int m_toTranslation;
    int m_testType;

    /// All entries in the test.
    QList<TestEntry*> m_allTestEntries;
    /// All entries that have not been asked.
    QList<TestEntry*> m_notAskedTestEntries;
    /// The list of entries that are being asked. If one of these is done, it can be deleted and an new one from m_notAskedTestEntries taken.
    QList<TestEntry*> m_currentEntries;
    int m_currentEntry;

    QTime m_time;
    int m_totalTime;

    KRandomSequence m_randomSequence;

    friend class EntryFilter;
};

}

#endif // kvtquery_included
