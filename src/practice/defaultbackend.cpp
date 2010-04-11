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
    , m_current(0)
    , m_testEntryManager(testEntryManager)
    , m_currentMode(AbstractFrontend::None)
    , m_mode(0)
{
    Q_UNUSED(doc)
    connect(m_frontend, SIGNAL(skipAction()), this, SLOT(nextEntry()));
    connect(m_frontend, SIGNAL(stopPractice()), this, SIGNAL(practiceFinished()));
}

DefaultBackend::~DefaultBackend()
{
    delete m_mode;
}

void DefaultBackend::startPractice()
{
    initializePracticeMode();
    nextEntry();
}

void DefaultBackend::initializePracticeMode()
{
    switch(m_options.mode()) {
        case Prefs::EnumPracticeMode::FlashCardsPractice:
            kDebug() << "Create Flash Card Practice backend";
            m_frontend->setMode(AbstractFrontend::FlashCard);
            m_mode = new FlashCardBackendMode(m_options, m_frontend, this);
            break;
        case Prefs::EnumPracticeMode::MultipleChoicePractice:
            kDebug() << "Create MultipleChoice Practice backend";
            m_frontend->setMode(AbstractFrontend::MultipleChoice);
            m_mode = new MultipleChoiceBackendMode(m_options, m_frontend, this, m_testEntryManager);
            break;
        case Prefs::EnumPracticeMode::MixedLettersPractice:
            kDebug() << "Create Mixed Letters Practice backend";
            m_frontend->setMode(AbstractFrontend::MixedLetters);
            m_mode = new WrittenBackendMode(m_options, m_frontend, this);
            break;
        case Prefs::EnumPracticeMode::WrittenPractice:
            kDebug() << "Create Written Practice backend";
            m_frontend->setMode(AbstractFrontend::Written);
            m_mode = new WrittenBackendMode(m_options, m_frontend, this);
            break;
        default:
            Q_ASSERT("Implement selected practice mode" == 0);
    }

    connect(m_mode, SIGNAL(currentEntryFinished()), this, SLOT(removeCurrentEntryFromPractice()));
    connect(m_mode, SIGNAL(nextEntry()), this, SLOT(nextEntry()));
    
    connect(m_frontend, SIGNAL(continueAction()), m_mode, SLOT(continueAction()));
    connect(m_frontend, SIGNAL(hintAction()), m_mode, SLOT(hintAction()));
}

void DefaultBackend::nextEntry()
{
    m_current = m_testEntryManager->getNextEntry();
    
    //after going through all words, or at the start of practice
    if (m_current == 0) {
        emit practiceFinished();
        return;
    }
    m_mode->setTestEntry(m_current);
    updateFrontend();
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
    m_frontend->setQuestionImage(imgUrl);
}

#include "defaultbackend.moc"
