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

#include "RandomQueryDlg.h"
#include "MCQueryDlg.h"
#include "AdjQueryDlg.h"
#include "VerbQueryDlg.h"
#include "ArtQueryDlg.h"
#include "SimpleQueryDlg.h"

#include "entry-dialogs/EntryDlg.h"

#include "kvtquery.h"
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
    m_testType = Prefs::testType();

    m_testDialog->deleteLater();
    m_testDialog = 0;

    m_app->removeEntryDlg();
    num_queryTimeout = 0;

    TestEntry * entry = m_entryManager->nextEntry();
    if ( entry == 0 ) {
        kDebug() << "entry == 0";
        return;
    }

    m_app->hide();

    switch ( m_testType ) {
    case Prefs::EnumTestType::WrittenTest:
        m_testDialog = new RandomQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::MultipleChoiceTest:
        m_testDialog = new MCQueryDlg(m_doc, m_app);
        break;

    // tests using the simple dialog
    case Prefs::EnumTestType::SynonymTest:
    case Prefs::EnumTestType::AntonymTest:
    case Prefs::EnumTestType::ExampleTest:
    case Prefs::EnumTestType::ParaphraseTest:
        m_testDialog = new SimpleQueryDlg(m_doc, m_app);
        break;

    case Prefs::EnumTestType::ConjugationTest:
        m_testDialog = new VerbQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::ArticleTest:
        m_testDialog = new ArtQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::ComparisonAdjectiveTest:
        m_testDialog = new AdjQueryDlg(m_doc, m_app);
        break;
    default:
        kError() << "QueryManager::startQuery: unknown type\n";
        stopPractice();
        return;
    }

    m_testDialog->setEntry( entry );
    m_testDialog->initFocus();
    m_testDialog->setProgressCounter(m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());
    connect(m_testDialog, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(m_testDialog, SIGNAL(sigQueryChoice(PracticeDialog::Result)), this, SLOT(slotResult(PracticeDialog::Result)));
    m_testDialog->show();
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


void QueryManager::slotResult(PracticeDialog::Result res)
{
kDebug() << "result: " << res;
    m_doc->setModified();

    // handle the result (upgrade grades etc)
    m_entryManager->result(res);

    // check if stop was requested
    if ( res == PracticeDialog::StopIt ) {
        stopPractice();
        return;
    }

    if ( res == PracticeDialog::Timeout ) {
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
        stopPractice();
        return;
    }

    if (m_testDialog == 0) {
        kError() << "m_testDialog == 0\n";
        stopPractice();
        return;
    }

    m_testDialog->setEntry(entry);
    m_testDialog->setProgressCounter(m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());
    m_testDialog->initFocus();
}


void QueryManager::stopPractice()
{
kDebug() << "stopPractice";
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



