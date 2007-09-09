/***************************************************************************

    C++ Interface: kvtquerymanager

    -----------------------------------------------------------------------

    begin         : So Mai 27 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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


#ifndef KVTQUERYMANAGER_H
#define KVTQUERYMANAGER_H

#include <QString>
#include <KRandomSequence>

#include "kvtquery.h"
#include "QueryDlgBase.h"

// class KVTQuery;
// class QueryDlgBase;

class KVocTrainApp;
class KEduVocDocument;


class SimpleQueryDlg;
class MCQueryDlg;
class VerbQueryDlg;
class RandomQueryDlg;
class AdjQueryDlg;
class ArtQueryDlg;

class QueryManager :public QObject
{
Q_OBJECT

public:
    QueryManager(KVocTrainApp *app, KEduVocDocument *doc);

    void query(int queryType, int toTranslation, int fromTranslation);

    void resumeQuery();
    void resumeQueryMC();
    void restartQuery();

    // merge these three:
    void startTypeQuery(int col, KVTQuery::QueryType queryType);
    void startPropertyQuery(int col, KVTQuery::QueryType property);
    void startQuery(const QString & trans, const QString & org, bool create_new);

    bool queryIsEmpty();

    /** exit query mode */
    void stopQuery();

    QString fromTranslation() const { return act_query_org; }
    QString toTranslation() const { return act_query_trans; }

    KVTQuery & getKVTQuery() { return m_query; }
    void showStatistics();

public slots:
    void slotQueryExpressionResult(QueryDlgBase::Result res);
    void slotTimeOutType(QueryDlgBase::Result res);
    void slotTimeOutProperty(QueryDlgBase::Result res);

private slots:
    void slotEditEntry(int row, int translation);

private:
    KVTQuery             m_query;
    KEduVocDocument     *m_doc;
    KVocTrainApp        *m_app;

    // A vector of entry list - so we have Lesson->Entries
    QuerySelection       queryList;

    // random_expr1 contains the vocabulary after they have been asked once
    QueryEntryList       random_expr1;
    // random_expr2 contains the vocab that was right
    QueryEntryList       random_expr2;

    // Vectors for use in Leitner style learning. There is no
    // correct_0_times, we simply reuse random_expr1.
    QueryEntryList       correct_1_times;
    QueryEntryList       correct_2_times;
    QueryEntryList       correct_3_times;


    int                  random_query_nr;
    QString              def_lang;
    int                  act_query_col;
    QString              act_query_trans;
    QString              act_query_org;

    SimpleQueryDlg      *simpleQueryDlg;
    MCQueryDlg          *mcQueryDlg;
    VerbQueryDlg        *verbQueryDlg;
    RandomQueryDlg      *randomQueryDlg;
    AdjQueryDlg         *adjQueryDlg;
    ArtQueryDlg         *artQueryDlg;
    int                  num_queryTimeout;
    int                  query_cycle;
    int                  query_num;
    int                  query_startnum;
    KVTQuery::QueryType  m_queryType;
    KRandomSequence      m_randomSequence;

};

#endif
