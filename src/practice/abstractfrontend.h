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

#ifndef PRACTICE_ABSTRACTFRONTEND_H
#define PRACTICE_ABSTRACTFRONTEND_H

#include <QtCore/qobject.h>
#include "abstractbackend.h"

namespace Practice {

class AbstractFrontend : public QObject
{
    Q_OBJECT
public:
    AbstractFrontend(AbstractBackend *backend, QObject* parent = 0);

    /**
     * Enables access to the input of the user.
     * This is queried by the backend when it needs to evaluate the input.
     */
    virtual QVariant userInput() = 0;

public slots:
    void updateDisplay();

signals:
    void continueAction(AbstractBackend::ContinueReason continueReason);
};

}

#endif // PRACTICE_ABSTRACTFRONTEND_H
