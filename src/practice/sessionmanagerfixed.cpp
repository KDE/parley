/***************************************************************************
    copyright     : (C) 2014      Inge Wallin <inge@lysator.liu.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sessionmanagerfixed.h"

// kdelibs
#include <klocale.h>
#include <kconfig.h>
#include <KDebug>

// kdeedulibs
#include <keduvocexpression.h>

// parley
#include <prefs.h>


using namespace Practice;

SessionManagerFixed::SessionManagerFixed(QWidget* parent)
    : SessionManagerBase(parent)
{
}

SessionManagerFixed::~SessionManagerFixed()
{
}


void SessionManagerFixed::initializeTraining()
{
#if 0 // FIXME: Use smaller numbers during debugging
    const int MaxEntries  = Prefs::testNumberOfEntries();
    const int MaxNewWords = 5;      // FIXME: Make this a config option. 
#else
    const int MaxEntries = 7;
    const int MaxNewWords = 3;
#endif

    // We will never add anything to the session after it's initialized.
    m_notAskedTestEntries.clear();

    // Pick N new words if there are any into the active set.
    int numNewWords = 0;
    int i = 0;
    while (i < m_allTestEntries.count() 
           && numNewWords < MaxNewWords
           && m_currentEntries.count() < MaxEntries)
    {
        if (m_allTestEntries.at(i)->entry()->translation(m_toTranslation)->grade() == 0) {
            m_currentEntries.append(m_allTestEntries.at(i));
            numNewWords++;
        }

        ++i;
    }

    // Pick the rest of the words from the already practiced ones.
    // Use higher graded entries before lower graded ones.
    for (int grade = KV_MAX_GRADE; grade > 0 ; --grade) {
        if (m_currentEntries.count() >= MaxEntries) {
            break;
        }

        // Step through all entries and collect those at the current
        // grade until the session is filled.
        foreach (TestEntry *entry, m_allTestEntries) {
            if (entry->entry()->translation(m_toTranslation)->grade() == grade) {
                m_currentEntries.append(entry);
            }
            if (m_currentEntries.count() >= MaxEntries) {
                break;
            }
        }
    }

    // Now we have decided exactly which ones to use.
    // We need to keep this for statistics reporting at the end.
    m_allTestEntries = m_currentEntries;
}
