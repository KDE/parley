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

void SessionManagerContinuous::initializeTraining()
{
    // Store all entries that will be used later.
    m_notAskedTestEntries = m_allTestEntries;

    // Pick the first N entries into the active set. No particular prioritization is done.
    for (int i = 0; i < qMin(m_notAskedTestEntries.count(), Prefs::testNumberOfEntries()); ++i) {
        m_currentEntries.append(m_notAskedTestEntries.takeAt(0));
    }
}
