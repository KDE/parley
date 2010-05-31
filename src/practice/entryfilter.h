/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ENTRYFILTER_H
#define ENTRYFILTER_H

#include "ui_entryfilter.h"

#include <QtCore/QList>
#include <QtCore/QSet>
#include <prefs.h>

class KEduVocText;
class KEduVocExpression;
class KEduVocDocument;
class KDialog;
class TestEntry;

namespace Practice {

class EntryFilter
    :public QObject
{
    Q_OBJECT
public:
    EntryFilter(QObject * parent, KEduVocDocument *doc);

    /**
     * Returns the list of test entries after filtering out invalid entries according to the settings.
     * @return list of entries to practice
     */
    QList<TestEntry*> entries();

private:
    /**
     * Called when starting a practice.
     * Looks if the time is up, if the work has been praced too long ago, it will drop in grade. 
     * Only if expiring is activated in prefs.
     */
    void expireEntries();

    void lessonEntries();
    void wordTypeEntries();

    bool isBlocked(const KEduVocText* const grade) const;
    void blockedEntries();
    void timesWrongEntries();
    void timesPracticedEntries();
    void minMaxGradeEntries();
    void updateTotal();

    /**
     * Remove entries that are empty or not of the right type for the specific test type
     */
    void cleanupInvalid();

    QList<TestEntry*> conjugationTestEntries() const;

private slots:
    void checkBoxChanged(bool filter);

private:
    Ui::EntryFilter ui;

    QSet<KEduVocExpression*> m_entries;
    QSet<KEduVocExpression*> m_entriesLesson;
    QSet<KEduVocExpression*> m_entriesWordType;
    QSet<KEduVocExpression*> m_entriesNotBlocked;
    QSet<KEduVocExpression*> m_entriesTimesWrong;
    QSet<KEduVocExpression*> m_entriesTimesPracticed;
    QSet<KEduVocExpression*> m_entriesMinMaxGrade;
    QSet<KEduVocExpression*> m_currentSelection;

    KEduVocDocument *m_doc;
    int m_fromTranslation;
    int m_toTranslation;
    Prefs::EnumPracticeMode::type m_mode;
    QStringList m_tenses;

    KDialog *m_dialog;
};
}

#endif

