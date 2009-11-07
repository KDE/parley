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

DefaultBackend::DefaultBackend(AbstractFrontend* frontend, ParleyDocument* doc, const Practice::PracticeOptions& options, Practice::TestEntryManager* testEntryManager, QObject* parent)
    : QObject(parent)
    , m_frontend(frontend)
    , m_options(options)
    , m_testEntryManager(testEntryManager)
    , m_currentMode(AbstractFrontend::None)
    , m_mode(0)
    , m_current(0)
{
    connect(m_frontend, SIGNAL(skipAction()), this, SLOT(nextEntry()));
    connect(m_frontend, SIGNAL(stopPractice()), this, SIGNAL(practiceFinished()));
}

DefaultBackend::~DefaultBackend()
{
    delete m_mode;
}

void DefaultBackend::startPractice()
{
    kDebug() << "start practice ... nextEntry";
    
    m_currentMode = nextPracticeMode(m_currentMode);
    if (m_currentMode == AbstractFrontend::None) {
        emit practiceFinished();
        return;
    }
    initializePracticeMode(m_currentMode);
    
    nextEntry();
}

void DefaultBackend::nextEntry()
{
    m_current = m_testEntryManager->getNextEntry();
    
    //after going through all words, or at the start of practice
    if (m_current == 0) {
        m_currentMode = nextPracticeMode(m_currentMode);
        
        // abort if no more modes available
        if (m_currentMode == AbstractFrontend::None) {
            emit practiceFinished();
            return;
        }
        
        initializePracticeMode(m_currentMode);
        // FIXME figure out, what to do with the grades - will only the very last mode update them?
        m_testEntryManager->startNextPracticeMode();
        
        m_current = m_testEntryManager->getNextEntry();
    }
    
    m_mode->setTestEntry(m_current);
    updateFrontend();
}

AbstractFrontend::Mode DefaultBackend::nextPracticeMode(AbstractFrontend::Mode currentMode)
{
    QList<AbstractFrontend::Mode> modes = m_options.modes();
    
    if (modes.size() == 0) {
        kWarning() << "No practice mode selected!";
        return AbstractFrontend::None;
    }
    int indexOfCurrent = modes.indexOf(currentMode);
    
    // m_currentMode is still None, it has not been initialized
    if (indexOfCurrent < 0) {
        return modes.at(0);
    }
    // all modes are done, return None
    if (indexOfCurrent == modes.size()-1) {
        return AbstractFrontend::None;
    }
    return modes.at(indexOfCurrent+1);
}

void DefaultBackend::initializePracticeMode(AbstractFrontend::Mode mode)
{
    delete m_mode;
    m_frontend->setMode(mode);
    
    switch(mode) {
        case AbstractFrontend::FlashCard:
            kDebug() << "Create Flash Card Practice";
            m_mode = new FlashCardBackendMode(m_options, m_frontend, this);
            break;
        case AbstractFrontend::MultipleChoice:
            kDebug() << "Create MultipleChoice Practice";
            m_mode = new MultipleChoiceBackendMode(m_options, m_frontend, this, m_testEntryManager);
            break;
        case AbstractFrontend::Written:
            kDebug() << "Create Written Practice";
            m_mode = new WrittenBackendMode(m_options, m_frontend, this);
            break;
        default:
            Q_ASSERT("Implement selected Mode" == 0);
            break;
    }

    connect(m_mode, SIGNAL(currentEntryFinished()), this, SLOT(removeCurrentEntryFromPractice()));
    connect(m_mode, SIGNAL(nextEntry()), this, SLOT(nextEntry()));
    
    connect(m_frontend, SIGNAL(signalContinueButton()), m_mode, SLOT(continueAction()));
    connect(m_frontend, SIGNAL(hintAction()), m_mode, SLOT(hintAction()));
}

void DefaultBackend::removeCurrentEntryFromPractice()
{
    m_testEntryManager->currentEntryFinished();
}

void DefaultBackend::updateFrontend()
{
    m_frontend->setLessonName(m_current->entry()->lesson()->name());
    m_frontend->setFinishedWordsTotalWords(
        m_testEntryManager->totalEntryCount() - m_testEntryManager->activeEntryCount(), 
        m_testEntryManager->totalEntryCount());

    m_frontend->setBoxes(m_current->entry()->translation(m_options.languageFrom())->grade());

    QString imgUrl = m_current->entry()->translation(m_options.languageFrom())->imageUrl().url();
    kDebug() << "Show image: " << imgUrl;
    m_frontend->setQuestionImage(imgUrl);
}

#include "defaultbackend.moc"
