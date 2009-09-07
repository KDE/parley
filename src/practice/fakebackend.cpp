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

FakeBackend::FakeBackend(QObject *parent)
    : AbstractBackend(parent)
{

}

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
    return QVariant("house");
}

Practice::AbstractBackend::Mode FakeBackend::mode()
{
    return Practice::AbstractBackend::Written;
}

QVariant FakeBackend::solution()
{
    return QVariant("solution");
}

bool FakeBackend::acceptUserInput()
{
    return true;
}


void FakeBackend::continueAction(AbstractBackend::ContinueReason continueReason)
{
    kDebug() << "continue action" << continueReason;
}

void FakeBackend::startPractice()
{
    kDebug() << "emitting modeChanged";
    emit modeChanged(Written);
}

#include "fakebackend.moc"
