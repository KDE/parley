/***************************************************************************

                       query parts of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include <QString>
#include <KDebug>
#include <KLocale>

#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"

#include "prefs.h"

#include "query-dialogs/kvtquery.h"

#define MAX_QUERY_TIMEOUT 3

static const char not_answered[] = I18N_NOOP(
                                       "The query dialog was not answered several times in a row.\n"
                                       "It is assumed that there is currently nobody in front of "
                                       "the screen, and for that reason the query is stopped.");


static const char not_contain[] = I18N_NOOP(
                                      "There are currently no suitable expressions for the "
                                      "query you started.\n"
                                      "There are several possible reasons for this; maybe you do not have "
                                      "any expressions for the type of query you requested.\n"
                                      "Most likely, you should adjust your settings referring to "
                                      "thresholds and blocking values in the query options:\n"
                                      "should the configuration dialog be invoked now?");



QueryManager::QueryManager(KVocTrainApp *app, KEduVocDocument *doc)
{
    m_app = app;
    m_doc = doc;

    random_expr1.clear();
    random_expr2.clear();
    queryList.clear();
    m_doc->queryIdentifier(act_query_org, act_query_trans);
    if (!act_query_org.isEmpty() && !act_query_trans.isEmpty()) {
        for (int i = 0; i < m_doc->entryCount(); i++) {
            KEduVocExpression *entry = m_doc->entry(i);
            if (entry->isInQuery()) {
                int less = entry->lesson();
                for (int l = (int) queryList.size(); l <= less; l++) {
                    QueryEntryList ref_vec;
                    queryList.append(ref_vec);
                }
                QueryEntry ref(entry, i);
                queryList[less].append(ref);
            }
        }
    }

    m_query.setLessonItems(m_doc->lessonsInQuery());

    // remove empty elements
    for (int i = (int) queryList.size()-1; i >= 0; i--)
        if (queryList[i].size() == 0) {
            queryList.erase(queryList.begin() + i);
        }

    query_cycle = 1;
    query_startnum = 0;
    for (int i = 0; i < (int) queryList.size(); i++)
        query_startnum += queryList[i].size();
    query_num = query_startnum;

    simpleQueryDlg = 0;
    mcQueryDlg = 0;
    verbQueryDlg = 0;
    randomQueryDlg = 0;
    adjQueryDlg = 0;
    artQueryDlg = 0;

}


void QueryManager::query(int command, int toTranslation, int fromTranslation)
{
    switch (command) {

    case START_QUERY:
        delete randomQueryDlg;
        randomQueryDlg = 0;
        m_queryType = KVTQuery::RandomQuery;
        slotStartQuery(fromTranslation ? m_doc->identifier(fromTranslation) : m_doc->originalIdentifier(),
                       toTranslation ? m_doc->identifier(toTranslation) : m_doc->originalIdentifier(), true);
        break;

    case START_MULTIPLE:
        delete mcQueryDlg;
        mcQueryDlg = 0;
        m_queryType = KVTQuery::MultipleChoiceQuery;
        slotStartQuery(fromTranslation ? m_doc->identifier(fromTranslation) : m_doc->originalIdentifier(),
                       toTranslation ? m_doc->identifier(toTranslation) : m_doc->originalIdentifier(), true);
        break;

    case START_VERB: {
        delete verbQueryDlg;
        verbQueryDlg = 0;
        m_queryType = KVTQuery::ConjugationQuery;
        slotStartTypeQuery(fromTranslation, QM_VERB);
    }
    break;

    case START_ARTICLE: {
        delete artQueryDlg;
        artQueryDlg = 0;
        m_queryType = KVTQuery::ArticlesQuery;
        slotStartTypeQuery(fromTranslation, QM_NOUN);
    }
    break;

    case START_ADJECTIVE: {
        delete adjQueryDlg;
        adjQueryDlg = 0;
        m_queryType = KVTQuery::ComparisonQuery;
        slotStartTypeQuery(fromTranslation, QM_ADJ);
    }
    break;

    case START_SYNONYM: {
        delete simpleQueryDlg;
        simpleQueryDlg = 0;
        slotStartPropertyQuery(fromTranslation, KVTQuery::SynonymQuery);
    }
    break;

    case START_ANTONYM: {
        delete simpleQueryDlg;
        simpleQueryDlg = 0;
        slotStartPropertyQuery(fromTranslation, KVTQuery::AntonymQuery);
    }
    break;

    case START_EXAMPLE: {
        delete simpleQueryDlg;
        simpleQueryDlg = 0;
        slotStartPropertyQuery(fromTranslation, KVTQuery::ExampleQuery);
    }
    break;

    case START_PARAPHRASE: {
        delete simpleQueryDlg;
        simpleQueryDlg = 0;
        slotStartPropertyQuery(fromTranslation, KVTQuery::ParaphraseQuery);
    }
    break;

    default:
        kError() << "Unknown command" << endl;

    }
}


bool QueryManager::queryIsEmpty()
{
/// @todo Make part of QueryManager!
    if (m_doc->lessonsInQuery().empty()) {
        KMessageBox::information(m_app, i18n("You have selected no lessons to be queried. Please select at least one on the left."), i18n("Starting Query"));
        return true;
    }

    if (query_startnum == 0) {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(m_app, i18n(not_contain), i18n("Starting Query")))
            m_app->slotGeneralOptionsPage(5);
        return true;
    }
    return false;
}


void QueryManager::slotStartPropertyQuery(int col, KVTQuery::QueryType property)
{
    m_app->removeEntryDlg();
    m_app->slotStatusMsg(i18n("Starting property query..."));
    m_queryType = property;
    num_queryTimeout = 0;
    act_query_col = col;


    m_app->prepareProgressBar();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    random_expr2.clear();
    queryList = m_query.select(m_doc, m_doc->currentLesson(), act_query_col, property);

    query_startnum = 0;
    if (queryList.count() > 0) {
        random_expr1 = queryList[0];
        queryList.erase(queryList.begin());
        query_startnum = random_expr1.count();
    }

    for (int i = 0; i < queryList.count(); i++) {
        int n = queryList[i].count();
        query_startnum += n;
    }
    query_num = query_startnum;

    QApplication::restoreOverrideCursor();
    m_app->removeProgressBar();
    query_cycle = 1;

    if (queryIsEmpty())
        return;

    m_app->hide();

    random_query_nr = m_randomSequence.getLong(random_expr1.count());

    simpleQueryDlg = new SimpleQueryDlg(m_doc, m_app);
    simpleQueryDlg->setQuery(m_queryType, random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, m_doc);
    connect(simpleQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
    connect(simpleQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutProperty(QueryDlgBase::Result)));
    simpleQueryDlg->initFocus();
    simpleQueryDlg->show();
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotTimeOutProperty(QueryDlgBase::Result res)
{
    if (simpleQueryDlg == 0) {
        kError() << "simpleQueryDlg == 0\n";
        slotStopQuery(true);
        return;
    }

//    FIXME: keep track of knowledge ?

    m_doc->setModified();
    switch (res) {
    case QueryDlgBase::Timeout:
        if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
            slotStopQuery(true);
            KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Query"));
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
            //nothing
        } else {
            slotStopQuery(true);
            return;
        }
        break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
        return;
        break;

    default :
        kError() << "unknown result from QueryDlg\n";
        slotStopQuery(true);
        return;
    }

    if (random_expr1.count() == 0) {
        if (random_expr2.count() == 0 && queryList.count() == 0) {
            slotStopQuery(true);
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

    if (random_expr1.count() == 0) { // should not happen !!
        kError() << "QueryManager::slotTimeProperty: random_expr1.count() == 0\n";
        slotStopQuery(true);
        return;
    }

    random_query_nr = m_randomSequence.getLong(random_expr1.count());

    simpleQueryDlg->setQuery(m_queryType, random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, m_doc);
    simpleQueryDlg->initFocus();
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotStartTypeQuery(int col, const QString & type)
{
    m_app->removeEntryDlg();
    m_app->slotStatusMsg(i18n("Starting special query..."));
    num_queryTimeout = 0;
    act_query_col = col;

    m_app->prepareProgressBar();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    random_expr2.clear();

    queryList = m_query.select(m_doc, m_doc->currentLesson(), act_query_col, type);

    query_startnum = 0;
    if (queryList.count() > 0) {
        random_expr1 = queryList[0];
        queryList.erase(queryList.begin());
        query_startnum = random_expr1.count();
    }
    for (int i = 0; i < queryList.count(); i++) {
        int n = queryList[i].count();
        query_startnum += n;
    }
    query_num = query_startnum;

    QApplication::restoreOverrideCursor();
    m_app->removeProgressBar();
    query_cycle = 1;

    if (queryIsEmpty())
        return;

    random_query_nr = m_randomSequence.getLong(random_expr1.count());
    KEduVocExpression *exp = random_expr1[random_query_nr].exp;

    m_app->hide();
    if (m_queryType == KVTQuery::ConjugationQuery) {
        verbQueryDlg = new VerbQueryDlg(m_doc, m_app);
        verbQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp,
                               m_doc->conjugation(act_query_col), exp->conjugation(act_query_col));
        verbQueryDlg->initFocus();
        connect(verbQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
        connect(verbQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        verbQueryDlg->show();
    } else if (m_queryType == KVTQuery::ArticlesQuery) {
        artQueryDlg = new ArtQueryDlg(m_doc, m_app);
        artQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc->article(act_query_col));
        artQueryDlg->initFocus();
        connect(artQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
        connect(artQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        artQueryDlg->show();
    } else if (m_queryType == KVTQuery::ComparisonQuery) {
        adjQueryDlg = new AdjQueryDlg(m_doc, m_app);
        adjQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp->comparison(act_query_col));
        adjQueryDlg->initFocus();
        connect(adjQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
        connect(adjQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
        adjQueryDlg->show();
    } else {
        kError() << "QueryManager::slotTimeOutType: unknown type\n";
        slotStopQuery(true);
        return;
    }
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotTimeOutType(QueryDlgBase::Result res)
{
//    FIXME: keep track of knowledge ?

    m_doc->setModified();
    switch (res) {
    case QueryDlgBase::Timeout:
        if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
            slotStopQuery(true);
            KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Query"));
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
            slotStopQuery(true);
            return;
        }
        break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
        return;
        break;

    default :
        kError() << "unknown result from QueryDlg\n";
        slotStopQuery(true);
        return;
    }


    if (random_expr1.count() == 0) {
        if (random_expr2.count() == 0 && queryList.count() == 0) {
            slotStopQuery(true);
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
        slotStopQuery(true);
        return;
    }

    random_query_nr = m_randomSequence.getLong(random_expr1.count());
    KEduVocExpression *exp = random_expr1[random_query_nr].exp;

    if (m_queryType == KVTQuery::ConjugationQuery) {
        if (verbQueryDlg == 0) {
            kError() << "verbQueryDlg == 0\n";
            slotStopQuery(true);
            return;
        }
        verbQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp,
                               m_doc->conjugation(act_query_col), exp->conjugation(act_query_col));

        verbQueryDlg->initFocus();
    } else if (m_queryType == KVTQuery::ArticlesQuery) {
        if (artQueryDlg == 0) {
            kError() << "artQueryDlg == 0\n";
            slotStopQuery(true);
            return;
        }
        artQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc->article(act_query_col));
        artQueryDlg->initFocus();
    } else if (m_queryType == KVTQuery::ComparisonQuery) {
        if (adjQueryDlg == 0) {
            kError() << "adjQueryDlg == 0\n";
            slotStopQuery(true);
            return;
        }
        adjQueryDlg->setQuery(random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp->comparison(act_query_col));
        adjQueryDlg->initFocus();
    } else {
        kError() << "QueryManager::slotTimeOutType: unknown type\n";
        slotStopQuery(true);
        return;
    }
    m_app->slotStatusMsg(IDS_DEFAULT);
}

void QueryManager::slotResumeQuery()
{
    m_queryType = KVTQuery::RandomQuery;
    slotRestartQuery();
}


void QueryManager::slotResumeQueryMC()
{
    m_queryType = KVTQuery::MultipleChoiceQuery;
    slotRestartQuery();
}


void QueryManager::slotRestartQuery()
{
    if (random_expr1.count() != 0) {
        queryList.insert(queryList.begin(), random_expr1);
        random_expr1.clear();
    }
    slotStartQuery(act_query_trans, act_query_org, false);
}

/**
 * Start regular type query
 * @param translang that is asked
 * @param orglang shown
 * @param create_new create query=true, resume=false
 */
void QueryManager::slotStartQuery(const QString & translang, const QString & orglang, bool create_new)
{

    m_app->removeEntryDlg();
    m_app->slotStatusMsg(i18n("Starting random query..."));
    num_queryTimeout = 0;

    //is translang ok?
    int tindex = m_doc->indexOfIdentifier(translang);
    if (tindex < 0)
        return;
    //the original language?
    int oindex = m_doc->indexOfIdentifier(orglang);
    if (oindex < 0)
        return;

    act_query_trans = translang;
    act_query_org = orglang;

    m_app->prepareProgressBar();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    random_expr2.clear();

    if (create_new || queryList.count() == 0)
        queryList = m_query.select(m_doc, m_doc->currentLesson(), oindex, tindex);

    query_startnum = 0;
    if (queryList.count() > 0) {
        random_expr1 = queryList[0];
        queryList.erase(queryList.begin());
        query_startnum = random_expr1.count();
    }
    for (int i = 0; i < queryList.count(); i++) {
        int n = queryList[i].count();
        query_startnum += n;
    }
    query_num = query_startnum;

    QApplication::restoreOverrideCursor();
    m_app->removeProgressBar();
    query_cycle = 1;

    if (queryIsEmpty())
        return;

    m_app->hide();

    random_query_nr = m_randomSequence.getLong(random_expr1.count());
    KEduVocExpression *exp = random_expr1[random_query_nr].exp;

    QString q_org;
    QString q_trans;

    if (oindex == 0) {  // usual: give original, ask for translation x
        q_org = exp->original();
        q_trans = exp->translation(tindex);
    } else {  // alternative: give translation x, ask for original
        q_org = exp->translation(oindex);
        q_trans = exp->original();
    }

    if (m_queryType == KVTQuery::RandomQuery) {
        randomQueryDlg = new RandomQueryDlg(m_doc, m_app);
        randomQueryDlg->setQuery(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum);
        randomQueryDlg->initFocus();
        connect(randomQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
        connect(randomQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutRandomQuery(QueryDlgBase::Result)));
        randomQueryDlg->show();
    } else if (m_queryType == KVTQuery::MultipleChoiceQuery) {
        mcQueryDlg = new MCQueryDlg(m_doc, m_app);
        mcQueryDlg->setQuery(q_org, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, m_doc);
        mcQueryDlg->initFocus();
        connect(mcQueryDlg, SIGNAL(sigEditEntry(int,int)), m_app, SLOT(slotEditEntry(int,int)));
        connect(mcQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutMultipleChoice(QueryDlgBase::Result)));
        mcQueryDlg->show();
    } else {
        kError() << "QueryManager::slotStartQuery: unknown type\n";
        slotStopQuery(true);
        return;
    }
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotTimeOutRandomQuery(QueryDlgBase::Result res)
{
    m_queryType = KVTQuery::RandomQuery;
    slotTimeOutQuery(res);
}


void QueryManager::slotTimeOutMultipleChoice(QueryDlgBase::Result res)
{
    m_queryType = KVTQuery::MultipleChoiceQuery;
    slotTimeOutQuery(res);
}


void QueryManager::slotTimeOutQuery(QueryDlgBase::Result res)
{
    m_doc->setModified();

    int tindex = m_doc->indexOfIdentifier(act_query_trans);
    int oindex = m_doc->indexOfIdentifier(act_query_org);
    QueryEntry qer = random_expr1[random_query_nr];
    KEduVocExpression *exp = qer.exp;

    if (res != QueryDlgBase::StopIt) {
        m_doc->setModified();

        if (oindex == 0) {
            exp->incQueryCount(tindex, false);
            exp->setQueryDate(tindex, QDateTime::currentDateTime(), false);
        } else {
            exp->incQueryCount(oindex, true);
            exp->setQueryDate(oindex, QDateTime::currentDateTime(), true);
        }
    }

    switch (res) {
    case QueryDlgBase::Timeout:
        if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
            slotStopQuery(true);
            KMessageBox::information(m_app, i18n(not_answered), i18n("Stopping Query"));
            return;
        } else {
            random_expr1.erase(random_expr1.begin() + random_query_nr);

            //When you get it wrong Leitner style, it ends up in the back of the line
            if (Prefs::altLearn())
                random_expr1.append(qer);
            else
                random_expr2.append(qer);

            if (oindex == 0) {
                exp->incBadCount(tindex, false);
                exp->setGrade(tindex, KV_LEV1_GRADE, false);  // unknown: reset grade
            } else {
                exp->incBadCount(oindex, true);
                exp->setGrade(oindex, KV_LEV1_GRADE, true);
            }
        }
        break;

    case QueryDlgBase::Unknown :
        num_queryTimeout = 0;
        random_expr1.erase(random_expr1.begin() + random_query_nr);

        //When you get it wrong Leisner style, it ends up in the back of the line
        if (Prefs::altLearn())
            random_expr1.append(qer);
        else
            random_expr2.append(qer);

        if (oindex == 0) {
            exp->incBadCount(tindex, false);
            exp->setGrade(tindex, KV_LEV1_GRADE, false);  // unknown: reset grade
        } else {
            exp->incBadCount(oindex, true);
            exp->setGrade(oindex, KV_LEV1_GRADE, true);
        }
        break;

    case QueryDlgBase::Known :
        num_queryTimeout = 0;
        if (Prefs::altLearn()) {
            //we always store the current question in the random_expr
            //array, so delete it from there.
            random_expr1.erase(random_expr1.begin() + random_query_nr);

            //The user guessed right (or she actually knew the
            //answer). Move the exp up to next level.
            switch (query_cycle) {
            case 1:
                correct_1_times.append(qer);
                break;
            case 2:
                correct_2_times.append(qer);
                break;
            case 3:
                correct_3_times.append(qer);
                break;
            case 4:
                //The user has answered correctly four times in a row. She is good!
                exp->setInQuery(false);

                query_num--;
                if (oindex == 0) {
                    exp->incGrade(tindex, false);
                } else {
                    exp->incGrade(oindex, true);
                }
                break;
            default:
                kError() << "You should not be able to answer correctly more than 4 times\n";
                slotStopQuery(true);
                return;
            }

            if (random_expr1.count() == 0 && correct_1_times.count() == 0 && correct_2_times.count() == 0 && correct_3_times.count() == 0) {
                slotStopQuery(true);
                return;
            }

        } else { //not Prefs::altLearn()
            query_num--;
            if (query_cycle <= 1) {
                if (oindex == 0) {
                    exp->incGrade(tindex, false); // incr grade only in first cycle
                } else {
                    exp->incGrade(oindex, true);
                }
            } else {
                if (oindex == 0) {
                    exp->setGrade(tindex, KV_LEV1_GRADE, false); // reset grade
                } else {
                    exp->setGrade(oindex, KV_LEV1_GRADE, true);
                }
            }
            exp->setInQuery(false);
            random_expr1.erase(random_expr1.begin() + random_query_nr);
            if (!(random_expr1.count() != 0 || random_expr2.count() != 0 || queryList.count() != 0)) {
                slotStopQuery(true);
                return;
            }
        }
        break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
        return;
        break;

    default :
        kError() << "unknown result from QueryDlg\n";
        slotStopQuery(true);
        return;
    }

    if (Prefs::altLearn()) {

        if (correct_3_times.count() > 7 || (correct_3_times.count() > 0 && correct_2_times.count() == 0 && correct_1_times.count() == 0 && random_expr1.count() == 0)) {
            QueryEntry t_qer = correct_3_times[0];
            correct_3_times.erase(correct_3_times.begin());
            random_query_nr = random_expr1.count();
            random_expr1.append(t_qer);
            query_cycle = 4;
        } else if (correct_2_times.count() > 5 || (correct_2_times.count() > 0 && correct_1_times.count() == 0 && random_expr1.count() == 0)) {
            QueryEntry t_qer = correct_2_times[0];
            correct_2_times.erase(correct_2_times.begin());
            random_query_nr = random_expr1.count();
            random_expr1.append(t_qer);
            query_cycle = 3;
        } else if (correct_1_times.count() > 5 || (correct_1_times.count() > 0  && random_expr1.count() == 0)) {
            QueryEntry t_qer = correct_1_times[0];
            correct_1_times.erase(correct_1_times.begin());
            random_query_nr = random_expr1.count();
            random_expr1.append(t_qer);
            query_cycle = 2;
        } else {
            //else we just pick from random_expr1 then
            if (random_expr1.count() == 0) {
                slotStopQuery(true);
                return;
            }
            query_cycle = 1;

            random_query_nr = m_randomSequence.getLong(random_expr1.count());
        }

    } else { // not Prefs::altLearn()
        if (random_expr1.count() == 0) {
            if (random_expr2.count() == 0 && queryList.count() == 0) {
                slotStopQuery(true);
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

        if (random_expr1.count() == 0) { // should not happen !!
            kError() << "QueryManager::slotTimeOutQuery: random_expr1.count() == 0\n";
            slotStopQuery(true);
            return;
        }

        random_query_nr = m_randomSequence.getLong(random_expr1.count());
    }

    exp = random_expr1[random_query_nr].exp;

    tindex = m_doc->indexOfIdentifier(act_query_trans);
    oindex = m_doc->indexOfIdentifier(act_query_org);
    QString q_org,
    q_trans;

    if (Prefs::swapDirection()) {
        bool rand_swap = m_randomSequence.getBool();

        if (rand_swap) {        // random direction
            int tmp = oindex;
            oindex = tindex;
            tindex = tmp;
        }

        if (!m_query.validate(exp, m_doc->currentLesson(), oindex, tindex)) {
            int tmp = oindex;  // must use other direction which is the only valid
            oindex = tindex;
            tindex = tmp;
        }
    }

    if (oindex == 0) {  // usual: give original, ask for translation x
        q_org = exp->original();
        q_trans = exp->translation(tindex);
    } else {  // alternative: give translation x, ask for original
        q_org = exp->translation(oindex);
        q_trans = exp->original();
    }

    if (m_queryType == KVTQuery::RandomQuery) {

        if (randomQueryDlg == 0) {
            kError() << "randomQueryDlg == 0\n";
            slotStopQuery(true);
            return;
        }

        randomQueryDlg->setQuery(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum);
        randomQueryDlg->initFocus();
    } else if (m_queryType == KVTQuery::MultipleChoiceQuery) {
        if (mcQueryDlg == 0) {
            kError() << "mcQueryDlg == 0\n";
            slotStopQuery(true);
            return;
        }
        mcQueryDlg->setQuery(q_org, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, m_doc);
        mcQueryDlg->initFocus();
    }
    m_app->slotStatusMsg(IDS_DEFAULT);
}


void QueryManager::slotStopQuery(bool)
{
    if (simpleQueryDlg != 0)
        simpleQueryDlg->deleteLater();
    if (mcQueryDlg != 0)
        mcQueryDlg->deleteLater();
    if (verbQueryDlg != 0)
        verbQueryDlg->deleteLater();
    if (randomQueryDlg != 0)
        randomQueryDlg->deleteLater();
    if (adjQueryDlg != 0)
        adjQueryDlg->deleteLater();
    if (artQueryDlg != 0)
        artQueryDlg->deleteLater();

    simpleQueryDlg = 0;
    mcQueryDlg = 0;
    verbQueryDlg = 0;
    randomQueryDlg = 0;
    adjQueryDlg = 0;
    artQueryDlg = 0;

    m_app->show();
}

#include "kvtquerymanager.moc"
