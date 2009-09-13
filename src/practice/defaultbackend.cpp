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

#include <kdebug.h>

#include <QtCore/QVariant>

using namespace Practice;

DefaultBackend::DefaultBackend(AbstractFrontend* frontend, ParleyDocument* doc, const PracticeOptions& options, QObject* parent)
    : AbstractBackend(frontend, parent)
    , m_options(options)
    , m_testEntryManager(doc->document(), 0)
    , m_currentMode(AbstractFrontend::Written)
    , m_mode(0)
{
}

DefaultBackend::~DefaultBackend()
{
    delete m_mode;
}

void DefaultBackend::startPractice()
{
    createPracticeMode();
    kDebug() << "start: " << m_options.languageFrom() << m_options.languageTo();

    nextEntry();
}

void DefaultBackend::createPracticeMode()
{
    delete m_mode;
    
    QList<AbstractFrontend::Mode> modes = m_options.modes();
    // TODO: mode needs to change at some point...
    m_currentMode = modes.at(0);
    m_frontend->setMode(m_currentMode);
    
    
    switch(m_currentMode) {
        case AbstractFrontend::Written:
            kDebug() << "Create Written Practice";
            m_mode = new WrittenBackendMode(m_options, m_frontend, this);
            break;
        case AbstractFrontend::FlashCard:
            kDebug() << "Create Flash Card Practice";
            m_mode = new FlashCardBackendMode(m_options, m_frontend, this);
            break;
        default:
            Q_ASSERT("Implement selected Mode" == 0);
            break;
    }
    kDebug() << "practice mode: " << m_currentMode;

    connect(m_mode, SIGNAL(nextEntry()), this, SLOT(nextEntry()));
}

PracticeOptions* DefaultBackend::options()
{
    return &m_options;
}

void DefaultBackend::nextEntry()
{
    m_current = m_testEntryManager.getNextEntry();
    m_mode->setTestEntry(m_current);
    m_frontend->setFinishedWordsTotalWords(
        m_testEntryManager.totalEntryCount() - m_testEntryManager.activeEntryCount(), 
        m_testEntryManager.totalEntryCount());
    m_frontend->setLessonName(m_current->entry()->lesson()->name());
}



#include "defaultbackend.moc"
