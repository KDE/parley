/*
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sessionmanagerfixed.h"

// kdelibs
#include <KLocalizedString>
#include <KConfig>
#include <QDebug>

// kdeedulibs
#include <KEduVocExpression>

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
    QList<TestEntry*>::Iterator it = m_allTestEntries.begin();
    while (it != m_allTestEntries.end()
           && numNewWords < MaxNewWords
           && m_currentEntries.count() < MaxEntries)
    {
        if ((*it)->practiceModeDependentMinGrade() == 0
            && (*it)->practiceModeDependentMinPreGrade() == 0)
        {
            m_currentEntries.append(*it);
            numNewWords++;
            it = m_allTestEntries.erase(it);
        } else {
            ++it;
        }
    }

    // Pick the rest of the words from the already practiced ones.
    // Use higher graded entries before lower graded ones.
    for (int grade = KV_MAX_GRADE; grade > 0 ; --grade) {
        if (m_currentEntries.count() >= MaxEntries) {
            break;
        }

        // Step through all entries and collect those at the current
        // grade until the session is filled.
        it = m_allTestEntries.begin();
        while (it != m_allTestEntries.end()
               && m_currentEntries.count() < MaxEntries)
        {
            if ((*it)->practiceModeDependentMaxGrade() == grade) {
                m_currentEntries.append(*it);
                it = m_allTestEntries.erase(it);
            } else {
                ++it;
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
        it = m_allTestEntries.begin();
        while (it != m_allTestEntries.end()
            && m_currentEntries.count() < MaxEntries)
        {
            if ((*it)->practiceModeDependentMaxPreGrade() == preGrade) {
                m_currentEntries.append(*it);
                it = m_allTestEntries.erase(it);
            } else {
                ++it;
            }
        }
    }

    // The remaining entries have to be deleted to prevent memory leaking
    qDeleteAll(m_allTestEntries);

    // Now we have decided exactly which ones to use.
    // We need to keep this for statistics reporting at the end.
    m_allTestEntries = m_currentEntries;
}
