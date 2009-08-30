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

#ifndef PRACTICE_FAKEBACKEND_H
#define PRACTICE_FAKEBACKEND_H

#include "abstractbackend.h"


namespace Practice {

class FakeBackend : public Practice::AbstractBackend
{
    Q_OBJECT

public:
    virtual QString lessonName();
    virtual int previousBox();
    virtual int currentBox();
    virtual int progressTotal();
    virtual int progress();
    virtual QVariant question();
    virtual Practice::AbstractBackend::State state();
    virtual Practice::AbstractBackend::Mode mode();

public slots:
    void continueAction(AbstractFrontend::ContinueReason continueReason);
};

}

#endif // PRACTICE_FAKEBACKEND_H
