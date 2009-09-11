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

#include "abstractbackend.h"
#include "parleydocument.h"

#include "practiceold/testentrymanager.h"
#include "practiceold/testentry.h"

namespace Practice {

class DefaultBackend : public Practice::AbstractBackend
{
    Q_OBJECT

public:
    DefaultBackend(ParleyDocument *doc, QObject *parent = 0);
    virtual QString lessonName();
    virtual int previousBox();
    virtual int currentBox();
    virtual int totalEntryCount();
    virtual int practicedEntryCount();
    virtual QVariant question();
    virtual Practice::AbstractBackend::Mode mode();
    virtual QVariant solution();
    virtual bool acceptUserInput();

public slots:
    void continueAction(AbstractBackend::ContinueReason continueReason);
    void startPractice();
    
private:
    TestEntryManager m_testEntryManager;
    TestEntry* m_current;
};

}

#endif
