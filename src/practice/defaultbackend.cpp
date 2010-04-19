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

#include <QtCore/QVariant>
#include <kdebug.h>

#include "examplesentencebackendmode.h"
#include "flashcardbackendmode.h"
#include "genderbackendmode.h"
#include "multiplechoicebackendmode.h"
#include "writtenbackendmode.h"
#include "conjugationbackendmode.h"

using namespace Practice;

DefaultBackend::DefaultBackend(AbstractFrontend* frontend, ParleyDocument* doc, const Practice::PracticeOptions& options, Practice::TestEntryManager* testEntryManager, QObject* parent)
    : QObject(parent)
    , m_frontend(frontend)
    , m_document(doc)
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
            m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_testEntryManager);
            break;
        case Prefs::EnumPracticeMode::WrittenPractice:
            kDebug() << "Create Written Practice backend";
            m_frontend->setMode(AbstractFrontend::Written);
            m_mode = new WrittenBackendMode(m_options, m_frontend, this, m_testEntryManager);
            break;
        case Prefs::EnumPracticeMode::ExampleSentencesPractice:
            kDebug() << "Create Written Practice backend";
            m_frontend->setMode(AbstractFrontend::Written);
            m_mode = new ExampleSentenceBackendMode(m_options, m_frontend, this,m_testEntryManager);
            break;
        case Prefs::EnumPracticeMode::GenderPractice:
            m_frontend->setMode(AbstractFrontend::MultipleChoice);
            m_mode = new GenderBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        case Prefs::EnumPracticeMode::ConjugationPractice:
            m_frontend->setMode(AbstractFrontend::Conjugation);
            m_mode = new ConjugationBackendMode(m_options, m_frontend, this, m_testEntryManager, m_document->document());
            break;
        default:
            Q_ASSERT("Implement selected practice mode" == 0);
    }

    
    connect(m_mode, SIGNAL(currentEntryFinished()), this, SLOT(gradeCurrentEntry()));
    connect(m_mode, SIGNAL(gradeEntry(TestEntry*)), this, SLOT(gradeEntry(TestEntry*)));
    connect(m_mode, SIGNAL(nextEntry()), this, SLOT(nextEntry()));
    
    connect(m_frontend, SIGNAL(continueAction()), m_mode, SLOT(continueAction()));
    connect(m_frontend, SIGNAL(hintAction()), m_mode, SLOT(hintAction()));
}

void DefaultBackend::gradeEntry(TestEntry* entry)
{
    entry->incGoodCount();
    kDebug() << "Mark synonym" << entry->entry()->translation(m_options.languageTo())->text() << " correct";
    if(entry->answeredCorrectInSequence() == 3 ||
        (!Prefs::altLearn() && entry->answeredCorrectInSequence() == 1)) {
        m_testEntryManager->entryFinished(entry);
        kDebug() << "Removing synonym" << entry->entry()->translation(m_options.languageTo())->text() << " from practice";
        updateFrontend();
    }
}

void DefaultBackend::gradeCurrentEntry()
{
    kDebug() << "Result is " << m_frontend->resultState();
    if (m_frontend->resultState() == AbstractFrontend::AnswerCorrect) {
        m_current->incGoodCount();
        kDebug() << "Mark " << m_current->entry()->translation(m_options.languageTo())->text() << " correct";
        if(m_current->answeredCorrectInSequence() == 3 ||
            (!Prefs::altLearn() && m_current->answeredCorrectInSequence() == 1)) {
            removeCurrentEntryFromPractice();
            kDebug() << "Removing " << m_current->entry()->translation(m_options.languageTo())->text() << " from practice";
        }
    } else {
        kDebug() << "Mark " << m_current->entry()->translation(m_options.languageTo())->text() << " incorrect";
        m_current->incBadCount();
    }
    nextEntry();
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

    int grade = m_current->entry()->translation(m_options.languageTo())->grade();
    m_frontend->setBoxes(grade, qMin(grade+1, KV_MAX_GRADE), KV_LEV1_GRADE);

    QString imgUrl = m_current->entry()->translation(m_options.languageFrom())->imageUrl().url();
    m_frontend->setQuestionImage(imgUrl);
}

#include "defaultbackend.moc"
