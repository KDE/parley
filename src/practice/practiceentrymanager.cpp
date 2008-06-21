//
// C++ Implementation: testentrymanager
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

 /***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <KRandomSequence>
#include <KMessageBox>
#include <KApplication>
#include <KDebug>

#include "prefs.h"
#include "../entryfilter.h"

#include "practiceentrymanager.h"

PracticeEntryManager::PracticeEntryManager(QObject * parent)
        : QObject(parent),
        m_iter(QList<KEduVocExpression*>()) // it has no empty constructor. Annoying...
{
    m_entries = QList<KEduVocExpression*>();
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    m_testType = Prefs::testType();


}

void PracticeEntryManager::filterTestEntries()
{
    EntryFilter filter(this, m_doc);
    QList<KEduVocExpression*> m_entries = filter.entries();
}


void PracticeEntryManager::open(KEduVocDocument* doc)
{
    m_doc = doc;

    if (m_doc && m_doc->lesson())
    {
        m_entries = m_doc->lesson()->entries(KEduVocContainer::Recursive);
        kDebug() << "count " << m_doc->lesson()->entryCount(KEduVocContainer::Recursive);
        if (0 == m_doc->lesson()->entryCount(KEduVocContainer::Recursive))
        {
//            KMessageBox::error(0, i18n("The vocabulary document contains no entries."));
            kDebug() << "No entries in selected document -- aborting.";
            exit(1);

        }
        m_iter = m_entries;
        filterTestEntries();
        shuffle();
    }
    else
    {
        kDebug() << "bad" << m_doc << m_doc->lesson();
    }

}

const QString PracticeEntryManager::currentSolution() const
{
    return m_entry->translation(Prefs::solutionLanguage())->text();
}

void PracticeEntryManager::appendExpressionToList(KEduVocExpression* expr)
{
    m_entries.append(expr);
    // when this entry was emitted, this was decremented, so now that we
    // get it back, we'll increment to stay even.
    ++m_numberEntriesRemaining;
}

// We append incorrect entries to the end of the list, so taking the
// current size each time wouldn't be what we want.
// So we'll only use the original size.
int PracticeEntryManager::totalEntryCount() const
{
    static int original_size = m_entries.count();
    return original_size;
}

int PracticeEntryManager::activeEntryCount() const
{
    return m_numberEntriesRemaining;
}


void PracticeEntryManager::shuffle()
{
    KRandomSequence(0).randomize(m_entries);
}


void PracticeEntryManager::slotNewEntry()
{
    if (m_iter.hasNext())
    {
        m_entry = m_iter.next();
        KEduVocTranslation * original = m_entry->translation(Prefs::questionLanguage());
        kDebug() << original->text();

        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        emit signalNewText(original->text());
        emit signalNewImage(original->imageUrl());
        emit signalNewSound(original->soundUrl());

        emit signalExpressionChanged(m_entry);
        emit signalNewEntry();
        --m_numberEntriesRemaining;
    }
    else
    {
        emit signalSetFinished();
        kDebug() << "finished";
    }
}
