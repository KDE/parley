/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_DEFAULTBACKEND_H
#define PRACTICE_DEFAULTBACKEND_H

#include "parleydocument.h"

#include "testentrymanager.h"
#include "testentry.h"
#include "practiceoptions.h"

#include "abstractfrontend.h"
#include "guifrontend.h"
#include "abstractbackendmode.h"

class ParleyDocument;

namespace Practice {
    

class DefaultBackend :public QObject
{
    Q_OBJECT

public:
    DefaultBackend(AbstractFrontend* frontend, ParleyDocument* doc, const PracticeOptions& options, Practice::TestEntryManager* testEntryManager, QObject* parent = 0);
    ~DefaultBackend();

    void startPractice();

public Q_SLOTS:
    /** Call when current entry is finished and next one should be selected */
    void nextEntry();

    void removeEntryFromPractice();
    
Q_SIGNALS:
    void practiceFinished();

private:
    void initializePracticeMode();
    void updateFrontend();
    
    AbstractFrontend* m_frontend;
    ParleyDocument* m_document;
    PracticeOptions m_options;
    TestEntry* m_current;
    TestEntryManager* m_testEntryManager;
    AbstractFrontend::Mode m_currentMode;
    AbstractBackendMode* m_mode;
};

}

#endif
