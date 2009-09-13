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
