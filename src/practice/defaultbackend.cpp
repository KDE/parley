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

DefaultBackend::DefaultBackend(ParleyDocument* doc, const PracticeOptions& options, QObject* parent)
    : AbstractBackend(parent)
    , m_options(options)
    , m_testEntryManager(doc->document(), 0)
    , m_currentMode(Written)
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

    continueAction();
}

void DefaultBackend::createPracticeMode()
{
    delete m_mode;
    
    QList<Mode> modes = m_options.modes();
    
    // TODO: mode needs to change at some point...
    m_currentMode = modes.at(0);
    
    switch(m_currentMode) {
        case Written:
            kDebug() << "Create Written Practice";
            m_mode = new WrittenBackendMode(m_options);
            break;
        case FlashCard:
            kDebug() << "Create Flash Card Practice";
            m_mode = new FlashCardBackendMode(m_options);
            break;
        default:
            Q_ASSERT("Implement selected Mode" == 0);
            break;
    }
    kDebug() << "practice mode: " << m_currentMode;
    
    m_mode->setTestEntry(m_current);
    
    emit modeChanged(m_currentMode);
}

QString DefaultBackend::lessonName()
{
    return m_current->entry()->lesson()->name();
}

int DefaultBackend::previousBox()
{
    return m_current->entry()->translation(m_options.languageTo())->grade();
}

int DefaultBackend::currentBox()
{
    return m_current->entry()->translation(m_options.languageTo())->grade();
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
    return m_mode->question();
}

QString DefaultBackend::questionPronunciation()
{
    return m_current->entry()->translation(m_options.languageFrom())->pronunciation();
}

AbstractBackend::Mode DefaultBackend::mode()
{
    return m_currentMode;
}

QVariant DefaultBackend::solution()
{
    return m_mode->solution();
}

QString DefaultBackend::solutionPronunciation()
{
    return m_current->entry()->translation(m_options.languageTo())->pronunciation();
}

bool DefaultBackend::acceptUserInput()
{
    return true;
}

void DefaultBackend::continueAction()
{
    kDebug() << "continue: " << m_options.languageFrom() << m_options.languageTo();
    m_current = m_testEntryManager.getNextEntry();
    m_mode->setTestEntry(m_current);
    if (!m_current) {
        // practice done
    } else {
        emit updateDisplay();
    }
}

void DefaultBackend::hintAction()
{}

void DefaultBackend::skipAction()
{}


#include "defaultbackend.moc"
