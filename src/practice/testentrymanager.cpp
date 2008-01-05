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

// for the enum
#include "practicedialog.h"

#include <prefs.h>
#include <klocale.h>
#include <kconfig.h>

#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocdocument.h>
#include <keduvocwordtype.h>
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


TestEntryManager::TestEntryManager(KEduVocDocument* doc, QObject * parent)
    :QObject(parent)
{
    m_doc = doc;
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    m_testType = Prefs::testType();

    m_practiceTimeoutCounter = 0;

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

    m_randomSequence = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    ///@todo respect sub lessons and their inPractice state!
    // append lesson entries
    foreach ( KEduVocContainer *container, m_doc->lesson()->childContainers() ) {
        KEduVocLesson *lesson = static_cast<KEduVocLesson*>(container);
        if ( lesson->inPractice() ) {
            int lessonLimit = m_allTestEntries.count();
            foreach ( KEduVocExpression *entry, lesson->entriesRecursive() ) {
                if ( Prefs::testOrderLesson() ) {
                    // insert after the last entry of the last lesson
                    m_allTestEntries.insert(
                        lessonLimit + m_randomSequence->getLong(lessonLimit - m_allTestEntries.count()),
                        new TestEntry(entry) );
                } else {
                    // insert at total random position
                    m_allTestEntries.insert(
                        m_randomSequence->getLong(m_allTestEntries.count()),
                        new TestEntry(entry) );

                }
            }
        }
    }

    if ( m_allTestEntries.count() == 0 ) {
        if ( KMessageBox::questionYesNo(0, i18n("<p>The lessons you selected for the practice contain no vocabulary.</p><p>Hint: To select a lesson set a checkmark next to it in the lesson column on the left.</p><p>Would you like to include all lessons?</p>"), i18n("No Entries in Selected Lessons") ) == KMessageBox::Yes ) {
            kDebug() << "Adding all lessons.";
            ///@todo reuse the above - make it a function?
            foreach ( KEduVocContainer *container, m_doc->lesson()->childContainers() ) {
                KEduVocLesson *lesson = static_cast<KEduVocLesson*>(container);
                int lessonLimit = m_allTestEntries.count();
                foreach ( KEduVocExpression * entry, lesson->entries() ) {
                    if ( Prefs::testOrderLesson() ) {
                        // insert after the last entry of the last lesson
                        m_allTestEntries.insert(
                            lessonLimit + m_randomSequence->getLong(lessonLimit - m_allTestEntries.count()),
                            new TestEntry(entry) );
                    } else {
                        // insert at total random position
                        m_allTestEntries.insert(
                            m_randomSequence->getLong(m_allTestEntries.count()),
                            new TestEntry(entry) );

                    }
                }
            }
        } else {
            return;
        }
    }
    kDebug() << "Found " << m_allTestEntries.count() << " entries in selected lessons.";

    // remove empty entries
    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        if ( m_allTestEntries.value(i)->entry()->translation(TestEntry::gradeFrom())->text().isEmpty() ||
                m_allTestEntries.value(i)->entry()->translation(TestEntry::gradeTo())->text().isEmpty() ) {
            delete m_allTestEntries.takeAt(i);
        }
    }
    kDebug() << "Found " << m_allTestEntries.count() << " entries that are not empty.";

    // expire (decrease grade after a certain amount of time)
    expireEntries();


    QList <TestEntry *> removeTestEntryList;
    // word type
    int validWordType = 0;
    int validWrongCount = 0;
    int validPracticeCount = 0;
    int validGrade = 0;
    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        bool remove = false;
        const KEduVocGrade& grade =
            m_allTestEntries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation);
        if ( checkType(m_allTestEntries.value(i)->entry()) ) {
            validWordType++;
        } else { remove = true; }
        if ( grade.badCount() >= Prefs::practiceMinimumWrongCount() && grade.badCount() <= Prefs::practiceMaximumWrongCount() ) {
            validWrongCount++;
        } else { remove = true; }
        if ( grade.practiceCount() >= Prefs::practiceMinimumTimesAsked() && grade.practiceCount() <= Prefs::practiceMaximumTimesAsked() ) {
            validPracticeCount++;
        } else { remove = true; }
        if ( grade.grade() >= Prefs::practiceMinimumGrade() && grade.grade() <= Prefs::practiceMaximumGrade() ) {
            validGrade++;
        } else { remove = true; }
        if ( remove ) {
            removeTestEntryList.append(m_allTestEntries.value(i));
        }
    }
    kDebug() << "Valid Type: " << validWordType << " Valid Grade: " << validGrade
        << " Valid Wrong Count: " << validWrongCount << " Valid Practice Count: " << validPracticeCount;
    kDebug() << "Found " << removeTestEntryList.count() << " entries with invalid threshold.";

    if (validWordType == 0) {
        if (m_testType == Prefs::EnumTestType::ArticleTest) {
            KMessageBox::information(0,
                i18n("You selected to practice the genders of nouns, but no appropriate nouns could be found. Use \"Edit Entry\" and select Noun as word type and the gender."),
                i18n("No valid word type found"));
            return;
        }
        if (m_testType == Prefs::EnumTestType::ComparisonTest) {
            KMessageBox::information(0,
                i18n("You selected to practice comparison forms, but no adjectives or adverbs containing comparison forms could be found. Use \"Edit Entry\" and select Adverb or Adjective as word type and enter the comparison forms."),
                i18n("No valid word type found"));
            return;
        }
        if (m_testType == Prefs::EnumTestType::ConjugationTest) {
            KMessageBox::information(0, i18n("You selected to practice conjugations, but no vocabulary containing conjugations in the tenses you selected could be found. Use \"Edit Entry\" and select Verb as word type and enter the conjugation forms."), i18n("No valid word type found"));
            return;
        }
    }

    if ( removeTestEntryList.count() == m_allTestEntries.count() ) {
        if ( KMessageBox::questionYesNo(0, i18n("<p>The lessons you selected for the practice contain no entries when the threshold settings are respected.</p><p>Hint: To configure the thresholds use the \"Threshold Page\" in the \"Configure Practice\" dialog.</p><p>Would you like to ignore the threshold setting?</p>"), i18n("No Entries with Current Threshold Settings") ) == KMessageBox::No ) {
            return;
        }
    } else {
        foreach ( TestEntry* entry, removeTestEntryList ) {
            delete m_allTestEntries.takeAt(m_allTestEntries.indexOf(entry));
        }
    }

    // use the old validate methods for now
    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        if ( !validate(m_allTestEntries.value(i)->entry()) ) {
            delete m_allTestEntries.takeAt(i);
        }
    }

    kDebug() << "Found " << m_allTestEntries.count() << " entries after filtering.";

///@todo separate the tests to show better info here. take blocking etc into account for tests other than written/mc.

    

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



void TestEntryManager::startPractice()
{
    if ( m_currentEntries.count() == 0 ) {
        return;
    }

    switch ( Prefs::testType() ) {
    case Prefs::EnumTestType::WrittenTest:
        m_practiceDialog = new WrittenPracticeDialog(m_doc, 0);
        break;
    case Prefs::EnumTestType::MultipleChoiceTest:
        m_practiceDialog = new MCQueryDlg(m_doc, 0);
        break;
    case Prefs::EnumTestType::MixedLettersTest:
        m_practiceDialog = new MixedLetterPracticeDialog(m_doc, 0);
        break;
    case Prefs::EnumTestType::ArticleTest:
        m_practiceDialog = new ArtQueryDlg(m_doc, 0);
        break;
    case Prefs::EnumTestType::ComparisonTest:
        m_practiceDialog = new AdjQueryDlg(m_doc, 0);
        break;
    case Prefs::EnumTestType::ConjugationTest:
        m_practiceDialog = new VerbQueryDlg(m_doc, 0);
        break;
    // tests using the simple dialog
    case Prefs::EnumTestType::SynonymTest:
    case Prefs::EnumTestType::AntonymTest:
    case Prefs::EnumTestType::ExampleTest:
    case Prefs::EnumTestType::ParaphraseTest:
        m_practiceDialog = new SimpleQueryDlg(m_doc, 0);
        break;
    default:
        kError() << "unknown test type";
    }
    connect(m_practiceDialog, SIGNAL(signalResult(TestEntryManager::Result)), SLOT(slotResult(TestEntryManager::Result)));
    connect(m_practiceDialog, SIGNAL(showSolutionFinished()), SLOT(setNextEntry()));
    setNextEntry();
    m_practiceDialog->exec();
    m_practiceDialog->deleteLater();

    PracticeSummaryDialog practiceSummaryDialog(this, 0);
    practiceSummaryDialog.exec();
}




void TestEntryManager::expireEntries()
{
    if ( Prefs::expire() ) {
        int counter = 0;
        for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
            int grade = m_allTestEntries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation).grade();

            const QDateTime &date =  m_allTestEntries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation).practiceDate();

            const QDateTime &expireDate = QDateTime::currentDateTime().addSecs( -Prefs::expireItem(grade) );

            if ( date < expireDate && grade > 0) {
                // decrease the grade
                m_allTestEntries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation).decGrade();

                // prevent from endless dropping
                m_allTestEntries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation).setPracticeDate( QDateTime::currentDateTime().addSecs( -Prefs::expireItem( grade - 2) ) );
                counter++;
            }
        }
        kDebug() << "Expired words dropped their grade: " << counter;
    }
}



bool TestEntryManager::compareBlocking(int grade, const QDateTime &date, bool use_it)
{
    if (grade == KV_NORM_GRADE || Prefs::blockItem(grade) == 0 || !use_it) { // don't care || all off
        return true;
    } else {
        return date.addSecs(Prefs::blockItem(grade)) < QDateTime::currentDateTime();
    }
}



bool TestEntryManager::validateWithSettings(KEduVocExpression *expr)
{
    if ( !compareBlocking(expr->translation(m_toTranslation)->gradeFrom(m_fromTranslation).grade(), expr->translation(m_toTranslation)->gradeFrom(m_fromTranslation).practiceDate(), Prefs::block())) {
        return false;
    }
    return true;
}

bool TestEntryManager::validate(KEduVocExpression *expr)
{

    ///@todo word type, min/max asked/wrong/grade

    switch (m_testType) {
    case Prefs::EnumTestType::SynonymTest:
        return !expr->translation(m_toTranslation)->synonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::AntonymTest:
        return !expr->translation(m_toTranslation)->antonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ParaphraseTest:
        return !expr->translation(m_toTranslation)->paraphrase().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ExampleTest:
        return !expr->translation(m_toTranslation)->example().simplified().isEmpty();
        break;

    case Prefs::EnumTestType::ConjugationTest:
    case Prefs::EnumTestType::ArticleTest:
    case Prefs::EnumTestType::ComparisonTest:
    // already in checkType
        return true;
        break;

    default:
        if ( validateWithSettings(expr) ) {
            return true;
        }
        ///@todo not sure about swap dir stuff...
//         if (Prefs::swapDirection()) {
//             int temp = m_fromTranslation;
//             m_fromTranslation = m_toTranslation;
//             m_toTranslation = temp;
//             return validateWithSettings(expr);
//         } // swapDirection
//         break;
    }
    return false;
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

bool TestEntryManager::checkType(KEduVocExpression * entry)
{
///@todo for special practices - check for the right types...
    /*
    QString wordType = entry->translation(m_toTranslation)->type();
    QString subWordType = entry->translation(m_toTranslation)->subType();
    QString specialWordType = m_doc->wordTypes().specialType(wordType);
    QString specialSubType = m_doc->wordTypes().specialSubType(wordType,
        subWordType);

    // if we do a grammar test, check only if the grammar type is valid
    if ( Prefs::testType() == Prefs::EnumTestType::ArticleTest ) {
        if ( specialWordType == m_doc->wordTypes().specialTypeNoun() ) {
            return
                specialSubType ==
                m_doc->wordTypes().specialTypeNounMale() ||
                specialSubType ==
                m_doc->wordTypes().specialTypeNounFemale() ||
                specialSubType ==
                m_doc->wordTypes().specialTypeNounNeutral();
        }
        return false;
    }

    if ( Prefs::testType() == Prefs::EnumTestType::ComparisonTest ) {
        if ( Prefs::comparisonIncludeAdjective() ) {
            if ( specialWordType == m_doc->wordTypes().specialTypeAdjective() ) {
                return !entry->translation(m_toTranslation)->comparison().isEmpty();
            }
        }
        if ( Prefs::comparisonIncludeAdverb() ) {
            if ( specialWordType == m_doc->wordTypes().specialTypeAdverb() ) {
                return !entry->translation(m_toTranslation)->comparison().isEmpty();
            }
        }
        return false;
    }

    if ( Prefs::testType() == Prefs::EnumTestType::ConjugationTest ) {
        if ( specialWordType == m_doc->wordTypes().specialTypeVerb() ) {
            return entry->translation(m_toTranslation)->conjugations().count() > 0;
        }
        return false;
    }*/
    ///@todo respect subtypes for special practices above
    if (entry->translation(m_toTranslation)->wordType()) {
        return entry->translation(m_toTranslation)->wordType()->inPractice();
    }
    return false;
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

void TestEntryManager::createPracticeDialog()
{

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
