/***************************************************************************

                             manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "testentrymanager.h"

#include "testentry.h"
#include "writtenpracticedialog.h"
#include "MCQueryDlg.h"
#include "AdjQueryDlg.h"
#include "VerbQueryDlg.h"
#include "ArtQueryDlg.h"
#include "SimpleQueryDlg.h"
#include "mixedletterpracticedialog.h"

#include "practicesummarydialog.h"

#include "entryfilter.h"

// for the enum
#include "practicedialog.h"

#include <prefs.h>
#include <klocale.h>
#include <kconfig.h>

#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocdocument.h>
#include <KRandomSequence>
#include <KMessageBox>
#include <QDateTime>

#define MAX_PRACTICE_TIMEOUT 3

// this has nothing really to do with the rest. stays here until it has a better home.
QString TestEntryManager::gradeStr(int i)
{
    switch (i) {
    case KV_NORM_GRADE:
        return i18n(KV_NORM_TEXT);
        break;
    case KV_LEV1_GRADE:
        return i18n(KV_LEV1_TEXT);
        break;
    case KV_LEV2_GRADE:
        return i18n(KV_LEV2_TEXT);
        break;
    case KV_LEV3_GRADE:
        return i18n(KV_LEV3_TEXT);
        break;
    case KV_LEV4_GRADE:
        return i18n(KV_LEV4_TEXT);
        break;
    case KV_LEV5_GRADE:
        return i18n(KV_LEV5_TEXT);
        break;
    case KV_LEV6_GRADE:
        return i18n(KV_LEV6_TEXT);
        break;
    case KV_LEV7_GRADE:
        return i18n(KV_LEV7_TEXT);
        break;
    default:
        return i18n(KV_LEV1_TEXT);
        break;
    }
}


TestEntryManager::TestEntryManager(KEduVocDocument* doc, QWidget * parent)
    :QObject(parent)
{
    m_doc = doc;
    m_parent = parent;
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    m_testType = Prefs::testType();

    m_practiceTimeoutCounter = 0;
    m_randomSequence = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    TestEntry::setGradeTo(m_toTranslation);

    // write grades of mono lingual tests into translation(i).gradeFrom(i)
    if ( m_testType == Prefs::EnumTestType::WrittenTest ||
        m_testType == Prefs::EnumTestType::MultipleChoiceTest) {
        TestEntry::setGradeFrom(m_fromTranslation);
    } else {
        TestEntry::setGradeFrom(m_toTranslation);
    }

    kDebug() << "Test from: " << m_doc->identifier(m_fromTranslation).name()
        << " to: " << m_doc->identifier(m_toTranslation).name();

    filterTestEntries();
    kDebug() << "Found " << m_allTestEntries.count() << " entries after filtering.";

    m_notAskedTestEntries = m_allTestEntries;

    for ( int i = 0; i < qMin(m_notAskedTestEntries.count(), Prefs::testNumberOfEntries() ); i++ ) {
        m_currentEntries.append( m_notAskedTestEntries.takeAt(0) );
    }

    m_currentEntry = 0;
}


TestEntryManager::~ TestEntryManager()
{
    delete m_randomSequence;
}


void TestEntryManager::filterTestEntries()
{
    EntryFilter filter(this, m_doc);
    QList<KEduVocExpression*> allEntries = filter.entries();

    if (allEntries.count() == 0) {
        return;
    }

    foreach (KEduVocExpression* entry, allEntries){
        m_allTestEntries.append(new TestEntry(entry));
    }
}

void TestEntryManager::startPractice()
{
    if ( m_currentEntries.count() == 0 ) {
        return;
    }
    if (m_parent) {
        m_parent->hide();
    }

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
    connect(m_practiceDialog, SIGNAL(signalResult(TestEntryManager::Result)), SLOT(slotResult(TestEntryManager::Result)));
    connect(m_practiceDialog, SIGNAL(showSolutionFinished()), SLOT(setNextEntry()));
    setNextEntry();
    m_practiceDialog->exec();

    if (m_parent) {
        m_parent->show();
    }

    PracticeSummaryDialog practiceSummaryDialog(this, m_parent);
    practiceSummaryDialog.exec();
}

void TestEntryManager::printStatistics()
{
    kDebug() << "Test statistics: ";
    foreach ( TestEntry* entry, m_allTestEntries ) {
        kDebug()
            << " asked: " << entry->statisticCount()
            << " +" << entry->statisticGoodCount() << " -" << entry->statisticBadCount()
            << " ->+" << entry->statisticSkipKnown() << " ->-" << entry->statisticSkipUnknown()
            << " time:" << entry->statisticTimeout()
            << "Entry: " << entry->entry()->translation(0)->text();
    }
}

int TestEntryManager::totalEntryCount()
{
    return m_allTestEntries.count();
}

int TestEntryManager::activeEntryCount()
{
    return m_notAskedTestEntries.count() + m_currentEntries.count();
}

int TestEntryManager::statisticTotalCorrectFirstAttempt()
{
    int count = 0;
    foreach(TestEntry* entry, m_allTestEntries) {
        if ( entry->statisticCorrectAtFirstAttempt() ) {
            count++;
        }
    }
    return count;
}

int TestEntryManager::statisticTotalWrong()
{
    int count = 0;
    foreach(TestEntry* entry, m_allTestEntries) {
        if ( entry->statisticBadCount() ) {
            count++;
        }
    }
    return count;
}

int TestEntryManager::statisticTotalUnanswered()
{
    int count = 0;
    foreach(TestEntry* entry, m_allTestEntries) {
        if ( entry->statisticCount() == 0 ) {
            count++;
        }
    }
    return count;
}

int TestEntryManager::statisticTotalSkipKnown()
{
    int count = 0;
    foreach(TestEntry* entry, m_allTestEntries) {
        if ( entry->statisticSkipKnown() ) {
            count++;
        }
    }
    return count;
}

int TestEntryManager::statisticTotalSkipUnknown()
{
    int count = 0;
    foreach(TestEntry* entry, m_allTestEntries) {
        if ( entry->statisticSkipUnknown() ) {
            count++;
        }
    }
    return count;
}


void TestEntryManager::slotResult(TestEntryManager::Result res)
{
kDebug() << "result: " << res;
    m_doc->setModified();

    // check if stop was requested
    if ( res == StopPractice ) {
        m_practiceDialog->accept();
        return;
    }

    if ( res == Timeout ) {
        // too many timeouts in a row will hold the test
        if (++m_practiceTimeoutCounter >= MAX_PRACTICE_TIMEOUT) {
            const QString not_answered = i18n(
                "The test dialog was not answered several times in a row.\n"
                "It is assumed that there is currently nobody in front of "
                "the screen, and for that reason the practice is stopped.");

            KMessageBox::information(m_practiceDialog, not_answered, i18n("Stopping Test"));
        }
    } else {
        m_practiceTimeoutCounter = 0;
    }

    // update general stuff (count, date), unless the practice has been stopped.
    m_doc->setModified();

    // change statistics, remove entry from test, if aplicable
    switch ( res ) {
    case Correct:
        m_currentEntries[m_currentEntry]->incGoodCount();
        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        if ( !Prefs::altLearn() ) {
            m_currentEntries.takeAt(m_currentEntry);
        } else {
            // alt learn: 3 times right
            if ( m_currentEntries[m_currentEntry]->answeredCorrectInSequence() >= 3 ) {
                m_currentEntries.takeAt(m_currentEntry);
            }
        }
        break;

    case SkipKnown:
        m_currentEntries[m_currentEntry]->incSkipKnown();
        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        if ( !Prefs::altLearn() ) {
            m_currentEntries.takeAt(m_currentEntry);
        } else {
            // alt learn: 3 times right
            if ( m_currentEntries[m_currentEntry]->answeredCorrectInSequence() >= 3 ) {
                m_currentEntries.takeAt(m_currentEntry);
            }
        }
        break;

    case SkipUnknown:
        m_currentEntries[m_currentEntry]->incSkipUnknown();
        break;

    case Wrong:
        m_currentEntries[m_currentEntry]->incBadCount();
        break;

    case Timeout:
        m_currentEntries[m_currentEntry]->incTimeout();
        break;

    default :
        kError() << "Unknown result from Practice Dialog.";
    }

//     printStatistics();
}


// get a new entry
void TestEntryManager::setNextEntry()
{
    // refill current entries
    while ( m_currentEntries.count() < Prefs::testNumberOfEntries() &&
            m_notAskedTestEntries.count() > 0 ) {
        m_currentEntries.append( m_notAskedTestEntries.takeAt(0) );
    }

    int lastEntry = m_currentEntry;
    // return one of the current entries
    if ( m_currentEntries.count() > 0 ) {
        // one of the current words (by random)
        m_currentEntry = m_randomSequence->getLong(m_currentEntries.count());
        // do not allow to ask the same entry twice in a row
        if ( m_currentEntries.count() > 1 ) {
            while ( m_currentEntry == lastEntry ) {
                m_currentEntry = m_randomSequence->getLong(m_currentEntries.count());
            }
        }

        kDebug() << "nextEntry: " << m_currentEntry << " = " << m_currentEntries.value(m_currentEntry)->entry()->translation(0)->text() << " (" << m_currentEntries.count() + m_notAskedTestEntries.count() << "entries remaining)";

        m_practiceDialog->setEntry(m_currentEntries.value(m_currentEntry));
        m_practiceDialog->setProgressCounter(
            totalEntryCount()-activeEntryCount(), totalEntryCount());
    } else {
        // we're done, no more entries
        m_practiceDialog->accept();
    }
}

#include "testentrymanager.moc"
