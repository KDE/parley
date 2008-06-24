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

#include "prefs.h"

#include <KDebug>

#include "practiceentry.h"
#include "entryfilter.h"
#include "testentrymanager.h"

TestEntryManager::TestEntryManager(QObject * parent)
        : QObject(parent),
        m_iter(QList<PracticeEntry*>()) // it has no empty constructor. Annoying...
{
    m_entries = QList<PracticeEntry*>();
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    m_testType = Prefs::testType();


}

TestEntryManager::~TestEntryManager()
{
    qDeleteAll(m_entries);
}

void TestEntryManager::filterTestEntries()
{
    EntryFilter filter(this, m_doc);
    QList<KEduVocExpression*> entries = filter.entries();
    foreach(entry : entries)
    {
        m_entries.append(new PracticeEntry(entry));
    }
}


void TestEntryManager::open(KEduVocDocument* doc)
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

const QString TestEntryManager::currentSolution() const
{
    return m_entry->translation(Prefs::solutionLanguage())->text();
}

int TestEntryManager::totalEntryCount() const
{
    return m_entries.count();
}

int TestEntryManager::activeEntryCount() const
{
    return totalEntryCount() - m_numberEntriesUsed;
}


void TestEntryManager::shuffle()
{
    KRandomSequence(0).randomize(m_entries);
}


void TestEntryManager::slotNewEntry()
{
    if (m_iter.hasNext())
    {
        m_entry = m_iter.next();
        KEduVocTranslation * original = m_entry->expression()->translation(Prefs::questionLanguage());
        kDebug() << original->text();

        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        emit signalNewText(original->text());
        emit signalNewImage(original->imageUrl());
        emit signalNewSound(original->soundUrl());

        emit signalEntryChanged(m_entry);

    }
    else
    {
        emit signalSetFinished();
        kDebug() << "finished";
    }
}
