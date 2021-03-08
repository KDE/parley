/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ENTRYFILTER_H
#define ENTRYFILTER_H

#include <QList>
#include <QSet>
#include <prefs.h>

class KEduVocText;
class KEduVocTranslation;
class KEduVocExpression;
class KEduVocDocument;
class TestEntry;


class EntryFilter
    : public QObject
{
    Q_OBJECT
public:
    EntryFilter(KEduVocDocument *doc, QObject *parent);

    /**
     * Returns the list of test entries after filtering out invalid entries
     * according to the settings.
     *
     * @param showDialog if true, show a dialog that lets the user slacken the
     *                   search parameters if there is no selected entries
     *                   using the current settings.
     *
     * @return list of entries to practice
     */
    QList<TestEntry*> entries(bool showDialog = true);

private:
    void collectEntries(int setNo);

    /**
     * Called when starting a practice.
     * Looks if the time is up, if the work has been practiced too long ago, it will drop in grade.
     * Only if expiring is activated in prefs.
     */
    void expireEntries(int setNo);
    void setupFilteredEntries(int setNo);

    void lessonEntries(int setNo);
    void wordTypeEntries(int setNo);

    bool isBlocked(const KEduVocText* const grade) const;
    bool isConjugationBlocked(KEduVocTranslation* translation) const;
    void blockedEntries(int setNo);
    void timesWrongEntries(int setNo);
    void timesPracticedEntries(int setNo);
    void minMaxGradeEntries(int setNo);

    void updateCurrentSelection();

    /**
     * Remove entries that are empty or not of the right type for the specific test type
     */
    void cleanupInvalid(int setNo);

    /**
     * In conjugation practice mode, creates more than one test entry per verb if necessary.
     * This depends on the number of tenses and on the practice mode selected (every pronoun
     * separately, or grouped by tense).
     */
    QList<TestEntry*> conjugationTestEntries(bool ignoreBlocked) const;

    static void randomizedInsert(QList<TestEntry*>& list, TestEntry* entry);

private:
    KEduVocDocument *m_doc;

    // All entries that are valid given all criteria
    QSet<KEduVocExpression*> m_entries[2];

    // Various sets of entries that are valid given only one criterium each.
    // These are used in the selection dialog if there are no totally valid entries.
    QSet<KEduVocExpression*> m_entriesLesson[2];
    QSet<KEduVocExpression*> m_entriesWordType[2];
    QSet<KEduVocExpression*> m_entriesNotBlocked[2];
    QSet<KEduVocExpression*> m_entriesTimesWrong[2];
    QSet<KEduVocExpression*> m_entriesTimesPracticed[2];
    QSet<KEduVocExpression*> m_entriesMinMaxGrade[2];

    int m_numSets;
    int m_fromTranslation;
    int m_toTranslation;

    // The tenses selected by the user for practice
    QStringList m_tenses;

    QSet<KEduVocExpression*> m_currentSelection[2];

    friend class EntryFilterDialog;
};


#endif
