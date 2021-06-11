/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sessionmanagercontinuous.h"

// Qt
#include <QDateTime>

// kdelibs
#include <KLocalizedString>
#include <KConfig>
#include <QDebug>
#include <KMessageBox>

// kdeedulibs
#include <KEduVocLesson>
#include <KEduVocDocument>

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
