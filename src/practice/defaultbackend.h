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
#include "writtenbackendmode.h"
#include "multiplechoicebackendmode.h"
#include "flashcardbackendmode.h"

#include "practice/testentrymanager.h"
#include "practiceold/testentry.h"
#include "practiceoptions.h"

#include "abstractfrontend.h"
#include "guifrontend.h"

namespace Practice {

class DefaultBackend :public QObject
{
    Q_OBJECT

public:
    DefaultBackend(AbstractFrontend* frontend, ParleyDocument* doc, const PracticeOptions& options, Practice::TestEntryManager* testEntryManager, QObject* parent = 0);
    ~DefaultBackend();
    
    void startPractice();
    
public Q_SLOTS:
    void nextEntry();
    void removeCurrentEntryFromPractice();

Q_SIGNALS:
    void practiceFinished();

private:
    AbstractFrontend::Mode nextPracticeMode(Practice::AbstractFrontend::Mode currentMode);
    void initializePracticeMode(AbstractFrontend::Mode mode);
    void updateFrontend();
    
    AbstractFrontend* m_frontend;
    PracticeOptions m_options;
    TestEntry* m_current;
    TestEntryManager* m_testEntryManager;
    AbstractFrontend::Mode m_currentMode;
    AbstractBackendMode* m_mode;
};

}

#endif
