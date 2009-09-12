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

#include <QVariant>

namespace Practice {
    
class AbstractBackendMode
{
public:
    AbstractBackendMode(const PracticeOptions& options) :m_options(options) {}
    virtual ~AbstractBackendMode() {}
    void setTestEntry(TestEntry* current) { m_current = current; }
    virtual QVariant question() = 0;
    virtual QVariant solution() = 0;
    virtual qreal verifyAnswer(const QVariant& answer) = 0;
    
protected:
    PracticeOptions m_options;
    TestEntry* m_current;
};

}

#endif // ABSTRACTBACKENDMODE_H
