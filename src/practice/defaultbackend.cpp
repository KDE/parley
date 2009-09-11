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

#include "defaultbackend.h"

#include "prefs.h"

#include <kdebug.h>

#include <QtCore/QVariant>

using namespace Practice;

DefaultBackend::DefaultBackend(ParleyDocument *doc, QObject *parent)
    : AbstractBackend(parent)
    , m_testEntryManager(doc->document(), 0)
{
    m_languageFrom = Prefs::questionLanguage();
    m_languageTo = Prefs::solutionLanguage();
}

QString DefaultBackend::lessonName()
{
    return m_current->entry()->lesson()->name();
}

int DefaultBackend::previousBox()
{
    return m_current->entry()->translation(m_languageTo)->grade();
}

int DefaultBackend::currentBox()
{
    return m_current->entry()->translation(m_languageTo)->grade();
}

int DefaultBackend::totalEntryCount()
{
    return m_testEntryManager.totalEntryCount();
}

int DefaultBackend::practicedEntryCount()
{
    return m_testEntryManager.totalEntryCount() - m_testEntryManager.activeEntryCount();
}

QVariant DefaultBackend::question()
{
    
    return m_current->entry()->translation(m_languageFrom)->text();
}

Practice::AbstractBackend::Mode DefaultBackend::mode()
{
    return Practice::AbstractBackend::Written;
}

QVariant DefaultBackend::solution()
{
    return m_current->entry()->translation(m_languageTo)->text();
}

bool DefaultBackend::acceptUserInput()
{
    return true;
}


void DefaultBackend::continueAction(AbstractBackend::ContinueReason continueReason)
{
    kDebug() << "continue action" << continueReason;
}

void DefaultBackend::startPractice()
{
    kDebug() << "emitting modeChanged";
    
    m_current = m_testEntryManager.getNextEntry();
    emit modeChanged(Written);
    
}

#include "defaultbackend.moc"
