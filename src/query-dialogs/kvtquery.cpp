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

// for the enum
#include "QueryDlgBase.h"

#include <prefs.h>
#include <klocale.h>
#include <kconfig.h>

#include <keduvoclesson.h>
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

    m_randomSequence = new KRandomSequence( QDateTime().toTime_t() );

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

    // use the old validate methods for now
    for ( int i = m_allTestEntries.count() - 1; i >= 0; i-- ) {
        if ( !validateWithSettings(m_allTestEntries.value(i)->exp) ) {
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


bool TestEntryManager::compareType(int type, const QString & exprtype, const QString & wordtype)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = (exprtype == wordtype);
        break;     // type is same
    case Prefs::EnumCompType::NotEqual:
        erg = (exprtype != wordtype);
        break;     // other type
    default:
        break;
    }
    return erg;
}


// QuerySelection TestEntryManager::queryEntries()
// {
//     if (m_doc == 0) {
//         kError() << "TestEntryManager::queryEntries(): Cannot create query without source document set." << endl;
//     }
//
//     // initialize vector with (m_doc->lessonCount() + 1) elements
//     QuerySelection random(m_doc->lessonCount() + 1);
//
//     // choose entries that we like by using isActive and validate
//     //Note that Leitner style learning (altlearn) normally only uses 20
//     //entries, we just ignore that here
//     for (int i = 0; i < m_doc->entryCount(); i++) {
//         KEduVocExpression *expr = m_doc->entry(i);
//         if (expr->isActive()) {
//             if (validate(expr)) {
//                 int lessonNumber;
//                 if (Prefs::altLearn()) {
//                     lessonNumber = 0; //We only use a single array in Leitner style
//                 } else {
//                     lessonNumber = expr->lesson();
//                 }
//                 random[lessonNumber].append(QueryEntry(expr, i));
//
//                 kDebug() << " Add to query: lesson: " << expr->lesson() << " from translation: " << expr->translation(m_fromTranslation).text() << " grade: " << expr->translation(m_toTranslation).gradeFrom(m_fromTranslation).grade() << " grade (reversed): " << expr->translation(m_fromTranslation).gradeFrom(m_toTranslation).grade();
//             }
//         }
//     }
//
//     // remove empty lesson elements - backwards to not interfere with smaller indexes...
//     for (int i = (int) random.size()-1; i >= 0; i--)
//         if (random[i].size() == 0)
//             random.erase(random.begin() + i);
//
//     // vector of list (lessons) of entries
//     return random;
// }


bool TestEntryManager::validateWithSettings(KEduVocExpression *expr)
{
    // check type in both directions
    if ( !
        (compareType(Prefs::compType(Prefs::EnumType::WordType), expr->translation(m_toTranslation).type(), Prefs::typeItem())
        ||
        compareType(Prefs::compType(Prefs::EnumType::WordType), expr->translation(m_fromTranslation).type(), Prefs::typeItem()) )) {
        return false;
    }
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
kDebug() << "Adding expression to query: " << expr->translation(m_toTranslation).text();
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
        return !expr->translation(m_fromTranslation).synonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::AntonymTest:
        return !expr->translation(m_fromTranslation).antonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ParaphraseTest:
        return !expr->translation(m_fromTranslation).paraphrase().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ExampleTest:
        return !expr->translation(m_fromTranslation).example().simplified().isEmpty();
        break;

    case Prefs::EnumTestType::ArticleTest:
        if ( !(expr->translation(m_fromTranslation).type() == m_doc->wordTypes()->specialTypeNoun()) ) {
            return false;
        }
        return
            expr->translation(m_fromTranslation).subType() ==
            m_doc->wordTypes()->specialTypeNounMale() ||
            expr->translation(m_fromTranslation).subType() ==
            m_doc->wordTypes()->specialTypeNounFemale() ||
            expr->translation(m_fromTranslation).subType() ==
            m_doc->wordTypes()->specialTypeNounNeutral();
        break;
    case Prefs::EnumTestType::ComparisonAdjectiveTest:
        return  expr->translation(m_fromTranslation).type() ==  m_doc->wordTypes()->specialTypeAdjective() && !expr->translation(m_fromTranslation).comparison().isEmpty();
        break;
    case Prefs::EnumTestType::ComparisonAdverbTest:
        return  expr->translation(m_fromTranslation).type() == m_doc->wordTypes()->specialTypeAdverb() && !expr->translation(m_fromTranslation).comparison().isEmpty();
        break;
    case Prefs::EnumTestType::ConjugationTest:
        return (expr->translation(m_fromTranslation).type() == m_doc->wordTypes()->specialTypeVerb())
                && expr->translation(m_fromTranslation).conjugations().count() > 0;
        break;
        return false;
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
    if ( m_currentEntries.count() < Prefs::testNumberOfEntries() ) {
        if ( m_notAskedTestEntries.count() > 0 ) {
            m_currentEntries.append( m_notAskedTestEntries.takeAt(0) );
        }
    }

    // return one of the current entries
    if ( m_currentEntries.count() > 0 ) {
        // one of the current words (by random)
        m_currentEntry = m_randomSequence->getLong(m_currentEntries.count());
        kDebug() << "nextEntry: " << m_currentEntries.value(m_currentEntry)->exp->translation(0).text() << " (" << m_currentEntries.count() << "entries remaining)";
        return m_currentEntries.value( m_currentEntry );
    }
    return 0;
}



void TestEntryManager::result(int result)
{
kDebug() << "Result: " << result;
    // handle the result

    if ( result == QueryDlgBase::StopIt ) {
        kDebug() << "Query stopped. Should not reach this function! TestEntryManager::result";
        return;
    }

    // update general stuff (count, date), unless the query has been stopped.
    m_doc->setModified();

    // change statistics, remove entry from test, if aplicable
    switch ( result ) {
    case QueryDlgBase::Correct:
        m_currentEntries[m_currentEntry]->incGoodCount();
        ///@todo consider Leitner

        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        m_currentEntries.takeAt(m_currentEntry);
        break;

    case QueryDlgBase::SkipKnown:
        m_currentEntries[m_currentEntry]->incSkipKnown();
        ///@todo consider Leitner

        // takeAt but no delete, since we still have the pointer in m_allTestEntries!
        m_currentEntries.takeAt(m_currentEntry);
        break;

    case QueryDlgBase::SkipUnknown:
        m_currentEntries[m_currentEntry]->incSkipUnknown();
        break;

    case QueryDlgBase::Wrong:
        m_currentEntries[m_currentEntry]->incBadCount();
        break;

    case QueryDlgBase::Timeout:
        m_currentEntries[m_currentEntry]->incTimeout();
        break;

    default :
        kError() << "Unknown result from QueryDlg\n";
        return;
    }





    /*
        num_queryTimeout = 0;
        if (Prefs::altLearn()) {
            //we always store the current question in the random_expr
            //array, so delete it from there.
            random_expr1.erase(random_expr1.begin() + random_query_nr);

            //The user guessed right (or she actually knew the
            //answer). Move the exp up to next level.
            switch (query_cycle) {
            case 1:
                correct_1_times.append(queryEntry);
                break;
            case 2:
                correct_2_times.append(queryEntry);
                break;
            case 3:
                correct_3_times.append(queryEntry);
                break;
            case 4:
                //The user has answered correctly four times in a row. She is good!
                correct_4_times.append(queryEntry);

                query_num--;
                exp->translation(tindex).gradeFrom(oindex).incGrade();

                break;
            default:
                kError() << "You should not be able to answer correctly more than 4 times\n";
                stopQuery();
                return;
            }

            if (random_expr1.count() == 0 && correct_1_times.count() == 0 && correct_2_times.count() == 0 && correct_3_times.count() == 0) {
                stopQuery();
                return;
            }

        } else { //not Prefs::altLearn()
            query_num--;
            if (query_cycle <= 1) {
                exp->translation(tindex).gradeFrom(oindex).incGrade(); // incr grade only in first cycle
            } else {
                exp->translation(tindex).gradeFrom(oindex).setGrade(KV_LEV1_GRADE); // reset grade
            }
            random_expr1.erase(random_expr1.begin() + random_query_nr);
            if (!(random_expr1.count() != 0 || random_expr2.count() != 0 || queryList.count() != 0)) {
                stopQuery();
                return;
            }
        }
        break;*/










//         //When you get it wrong Leisner style, it ends up in the back of the line
//         if (Prefs::altLearn())
//             random_expr1.append(queryEntry);
//         else
//             random_expr2.append(queryEntry);
//









//     if (Prefs::altLearn()) {
//
//         if (correct_3_times.count() > 7 || (correct_3_times.count() > 0 && correct_2_times.count() == 0 && correct_1_times.count() == 0 && random_expr1.count() == 0)) {
//             QueryEntry t_qer = correct_3_times[0];
//             correct_3_times.erase(correct_3_times.begin());
//             random_query_nr = random_expr1.count();
//             random_expr1.append(t_qer);
//             query_cycle = 4;
//         } else if (correct_2_times.count() > 5 || (correct_2_times.count() > 0 && correct_1_times.count() == 0 && random_expr1.count() == 0)) {
//             QueryEntry t_qer = correct_2_times[0];
//             correct_2_times.erase(correct_2_times.begin());
//             random_query_nr = random_expr1.count();
//             random_expr1.append(t_qer);
//             query_cycle = 3;
//         } else if (correct_1_times.count() > 5 || (correct_1_times.count() > 0  && random_expr1.count() == 0)) {
//             QueryEntry t_qer = correct_1_times[0];
//             correct_1_times.erase(correct_1_times.begin());
//             random_query_nr = random_expr1.count();
//             random_expr1.append(t_qer);
//             query_cycle = 2;
//         } else {
//             //else we just pick from random_expr1 then
//             if (random_expr1.count() == 0) {
//                 stopQuery();
//                 return;
//             }
//             query_cycle = 1;
//
//             random_query_nr = m_randomSequence.getLong(random_expr1.count());
//         }

    // not Prefs::altLearn()

}

void TestEntry::incGoodCount()
{
    update();
    m_statisticGoodCount++;
    m_answeredCorrectInSequence++;
}

void TestEntry::incSkipKnown()
{
    update();
    m_statisticSkipKnown++;
    m_answeredCorrectInSequence++;
}

void TestEntry::incBadCount()
{
    update();
    m_statisticBadCount++;
    m_answeredCorrectInSequence = 0;
    exp->translation(Prefs::toIdentifier()).gradeFrom(Prefs::fromIdentifier()).incBadCount();
}

void TestEntry::incTimeout()
{
    update();
    m_statisticTimeout++;
    m_answeredCorrectInSequence = 0;
    exp->translation(Prefs::toIdentifier()).gradeFrom(Prefs::fromIdentifier()).incBadCount();
}

void TestEntry::incSkipUnknown()
{
    update();
    m_statisticSkipUnknown++;
    m_answeredCorrectInSequence = 0;
    exp->translation(Prefs::toIdentifier()).gradeFrom(Prefs::fromIdentifier()).incBadCount();
}


void TestEntry::update()
{
    exp->translation(Prefs::toIdentifier()).gradeFrom(Prefs::fromIdentifier()).incQueryCount();
    m_statisticCount++;
    exp->translation(Prefs::toIdentifier()).gradeFrom(Prefs::fromIdentifier()).setQueryDate( QDateTime::currentDateTime() );
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

int TestEntry::statisticCount()
{
    return m_statisticCount;
}

int TestEntry::statisticBadCount()
{
    return m_statisticBadCount;
}

int TestEntry::statisticSkipKnown()
{
    return m_statisticSkipKnown;
}

int TestEntry::statisticSkipUnknown()
{
    return m_statisticSkipUnknown;
}

int TestEntry::statisticTimeout()
{
    return m_statisticTimeout;
}

int TestEntry::statisticGoodCount()
{
    return m_statisticGoodCount;
}
