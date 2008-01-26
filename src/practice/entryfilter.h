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

class KEduVocExpression;
class KEduVocDocument;
class KDialog;

class EntryFilter
    :public QObject
{
    Q_OBJECT
public:
    EntryFilter(QObject * parent, KEduVocDocument *doc);
    ~EntryFilter();

    QList<KEduVocExpression*> entries();

private:
    /**
     * Called when starting a practice.
     * Looks if the time is up, if the work has been praced too long ago, it will drop in grade. 
     * Only if expiring is activated in prefs.
     */
    void expireEntries();

    void lessonEntries();
    void wordTypeEntries();
    void blockedEntries();
    void gradeEntries();
    void timesWrongEntries();
    void timesPracticedEntries();
    void minMaxGradeEntries();
    void updateTotal();
    
    /**
     * Remove entries that are empty or not of the right type for the specific test type
     */
    void cleanupInvalid();

private slots:
    void filterLesson(bool filter);
    
private:
    Ui::EntryFilter ui;

    QSet<KEduVocExpression*> m_entries;
    QSet<KEduVocExpression*> m_entriesLesson;
    QSet<KEduVocExpression*> m_entriesWordType;
    QSet<KEduVocExpression*> m_entriesBlocked;
    QSet<KEduVocExpression*> m_entriesGrade;
    QSet<KEduVocExpression*> m_entriesTimesWrong;
    QSet<KEduVocExpression*> m_entriesTimesPracticed;
    QSet<KEduVocExpression*> m_entriesMinMaxGrade;
    QSet<KEduVocExpression*> m_currentSelection;

    KEduVocDocument *m_doc;
    int m_fromTranslation;
    int m_toTranslation;

    KDialog *m_dialog;
};

#endif

