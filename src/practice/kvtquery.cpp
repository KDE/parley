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

#include "kvtquery.h"

#include "testentry.h"

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
#include <QDateTime>


///@todo rename this file and the .h



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

TestEntryManager::~ TestEntryManager()
{
    delete m_randomSequence;
}

TestEntryManager::TestEntryManager(KEduVocDocument* doc)
{
    m_doc = doc;
    m_fromTranslation = Prefs::fromIdentifier();
    m_toTranslation = Prefs::toIdentifier();
    m_testType = Prefs::testType();

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

    // append lesson entries
    foreach ( KEduVocLesson lesson, m_doc->lessons() ) {
        if ( lesson.inQuery() ) {
            int lessonLimit = m_allTestEntries.count();
            foreach ( int entryIndex, lesson.entries() ) {
                if ( Prefs::testOrderLesson() ) {
                    // insert after the last entry of the last lesson
                    m_allTestEntries.insert(
                        lessonLimit + m_randomSequence->getLong(lessonLimit - m_allTestEntries.count()),
                        new TestEntry(m_doc->entry(entryIndex), entryIndex) );
                } else {
                    // insert at total random position
                    m_allTestEntries.insert(
                        m_randomSequence->getLong(m_allTestEntries.count()),
                        new TestEntry(m_doc->entry(entryIndex), entryIndex) );

                }
            }
        }
    }

    kDebug() << "Found " << m_allTestEntries.count() << " entries in selected lessons.";

    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        if ( !checkType(m_allTestEntries.value(i)->exp) ) {
            delete m_allTestEntries.takeAt(i);
        }
    }
    kDebug() << "Found " << m_allTestEntries.count() << " entries with valid word type.";

    // use the old validate methods for now
    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        if ( !validate(m_allTestEntries.value(i)->exp) ) {
            delete m_allTestEntries.takeAt(i);
        }
    }

    kDebug() << "Found " << m_allTestEntries.count() << " entries after filtering.";

///@todo seperate the tests to show better info here. take blocking etc into account for tests other than written/mc.

    m_notAskedTestEntries = m_allTestEntries;

    for ( int i = 0; i < qMin(m_notAskedTestEntries.count(), Prefs::testNumberOfEntries() ); i++ ) {
        m_currentEntries.append( m_notAskedTestEntries.takeAt(0) );
    }

    m_currentEntry = 0;
}


bool TestEntryManager::compareBlocking(int grade, const QDateTime &date, bool use_it)
{
    if (grade == KV_NORM_GRADE || Prefs::blockItem(grade) == 0 || !use_it) // don't care || all off
        return true;
    else
        return date.addSecs(Prefs::blockItem(grade)) < QDateTime::currentDateTime();
}


bool TestEntryManager::compareExpiring(int grade, const QDateTime &date, bool use_it)
{
    if (grade == KV_NORM_GRADE || Prefs::expireItem(grade) == 0 || !use_it) // don't care || all off
        return false;
    else
        return date.addSecs(Prefs::expireItem(grade)) < QDateTime::currentDateTime();
}


bool TestEntryManager::compareDate(int type, const QDateTime &qd)
{
    QDateTime now = QDateTime::currentDateTime();
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::Before:
        erg = qd.toTime_t() == 0 || qd < now.addSecs(-Prefs::dateItem());
        break; // never queried or older date
    case Prefs::EnumCompType::Within:
        erg = qd >= now.addSecs(-Prefs::dateItem());
        break; // newer date
    case Prefs::EnumCompType::NotQueried:
        erg = qd.toTime_t() == 0;
        break;
    default:
        ;
    }
    return erg;
}


bool TestEntryManager::compareQuery(int type, int qgrade, int limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::MoreThan:
        erg = qgrade > limit;
        break; // sel has higher query count
    case Prefs::EnumCompType::MoreEqThan:
        erg = qgrade >= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = qgrade == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = qgrade != limit;
        break;
    case Prefs::EnumCompType::LessEqThan:
        erg = qgrade <= limit;
        break; // sel has less count
    case Prefs::EnumCompType::LessThan:
        erg = qgrade < limit;
        break;
    default:
        ;
    }
    return erg;
}


bool TestEntryManager::compareBad(int type, int bcount, int limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::MoreThan:
        erg = bcount > limit;
        break;   // sel has higher bad count
    case Prefs::EnumCompType::MoreEqThan:
        erg = bcount >= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = bcount == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = bcount != limit;
        break;
    case Prefs::EnumCompType::LessEqThan:
        erg = bcount <= limit;
        break;  // sel has less count
    case Prefs::EnumCompType::LessThan:
        erg = bcount < limit;
        break;
    default:
        ;
    }
    return erg;
}


bool TestEntryManager::compareGrade(int type, grade_t qgrade, grade_t limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::WorseThan:
        erg = qgrade < limit;
        break; // sel has worse grade
    case Prefs::EnumCompType::WorseEqThan:
        erg = qgrade <= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = qgrade == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = qgrade != limit;
        break;
    case Prefs::EnumCompType::BetterEqThan:
        erg = qgrade >= limit;
        break; // sel has better grade
    case Prefs::EnumCompType::BetterThan:
        erg = qgrade > limit;
        break;
    default:
        ;
    }
    return erg;
}


bool TestEntryManager::validateWithSettings(KEduVocExpression *expr)
{
    if(expr->translation(m_fromTranslation).text().simplified().isEmpty()) {
        return false;
    }
    if(expr->translation(m_toTranslation).text().simplified().isEmpty()) {
        return false;
    }

    // if expired, always take it
    if( compareExpiring(expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).grade(), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).queryDate(), Prefs::expire() ) ) {
        return true;
    }

    if ( !compareGrade(Prefs::compType(Prefs::EnumType::Grade), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).grade(), Prefs::gradeItem()) ) {
        return false;
    }

    if ( !compareQuery(Prefs::compType(Prefs::EnumType::Query), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).queryCount(), Prefs::queryItem())) {
        return false;
    }
    if ( !compareBad(Prefs::compType(Prefs::EnumType::Bad), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).badCount(), Prefs::badItem())) {
        return false;
    }
    if ( !compareDate(Prefs::compType(Prefs::EnumType::Date), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).queryDate())) {
        return false;
    }
    if ( !compareBlocking(expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).grade(), expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).queryDate(), Prefs::block())) {
        return false;
    }
    return true;
}

bool TestEntryManager::validate(KEduVocExpression *expr)
{
    switch (m_testType)
    {
    // The type queries so far do not consider any settings except lesson. So they return true as long as the type is right or there is data available.
    // This could be improved, but there are no open bugs concerning this atm.
    // So this is rather low priority.
    case Prefs::EnumTestType::SynonymTest:
        return !expr->translation(m_toTranslation).synonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::AntonymTest:
        return !expr->translation(m_toTranslation).antonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ParaphraseTest:
        return !expr->translation(m_toTranslation).paraphrase().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ExampleTest:
        return !expr->translation(m_toTranslation).example().simplified().isEmpty();
        break;

    case Prefs::EnumTestType::GrammarTest:
    // already in checkType
        return true;
        break;

    case Prefs::EnumTestType::WrittenTest: // Random and MC use the full settings:
    case Prefs::EnumTestType::MultipleChoiceTest:
        if ( validateWithSettings(expr) ) {
            return true;
        }
        ///@todo not sure about swap dir stuff...
        if (Prefs::swapDirection()) {
            int temp = m_fromTranslation;
            m_fromTranslation = m_toTranslation;
            m_toTranslation = temp;
            return validateWithSettings(expr);
        } // swapDirection
        break;

    default:
        kError() << "Trying to validate with unknown query type!" << endl;
    }
    return false;
}

TestEntry * TestEntryManager::nextEntry()
{
    // refill current entries
    while ( m_currentEntries.count() < Prefs::testNumberOfEntries() &&
            m_notAskedTestEntries.count() > 0 ) {
        m_currentEntries.append( m_notAskedTestEntries.takeAt(0) );
    }

    // return one of the current entries
    if ( m_currentEntries.count() > 0 ) {
        // one of the current words (by random)
        m_currentEntry = m_randomSequence->getLong(m_currentEntries.count());
        kDebug() << "nextEntry: " << m_currentEntry << " = " << m_currentEntries.value(m_currentEntry)->exp->translation(0).text() << " (" << m_currentEntries.count() + m_notAskedTestEntries.count() << "entries remaining)";
        return m_currentEntries.value( m_currentEntry );
    }
    return 0;
}


void TestEntryManager::result(int result)
{
kDebug() << "Result: " << result;
    // handle the result

    if ( result == PracticeDialog::StopIt ) {
        kDebug() << "Query stopped. Should not reach this function! TestEntryManager::result";
        return;
    }

    // update general stuff (count, date), unless the query has been stopped.
    m_doc->setModified();

    // change statistics, remove entry from test, if aplicable
    switch ( result ) {
    case PracticeDialog::Correct:
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

    case PracticeDialog::SkipKnown:
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

    case PracticeDialog::SkipUnknown:
        m_currentEntries[m_currentEntry]->incSkipUnknown();
        break;

    case PracticeDialog::Wrong:
        m_currentEntries[m_currentEntry]->incBadCount();
        break;

    case PracticeDialog::Timeout:
        m_currentEntries[m_currentEntry]->incTimeout();
        break;

    default :
        kError() << "Unknown result from QueryDlg\n";
    }
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
            << "Entry: " << entry->exp->translation(0).text();
    }
}

int TestEntryManager::totalEntryCount()
{
    return m_allTestEntries.count();
}

int TestEntryManager::activeEntryCount()
{
    return m_allTestEntries.count() - (m_notAskedTestEntries.count() + m_currentEntries.count());
}

bool TestEntryManager::checkType(KEduVocExpression * entry)
{
    QString wordType = entry->translation(m_toTranslation).type();
    QString specialWordType = m_doc->wordTypes()->specialType(wordType);
    QString specialSubType = m_doc->wordTypes()->specialSubType(wordType,
        entry->translation(m_toTranslation).subType());

    // if we do a grammar test, check only if the grammar type is valid
    if ( Prefs::testType() == Prefs::EnumTestType::GrammarTest ) {
        if ( Prefs::grammarArticleTest() ) {
            if ( specialWordType == m_doc->wordTypes()->specialTypeNoun() ) {
                return
                    specialSubType ==
                    m_doc->wordTypes()->specialTypeNounMale() ||
                    specialSubType ==
                    m_doc->wordTypes()->specialTypeNounFemale() ||
                    specialSubType ==
                    m_doc->wordTypes()->specialTypeNounNeutral();
            }
        }
        if ( Prefs::grammarComparisonAdjectiveTest() ) {
            if ( specialWordType == m_doc->wordTypes()->specialTypeAdjective() ) {
                return !entry->translation(m_toTranslation).comparison().isEmpty();
            }
        }
        if ( Prefs::grammarComparisonAdverbTest() ) {
            if ( specialWordType == m_doc->wordTypes()->specialTypeAdverb() ) {
                return !entry->translation(m_toTranslation).comparison().isEmpty();
            }
        }
        if ( Prefs::grammarConjugationTest() ) {
            if ( specialWordType == m_doc->wordTypes()->specialTypeVerb() ) {
                return entry->translation(m_fromTranslation).conjugations().count() > 0;
            }
        }
        return false;
    }

    switch (Prefs::compType(Prefs::EnumType::WordType)) {
    case Prefs::EnumCompType::DontCare:
        return true;
        break;
    case Prefs::EnumCompType::EqualTo:
        return (wordType == Prefs::typeItem());
        break;     // type is same
    case Prefs::EnumCompType::NotEqual:
        return (wordType != Prefs::typeItem());
        break;     // other type
    }
    return false;
}

