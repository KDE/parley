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

namespace Practice {

class AbstractBackend;

class AbstractFrontend : public QObject
{
    Q_OBJECT
public:
    AbstractFrontend(AbstractBackend *backend, QObject* parent = 0);

    enum ContinueReason { Default, AnswerLater, Hint };

    /**
     * Enables access to the input of the user.
     * This is queried
     */
    virtual QVariant userInput() = 0;

signals:
    void continueAction(ContinueReason continueReason);
};

}

#endif // PRACTICE_ABSTRACTFRONTEND_H
