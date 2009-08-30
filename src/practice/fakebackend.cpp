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

#include "fakebackend.h"

#include <kdebug.h>

#include <QtCore/QVariant>

using namespace Practice;

QString FakeBackend::lessonName()
{
    return "foo";
}

int FakeBackend::previousBox()
{
    return 1;
}

int FakeBackend::currentBox()
{
    return 2;
}

int FakeBackend::progressTotal()
{
    return 42;
}

int FakeBackend::progress()
{
    return 7;
}

QVariant FakeBackend::question()
{
    return QVariant("tree");
}

Practice::AbstractBackend::State FakeBackend::state()
{
    return Practice::AbstractBackend::AskUser;
}

Practice::AbstractBackend::Mode FakeBackend::mode()
{
    return Practice::AbstractBackend::Written;
}

void FakeBackend::continueAction(AbstractFrontend::ContinueReason continueReason)
{
    kDebug() << "continue action" << continueReason;
}
