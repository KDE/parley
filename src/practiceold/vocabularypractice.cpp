/***************************************************************************
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


#include "vocabularypractice.h"

#include "testentry.h"
#include "writtenpracticedialog.h"
#include "MCQueryDlg.h"
#include "AdjQueryDlg.h"
#include "VerbQueryDlg.h"
#include "ArtQueryDlg.h"
#include "SimpleQueryDlg.h"
#include "mixedletterpracticedialog.h"
#include "practicesummarydialog.h"

#include <keduvocdocument.h>

VocabularyPractice::VocabularyPractice(KEduVocDocument *doc, QWidget * parent)
    :QObject(parent)
    ,m_testEntryManager(doc, parent)
    ,m_parent(parent)
    ,m_doc(doc)
    ,m_practiceTimeoutCounter(0)
{
    m_doc->setModified(true);
}

VocabularyPractice::~VocabularyPractice()
{}

void VocabularyPractice::startPractice()
{
    switch ( Prefs::testType() ) {
    case Prefs::EnumTestType::WrittenTest:
        m_practiceDialog = new WrittenPracticeDialog(m_doc, m_parent);
        break;
    case Prefs::EnumTestType::MultipleChoiceTest:
        m_practiceDialog = new MCQueryDlg(m_doc, m_parent);
        break;
    case Prefs::EnumTestType::MixedLettersTest:
        m_practiceDialog = new MixedLetterPracticeDialog(m_doc, m_parent);
        break;
    case Prefs::EnumTestType::ArticleTest:
        m_practiceDialog = new ArtQueryDlg(m_doc, m_parent);
        break;
    case Prefs::EnumTestType::ComparisonTest:
        m_practiceDialog = new AdjQueryDlg(m_doc, m_parent);
        break;
    case Prefs::EnumTestType::ConjugationTest:
        m_practiceDialog = new VerbQueryDlg(m_doc, m_parent);
        break;
    // tests using the simple dialog
    case Prefs::EnumTestType::SynonymTest:
    case Prefs::EnumTestType::AntonymTest:
    case Prefs::EnumTestType::ExampleTest:
    case Prefs::EnumTestType::ParaphraseTest:
        m_practiceDialog = new SimpleQueryDlg(m_doc, m_parent);
        break;
    default:
        kError() << "unknown test type";
    }
    connect(m_practiceDialog, SIGNAL(currentEntryFinished()), SLOT(entryDone()));

    nextEntry();
    m_practiceDialog->exec();

    PracticeSummaryDialog practiceSummaryDialog(&m_testEntryManager, m_parent);
    practiceSummaryDialog.exec();
}

void VocabularyPractice::entryDone()
{
    setResultForCurrent(m_practiceDialog->result());
    nextEntry();
}

void VocabularyPractice::nextEntry()
{
    m_currentEntry = m_testEntryManager.getNextEntry();
    if (m_currentEntry) {
        m_practiceDialog->setEntry(m_currentEntry);
        m_practiceDialog->setProgressCounter(
            m_testEntryManager.totalEntryCount() - m_testEntryManager.activeEntryCount(),
            m_testEntryManager.totalEntryCount());
    } else {
        // we're done, no more entries
        m_practiceDialog->accept();
    }
}

void VocabularyPractice::setResultForCurrent(VocabularyPractice::Result res)
{
    kDebug() << "result: " << res;
    m_doc->setModified();

    if ( res == Timeout ) {
        // too many timeouts in a row will hold the test
        if (++m_practiceTimeoutCounter >= MAX_PRACTICE_TIMEOUT) {
            const QString not_answered = i18n(
                "The test dialog was not answered several times in a row.\n"
                "It is assumed that there is currently nobody in front of "
                "the screen, and for that reason the practice is stopped.");

            KMessageBox::information(m_practiceDialog, not_answered, i18n("Stopping Test"));
        }
        return;
    } else {
        m_practiceTimeoutCounter = 0;
    }

    // check if stop was requested
    switch (res) {
    case StopPractice:
        m_practiceDialog->accept();
        break;
    case Correct:
        m_currentEntry->incGoodCount();
        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        if ( !Prefs::altLearn() ) {
            m_testEntryManager.currentEntryFinished();
        } else {
            // alt learn: 3 times right
            if ( m_currentEntry->answeredCorrectInSequence() >= 3 ) {
                m_testEntryManager.currentEntryFinished();
            }
        }
        break;
    case SkipKnown:
        m_currentEntry->incSkipKnown();
        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        if ( !Prefs::altLearn() ) {
            m_testEntryManager.currentEntryFinished();
        } else {
            // alt learn: 3 times right
            if ( m_currentEntry->answeredCorrectInSequence() >= 3 ) {
                m_testEntryManager.currentEntryFinished();
            }
        }
        break;
    case SkipUnknown:
        m_currentEntry->incSkipUnknown();
        break;
    case Wrong:
        m_currentEntry->incBadCount();
        break;
    case Timeout:
        m_currentEntry->incTimeout();
        break;
    }
}

#include "vocabularypractice.moc"
