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
    const int MaxEntries  = Prefs::sessionMaxSize();
    const int MaxNewWords = Prefs::sessionMaxNewWords();

    // We will never add anything to the session after it's initialized.
    m_notAskedTestEntries.clear();

    // Pick N new words if there are any into the active set.
    int numNewWords = 0;
    int i = 0;
    while (i < m_allTestEntries.count()
           && numNewWords < MaxNewWords
           && m_currentEntries.count() < MaxEntries)
    {
        TestEntry *it = m_allTestEntries.at(i);
        if (it->entry()->translation(it->languageTo())->grade() == 0
            && it->entry()->translation(it->languageTo())->preGrade() == 0) {
            m_currentEntries.append(it);
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
            if (entry->entry()->translation(entry->languageTo())->grade() == grade) {
                m_currentEntries.append(entry);
            }
            if (m_currentEntries.count() >= MaxEntries) {
                break;
            }
        }
    }

    // If there is still room in the session, pick the rest of the
    // words from the ones with pregrades.  Also here, use higher
    // graded entries before lower graded ones.
    for (int preGrade = KV_MAX_GRADE; preGrade > 0 ; --preGrade) {
        if (m_currentEntries.count() >= MaxEntries) {
            break;
        }

        // Step through all entries and collect those at the current
        // grade until the session is filled.
        foreach (TestEntry *entry, m_allTestEntries) {
            if (entry->entry()->translation(entry->languageTo())->preGrade() == preGrade) {
                m_currentEntries.append(entry);
            }
            if (m_currentEntries.count() >= MaxEntries) {
                break;
            }
        }
    }

    // Now we have decided exactly which ones to use.
    // We need to keep this for statistics reporting at the end.
    //
    // FIXME: Seems to be a memory leak here.
    m_allTestEntries = m_currentEntries;
}
