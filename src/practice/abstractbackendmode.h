/***************************************************************************
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

#ifndef ABSTRACTBACKENDMODE_H
#define ABSTRACTBACKENDMODE_H

#include "practiceold/testentry.h"
#include "practiceoptions.h"
#include "abstractfrontend.h"

#include <QVariant>

namespace Practice {
    
class AbstractBackendMode :public QObject
{
    Q_OBJECT
    
public:
    AbstractBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend *frontend, QObject *parent);
    virtual ~AbstractBackendMode() {}
    virtual void setTestEntry(TestEntry* current) { m_current = current; }
    
public Q_SLOTS:
    virtual void continueAction() = 0;
    virtual void hintAction() = 0;

Q_SIGNALS:
    void currentEntryFinished();
    void nextEntry();
    
protected:
    PracticeOptions m_practiceOptions;
    AbstractFrontend* m_frontend;
    TestEntry* m_current;
};

}

#endif // ABSTRACTBACKENDMODE_H
