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

#ifndef kvtquery_included
#define kvtquery_included

#include "testentry.h"
#include "prefs.h"

#include <keduvocgrade.h>
#include <keduvocexpression.h>
#include <QVector>

class KEduVocDocument;
class Prefs;
class EnumCompType;
class KRandomSequence;

class TestEntryManager
{
public:
    /**
     * Create a collection of entries to be practiced.
     * @param doc
     */
    TestEntryManager(KEduVocDocument *doc);
    /**
     * Default ctor
     */
    ~TestEntryManager();

    /**
     * This is called to get the next entry to show to the user.
     * @return
     */
    TestEntry* nextEntry();

    /**
     * The user has answered with this result, internal update.
     * @param result the result (PracticeDialog)
     */
    void result(int result);

    int totalEntryCount();
    int activeEntryCount();


    /// grade int to string (?)
    static QString gradeStr(int i);

    void printStatistics();

private:

    /**
     * Check if @p expr is valid for the current query settings.
     * @param expr
     * @return
     */
    bool validate(KEduVocExpression *expr);
    bool validateWithSettings(KEduVocExpression *expr);
    bool compareBlocking(int grade, const QDateTime &limit, bool use_it);
    bool compareExpiring(int grade, const QDateTime &limit, bool use_it);
    bool compareDate(int type, const QDateTime &qd);
    bool compareQuery(int type, int qgrade, int limit);
    bool compareBad(int type, int bcount, int limit);
    bool compareGrade(int type, grade_t qgrade, grade_t limit);

    /**
     * Test if an entry has the right word type to be included in the query.
     * @param entry
     * @return
     */
    bool checkType(KEduVocExpression* entry);

    KEduVocDocument *m_doc;
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

    KRandomSequence* m_randomSequence;
};

#endif // kvtquery_included
