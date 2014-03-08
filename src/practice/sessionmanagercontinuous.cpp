/***************************************************************************
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
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

#include "sessionmanagercontinuous.h"

// Qt
#include <QDateTime>

// kdelibs
#include <klocale.h>
#include <kconfig.h>
#include <KDebug>
#include <KMessageBox>

// kdeedulibs
#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocdocument.h>

// parley
#include "entryfilter.h"
#include <prefs.h>


using namespace Practice;

SessionManagerContinuous::SessionManagerContinuous(QWidget* parent)
    : SessionManagerBase(parent)
{
}

SessionManagerContinuous::~SessionManagerContinuous()
{
}

void SessionManagerContinuous::setDocument(KEduVocDocument* doc)
{
    SessionManagerBase::setDocument(doc);
}

#if 0 // Keep here because we are going to break out a method in the next commit
TestEntry* SessionManagerContinuous::nextTrainingEntry()
{
    // Refill current entries.
    while (m_currentEntries.count() < Prefs::testNumberOfEntries() &&
            m_notAskedTestEntries.count() > 0) {
        m_currentEntries.append(m_notAskedTestEntries.takeAt(0));
    }

    int lastEntry = m_currentEntry;
    // return one of the current entries
    if (m_currentEntries.count() > 0) {
        // one of the current words (by random)
        m_currentEntry = m_randomSequence.getLong(m_currentEntries.count());
        // do not allow to ask the same entry twice in a row
        if (m_currentEntries.count() > 1) {
            while (m_currentEntry == lastEntry) {
                m_currentEntry = m_randomSequence.getLong(m_currentEntries.count());
            }
        }

        kDebug() << "nextEntry: " << m_currentEntry << " = " << m_currentEntries.value(m_currentEntry)->entry()->translation(0)->text() << " (" << m_currentEntries.count() + m_notAskedTestEntries.count() << "entries remaining)";

        return m_currentEntries.value(m_currentEntry);
    } else {
        return 0;
    }
}
#endif
