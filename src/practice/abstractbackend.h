/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_ABSTRACTBACKEND_H
#define PRACTICE_ABSTRACTBACKEND_H

#include "abstractfrontend.h"

#include <QtCore/qobject.h>

class QVariant;

namespace Practice {

class AbstractBackend : public QObject
{
    Q_OBJECT
public:
   
    AbstractBackend(AbstractFrontend *frontend, QObject *parent = 0);
    virtual ~AbstractBackend() {}
    
    /** The number of finished entries that will not be asked in this practice session again */
    virtual int practicedEntryCount() = 0;
    /** Total of entries to practice */
    virtual int totalEntryCount() = 0;
    /** The current grade of the entry */
    virtual int currentBox() = 0;
    /** The new grade */
    virtual int previousBox() = 0;
    /** Name of the lesson the currently practiced word is in */
    virtual QString lessonName() = 0;
    
public slots:
    virtual void nextEntry() = 0;

signals:
    /** something changed, update the frontend */
    void updateDisplay();

protected:
    AbstractFrontend *m_frontend;
};

}

#endif // PRACTICE_ABSTRACTBACKEND_H
