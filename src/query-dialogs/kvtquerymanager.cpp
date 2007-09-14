/***************************************************************************

                       query parts of Parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

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

#include "kvtquerymanager.h"

#include "kvoctrain.h"

#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"

#include "entry-dialogs/EntryDlg.h"

#include "query-dialogs/kvtquery.h"
#include "prefs.h"

#include <keduvoclesson.h>

#include <KDebug>
#include <KLocale>
#include <KMessageBox>


#include <QString>

#define MAX_QUERY_TIMEOUT 3

static const char not_answered[] = I18N_NOOP(
                                       "The test dialog was not answered several times in a row.\n"
                                       "It is assumed that there is currently nobody in front of "
                                       "the screen, and for that reason the query is stopped.");


static const char not_contain[] = I18N_NOOP(
                                      "There are currently no suitable expressions for the "
                                      "test you started.\n"
                                      "There are several possible reasons for this; maybe you do not have "
                                      "any expressions for the type of practice you requested.\n"
                                      "Most likely, you should adjust your settings referring to "
                                      "thresholds and blocking values in the practice options:\n"
                                      "should the configuration dialog be invoked now?");



QueryManager::QueryManager(KVocTrainApp *app, KEduVocDocument *doc)
{
    m_app = app;
    m_doc = doc;

    m_entryManager = new TestEntryManager(m_doc);

    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
}


void QueryManager::startPractice()
{
kDebug() << "QueryManager::startPractice()";
    m_fromTranslation = Prefs::fromIdentifier();
    m_toTranslation = Prefs::toIdentifier();
    m_testType = Prefs::testType();

///@todo in general rather use deleteLater()

    switch (m_testType) {
    case Prefs::EnumTestType::WrittenTest:
        delete m_testDialog;
        m_testDialog = 0;
        startQuery();
        break;

    case Prefs::EnumTestType::MultipleChoiceTest:
        delete m_testDialog;
        m_testDialog = 0;
        startQuery();
        break;

    case Prefs::EnumTestType::ConjugationTest:
        delete m_testDialog;
        m_testDialog = 0;
        startTypeQuery();
        break;

    case Prefs::EnumTestType::ArticleTest:
        delete m_testDialog;
        m_testDialog = 0;
        startTypeQuery();
        break;

    case Prefs::EnumTestType::ComparisonAdjectiveTest:
        delete m_testDialog;
        m_testDialog = 0;
        startTypeQuery();
        break;

    case Prefs::EnumTestType::ComparisonAdverbTest:
        delete m_testDialog;
        m_testDialog = 0;
        startTypeQuery();
        break;

    // tests using the simple dialog
    case Prefs::EnumTestType::SynonymTest:
    case Prefs::EnumTestType::AntonymTest:
    case Prefs::EnumTestType::ExampleTest:
    case Prefs::EnumTestType::ParaphraseTest:
        delete m_testDialog;
        m_testDialog = 0;
        startPropertyQuery();
        break;

    default:
        kError() << "Unknown command" << endl;

    }
}


bool QueryManager::queryIsEmpty()
{
    int i;
    for ( i = 0; i < m_doc->lessonCount(); i++ ) {
        if ( m_doc->lesson(i).inQuery() ) {
            break;
        }
    }

    if ( i == m_doc->lessonCount() ) {
        KMessageBox::information(m_app, i18n("You have selected no lessons to be practiced. Please select at least one on the left."), i18n("Starting Test"));
        return true;
    }

///@todo handle empty test
//     if (query_startnum == 0) {
//         if (KMessageBox::Yes == KMessageBox::questionYesNo(m_app, i18n(not_contain), i18n("Starting Test")))
//             m_app->slotGeneralOptionsPage(5);
//         return true;
//     }
    return false;
}


void QueryManager::startPropertyQuery()
{
    m_app->removeEntryDlg();
    num_queryTimeout = 0;

    TestEntry* entry = m_entryManager->nextEntry();

    if (entry == 0) {
        return;
    }

    m_app->hide();

    m_testDialog = new SimpleQueryDlg(m_doc, m_app);
    m_testDialog->setQuery(entry);
    connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutProperty(QueryDlgBase::Result)));
    m_testDialog->initFocus();
    m_testDialog->show();
}


void QueryManager::slotTimeOutProperty(QueryDlgBase::Result res)
{
kDebug() << "implement me :(";

    if (m_testDialog == 0) {
        kError() << "m_testDialog == 0\n";
        stopQuery();
        return;
    }


    stopQuery();

//    FIXME: keep track of knowledge ?

//     m_doc->setModified();
//     switch (res) {
//     case QueryDlgBase::Timeout:
//         if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
//             stopQuery();
//             KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Test"));
//             return;
//         } else {
//             random_expr2.append(random_expr1[random_query_nr]);
//             random_expr1.erase(random_expr1.begin() + random_query_nr);
//         }
//         break;
//
//     case QueryDlgBase::Unknown :
//         num_queryTimeout = 0;
//         random_expr2.append(random_expr1[random_query_nr]);
//         random_expr1.erase(random_expr1.begin() + random_query_nr);
//         break;
//
//     case QueryDlgBase::Known :
//         num_queryTimeout = 0;
//         query_num--;
//
//         random_expr1.erase(random_expr1.begin() + random_query_nr);
//         if (random_expr1.count() != 0 || random_expr2.count() != 0 || queryList.count() != 0) {
//             //nothing
//         } else {
//             stopQuery();
//             return;
//         }
//         break;
//
//     case QueryDlgBase::StopIt :
//         num_queryTimeout = 0;
//         stopQuery();
//         return;
//         break;
//
//     default :
//         kError() << "unknown result from QueryDlg\n";
//         stopQuery();
//         return;
//     }
//
//     if (random_expr1.count() == 0) {
//         if (random_expr2.count() == 0 && queryList.count() == 0) {
//             stopQuery();
//             return;
//         }
//
//         if (random_expr2.count() != 0) {  // next cycle with current lesson
//             random_expr1 = random_expr2;
//             random_expr2.clear();
//             query_cycle++;
//         } else { // next lesson
//             query_cycle = 1;
//             random_expr1 = queryList[0];
//             queryList.erase(queryList.begin());
//         }
//     }
//
//     if (random_expr1.count() == 0) { // should not happen !!
//         kError() << "QueryManager::slotTimeProperty: random_expr1.count() == 0\n";
//         stopQuery();
//         return;
//     }
//
//     random_query_nr = m_randomSequence.getLong(random_expr1.count());
//
//     m_testDialog->setQuery(m_testType, random_expr1[random_query_nr].m_index, m_toTranslation, query_cycle, query_num, query_startnum, m_doc);
//     m_testDialog->initFocus();
//     m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::startTypeQuery()
{
    /// @todo merge this with startPropertyQuery (and possibly startQuery)

    m_app->removeEntryDlg();
    m_app->slotStatusMsg(i18n("Starting special practice..."));
    num_queryTimeout = 0;

    TestEntry* entry = m_entryManager->nextEntry();

    if (entry == 0) {
        return;
    }

    m_app->hide();

    if (m_testType == Prefs::EnumTestType::ConjugationTest) {
        m_testDialog = new VerbQueryDlg(m_doc, m_app);
        m_testDialog->setQuery(entry);
        m_testDialog->initFocus();
        connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
        connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        m_testDialog->show();
    } else if (m_testType == Prefs::EnumTestType::ArticleTest) {
        m_testDialog = new ArtQueryDlg(m_doc, m_app);
        m_testDialog->setQuery(entry);
        m_testDialog->initFocus();
        connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
        connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        m_testDialog->show();
    } else if (m_testType == Prefs::EnumTestType::ComparisonAdjectiveTest) {
        m_testDialog = new AdjQueryDlg(m_doc, m_app);
        m_testDialog->setQuery(entry);
        m_testDialog->initFocus();
        connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
        connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        m_testDialog->show();

        /// @todo implement an adverb query dialog. can we reuse m_testDialog ?
//     } else if (m_testType == KVTQuery::ComparisonAdverbQuery) {
//         m_testDialog = new AdjQueryDlg(m_doc, m_app);
//         m_testDialog->setQuery(random_expr1[random_query_nr].m_index, act_query_col, query_cycle, query_num, query_startnum, exp->translation(act_query_col).comparison());
//         m_testDialog->initFocus();
//         connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
//         connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
//         m_testDialog->show();
    } else {
        kError() << "QueryManager::slotTimeOutType: unknown type\n";
        stopQuery();
        return;
    }
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotTimeOutType(QueryDlgBase::Result res)
{
kDebug() << "implement me :(";
//    FIXME: keep track of knowledge ?
    stopQuery();
/*    m_doc->setModified();
    switch (res) {
    case QueryDlgBase::Timeout:
        if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
            stopQuery();
            KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Test"));
            return;
        } else {
            random_expr2.append(random_expr1[random_query_nr]);
            random_expr1.erase(random_expr1.begin() + random_query_nr);
        }
        break;

    case QueryDlgBase::Unknown :
        num_queryTimeout = 0;
        random_expr2.append(random_expr1[random_query_nr]);
        random_expr1.erase(random_expr1.begin() + random_query_nr);
        break;

    case QueryDlgBase::Known :
        num_queryTimeout = 0;
        query_num--;

        random_expr1.erase(random_expr1.begin() + random_query_nr);
        if (random_expr1.count() != 0 || random_expr2.count() != 0 || queryList.count() != 0) {
            // nothing
        } else {
            stopQuery();
            return;
        }
        break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        stopQuery();
        return;
        break;

    default :
        kError() << "unknown result from QueryDlg\n";
        stopQuery();
        return;
    }


    if (random_expr1.count() == 0) {
        if (random_expr2.count() == 0 && queryList.count() == 0) {
            stopQuery();
            return;
        }

        if (random_expr2.count() != 0) {  // next cycle with current lesson
            random_expr1 = random_expr2;
            random_expr2.clear();
            query_cycle++;
        } else { // next lesson
            query_cycle = 1;
            random_expr1 = queryList[0];
            queryList.erase(queryList.begin());
        }
    }
    m_app->hide();

    if (random_expr1.count() == 0) { // should not happen !!
        kError() << "QueryManager::slotTimeSpecial: random_expr1.count() == 0\n";
        stopQuery();
        return;
    }

    random_query_nr = m_randomSequence.getLong(random_expr1.count());
    KEduVocExpression *exp = random_expr1[random_query_nr].exp;

    if (m_testType == Prefs::EnumTestType::ConjugationTest) {
        if (m_testDialog == 0) {
            kError() << "m_testDialog == 0\n";
            stopQuery();
            return;
        }
        m_testDialog->setQuery(random_expr1[random_query_nr].m_index, m_toTranslation, query_cycle, query_num, query_startnum, exp,
                               m_doc->identifier(m_toTranslation).personalPronouns(), exp->translation(m_toTranslation).conjugations());

        m_testDialog->initFocus();
    } else if (m_testType == Prefs::EnumTestType::ArticleTest) {
        if (m_testDialog == 0) {
            kError() << "m_testDialog == 0\n";
            stopQuery();
            return;
        }
        m_testDialog->setQuery(random_expr1[random_query_nr].m_index, m_toTranslation, query_cycle, query_num, query_startnum, exp, m_doc->identifier(m_toTranslation).article());
        m_testDialog->initFocus();
    } else if (m_testType == Prefs::EnumTestType::ComparisonAdjectiveTest) {
        if (m_testDialog == 0) {
            kError() << "m_testDialog == 0\n";
            stopQuery();
            return;
        }
        m_testDialog->setQuery(random_expr1[random_query_nr].m_index, m_toTranslation, query_cycle, query_num, query_startnum, exp->translation(m_toTranslation).comparison());
        m_testDialog->initFocus();
    } else {
        kError() << "QueryManager::slotTimeOutType: unknown type\n";
        stopQuery();
        return;
    }
    m_app->slotStatusMsg(IDS_DEFAULT);*/
}


/**
 * Start regular type query
 */
void QueryManager::startQuery()
{
kDebug() << "QueryManager::startQuery()";

    m_app->removeEntryDlg();
    num_queryTimeout = 0;

    if (m_fromTranslation < 0 || m_toTranslation < 0) {
        kDebug() << "Invalid indetifiers for test! " << m_fromTranslation << ", " << m_toTranslation;
        return;
    }

    TestEntry * entry = m_entryManager->nextEntry();
    if ( entry == 0 ) {
        kDebug() << "entry == 0";
        return;
    }

    m_app->hide();

    if (m_testType == Prefs::EnumTestType::WrittenTest) {
        m_testDialog = new RandomQueryDlg(m_doc, m_app);
    } else if (m_testType == Prefs::EnumTestType::MultipleChoiceTest) {
        m_testDialog = new MCQueryDlg(m_doc, m_app);
    } else {
        kError() << "QueryManager::startQuery: unknown type\n";
        stopQuery();
        return;
    }

    m_testDialog->setQuery( entry );
    m_testDialog->initFocus();
    m_testDialog->setProgressCounter(m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());
    connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(m_testDialog, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotQueryExpressionResult(QueryDlgBase::Result)));
    m_testDialog->show();
}


void QueryManager::slotQueryExpressionResult(QueryDlgBase::Result res)
{
kDebug() << "result: " << res;
    m_doc->setModified();

    // handle the result (upgrade grades etc)
    m_entryManager->result(res);

    // check if stop was requested
    if ( res == QueryDlgBase::StopIt ) {
        stopQuery();
        return;
    }

    if ( res == QueryDlgBase::Timeout ) {
        // too many timeouts in a row will hold the test
        if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
            KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Test"));
        }
    } else {
        num_queryTimeout = 0;
    }

    // get a new entry
    TestEntry* entry = m_entryManager->nextEntry();
    if ( entry == 0 ) {
        stopQuery();
        return;
    }

    if (m_testDialog == 0) {
        kError() << "m_testDialog == 0\n";
        stopQuery();
        return;
    }

    m_testDialog->setQuery(entry);
    m_testDialog->setProgressCounter(m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());
    m_testDialog->initFocus();
}


void QueryManager::stopQuery()
{
kDebug() << "stopQuery";
    if (m_testDialog != 0)
        m_testDialog->deleteLater();
    if (m_testDialog != 0)
        m_testDialog->deleteLater();
    if (m_testDialog != 0)
        m_testDialog->deleteLater();
    if (m_testDialog != 0)
        m_testDialog->deleteLater();
    if (m_testDialog != 0)
        m_testDialog->deleteLater();
    if (m_testDialog != 0)
        m_testDialog->deleteLater();

    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;
    m_testDialog = 0;

    /// @todo something like: if ( Prefs::ShowStatistics ) {
        // Let's display some statistics:

//         showStatistics();
    //}

    m_entryManager->printStatistics();

    m_app->show();
    deleteLater();
}


// void QueryManager::showStatistics()
// {
//     /// @todo implement me
//     kDebug() << "STATISTICS:";
//     kDebug() << "Count - queryList: " << queryList.count()
//         << " random_expr1: " << random_expr1.count()
//         << " random_expr2: " << random_expr2.count()
//         << " correct_1_times: " << correct_1_times.count()
//         << " correct_2_times: " << correct_2_times.count()
//         << " correct_3_times: " << correct_3_times.count()
//         << endl;
//
//     for (int i = 0; i<random_expr1.count(); i++) {
//         kDebug() << "random_expr1 " << i << " " << random_expr1[i].exp->translation(0).text();
//     }
// }

void QueryManager::slotEditEntry(int row, int translation)
{
kDebug() << "slotEditEntry(int, int) from a query: " << row << ", " << translation;

    EntryDlg* entryDlg = new EntryDlg(0 , m_doc);

    QList<int> entry;
    entry.append(row);
    entryDlg->setData(entry, translation);
    entryDlg->exec();
}

#include "kvtquerymanager.moc"



