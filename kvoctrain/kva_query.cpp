/***************************************************************************

                       query parts of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>

#include "kvoctrain.h"
#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"
#include "prefs.h"

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

void KVocTrainApp::slotStartPropertyQuery(int col, KVTQuery::QueryType property)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting property query..."));
  m_queryType = property;
  querymode = false;
  num_queryTimeout = 0;
  act_query_col = col;

  if (m_tableModel->rowCount(QModelIndex()) < 1)
    return;

  prepareProgressBar();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  random_expr2.clear();
  queryList = querymanager.select(m_doc, m_currentLesson, act_query_col, property);

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
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n(not_contain), i18n("Starting Query")))
      slotGeneralOptionsPage(5);
    return;
  }

  hide();
  querymode = true;

  random_query_nr = m_randomSequence.getLong(random_expr1.count());
  KEduVocExpression *exp = random_expr1[random_query_nr].exp;

  simpleQueryDlg = new SimpleQueryDlg(m_queryType, random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc);
  connect(simpleQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
  connect(simpleQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutProperty(QueryDlgBase::Result)));
  simpleQueryDlg->initFocus();
  simpleQueryDlg->show();
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotTimeOutProperty(QueryDlgBase::Result res)
{
  if (simpleQueryDlg == 0) {
    kError() << "simpleQueryDlg == 0\n";
    slotStopQuery(true);
    return;
  }

  // FIXME: keep track of knowledge ?

  m_doc->setModified();
  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered), i18n("Stopping Query"));
        return;
      }
      else {
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
        //
      }
      else {
        slotStopQuery (true);
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
    }
    else {  // next lesson
      query_cycle = 1;
      random_expr1 = queryList[0];
      queryList.erase(queryList.begin());
    }
  }

  if (random_expr1.count() == 0) { // should not happen !!
    kError() << "KVocTrainApp::slotTimeProperty: random_expr1.count() == 0\n";
    slotStopQuery(true);
    return;
  }

  random_query_nr = m_randomSequence.getLong(random_expr1.count());
  KEduVocExpression *exp = random_expr1[random_query_nr].exp;

  simpleQueryDlg->setQuery(m_queryType, random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc);
  simpleQueryDlg->initFocus();
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotStartTypeQuery(int col, const QString & type)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting special query..."));
  querymode = false;
  num_queryTimeout = 0;
  act_query_col = col;

  if (m_tableModel->rowCount(QModelIndex()) < 1)
    return;

  prepareProgressBar();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  random_expr2.clear();

  queryList = querymanager.select(m_doc, m_currentLesson, act_query_col, type);

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
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n(not_contain), i18n("Starting Query")))
      slotGeneralOptionsPage(5);
    return;
  }

  random_query_nr = m_randomSequence.getLong(random_expr1.count());
  KEduVocExpression *exp = random_expr1[random_query_nr].exp;

  hide();
  querymode = true;
  if (m_queryType == KVTQuery::ConjugationQuery) {
    verbQueryDlg = new VerbQueryDlg(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                                    m_doc->conjugation(act_query_col), exp->conjugation(act_query_col));
    verbQueryDlg->initFocus();
    connect(verbQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(verbQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    verbQueryDlg->show();
  }
  else if (m_queryType == KVTQuery::ArticlesQuery) {
    artQueryDlg = new ArtQueryDlg(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                                  m_doc->article(act_query_col));
    artQueryDlg->initFocus();
    connect(artQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(artQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    artQueryDlg->show();
  }
  else if (m_queryType == KVTQuery::ComparisonQuery) {
    adjQueryDlg = new AdjQueryDlg(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                                  exp->comparison(act_query_col));
    adjQueryDlg->initFocus();
    connect(adjQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(adjQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    adjQueryDlg->show();
  }
  else {
    kError() << "KVocTrainApp::slotTimeOutType: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotTimeOutType(QueryDlgBase::Result res)
{
  // FIXME: keep track of knowledge ?

  m_doc->setModified();
  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered), i18n("Stopping Query"));
        return;
      }
      else {
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
        //
      }
      else {
        slotStopQuery (true);
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
    }
    else {  // next lesson
      query_cycle = 1;
      random_expr1 = queryList[0];
      queryList.erase(queryList.begin());
    }
  }
  hide();

  if (random_expr1.count() == 0) { // should not happen !!
    kError() << "KVocTrainApp::slotTimeSpecial: random_expr1.count() == 0\n";
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
    verbQueryDlg->setQuery(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                           m_doc->conjugation(act_query_col), exp->conjugation(act_query_col));

    verbQueryDlg->initFocus();
  }
  else if (m_queryType == KVTQuery::ArticlesQuery) {
    if (artQueryDlg == 0) {
      kError() << "artQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    artQueryDlg->setQuery(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                          m_doc->article(act_query_col));
    artQueryDlg->initFocus();
  }
  else if (m_queryType == KVTQuery::ComparisonQuery) {
    if (adjQueryDlg == 0) {
      kError() << "adjQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    adjQueryDlg->setQuery(exp->type(act_query_col), random_expr1[random_query_nr].nr, act_query_col, query_cycle, query_num, query_startnum, exp, m_doc,
                          exp->comparison(act_query_col));
    adjQueryDlg->initFocus();
  }
  else {
    kError() << "KVocTrainApp::slotTimeOutType: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotResumeQuery()
{
  m_queryType = KVTQuery::RandomQuery;
  slotRestartQuery();
}


void KVocTrainApp::slotResumeQueryMC()
{
  m_queryType = KVTQuery::MultipleChoiceQuery;
  slotRestartQuery();
}


void KVocTrainApp::slotRestartQuery()
{
  if (random_expr1.count() != 0) {
    queryList.insert(queryList.begin(), random_expr1);
    random_expr1.clear();
  }
  slotStartQuery(act_query_trans, act_query_org, false);
}


void KVocTrainApp::slotStartQuery(const QString & translang, const QString & orglang, bool create_new)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting random query..."));
  querymode = false;
  num_queryTimeout = 0;

  if (m_tableModel->rowCount(QModelIndex()) < 1)
    return;

  int tindex = m_doc->indexOfIdentifier(translang);
  if (tindex < 0)
    return;

  int oindex = m_doc->indexOfIdentifier(orglang);
  if (oindex < 0)
    return;

  act_query_trans = translang;
  act_query_org = orglang;

  prepareProgressBar();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  random_expr2.clear();

  if (create_new || queryList.count() == 0)
    queryList = querymanager.select(m_doc, m_currentLesson, oindex, tindex);

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
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n(not_contain), i18n("Starting Query")))
       slotGeneralOptionsPage(5);
     return;
  }

  hide();
  querymode = true;

  random_query_nr = m_randomSequence.getLong(random_expr1.count());
  KEduVocExpression *exp = random_expr1[random_query_nr].exp;

  QString q_org;
  QString q_trans;

  if (oindex == 0) {  // usual: give original, ask for translation x
    q_org = exp->original();
    q_trans = exp->translation(tindex);
  }
  else {   // alternative: give translation x, ask for original
    q_org = exp->translation(oindex);
    q_trans = exp->original();
  }

  if (m_queryType == KVTQuery::RandomQuery) {
    randomQueryDlg = new RandomQueryDlg(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, exp, m_doc);
    randomQueryDlg->initFocus();
    connect(randomQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(randomQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutRandomQuery(QueryDlgBase::Result)));
    randomQueryDlg->show();
  }
  else if (m_queryType == KVTQuery::MultipleChoiceQuery) {
    mcQueryDlg = new MCQueryDlg(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, exp, m_doc);
    mcQueryDlg->initFocus();
    connect(mcQueryDlg, SIGNAL(sigEditEntry(int,int)), this, SLOT(slotEditEntry(int,int)));
    connect(mcQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)), this, SLOT(slotTimeOutMultipleChoice(QueryDlgBase::Result)));
    mcQueryDlg->show();
  }
  else {
    kError() << "KVocTrainApp::slotStartQuery: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotTimeOutRandomQuery(QueryDlgBase::Result res)
{
  m_queryType = KVTQuery::RandomQuery;
  slotTimeOutQuery(res);
}


void KVocTrainApp::slotTimeOutMultipleChoice(QueryDlgBase::Result res)
{
  m_queryType = KVTQuery::MultipleChoiceQuery;
  slotTimeOutQuery(res);
}


void KVocTrainApp::slotTimeOutQuery(QueryDlgBase::Result res)
{
  m_doc->setModified();

  int tindex = m_doc->indexOfIdentifier(act_query_trans);
  int oindex = m_doc->indexOfIdentifier(act_query_org);
  QueryEntryRef qer = random_expr1[random_query_nr];
  KEduVocExpression *exp = qer.exp;

  if (res != QueryDlgBase::StopIt) {
    m_doc->setModified();

    if (oindex == 0) {
      exp->incQueryCount(tindex, false);
      exp->setQueryDate(tindex, QDateTime::currentDateTime(), false);
    }
    else {
      exp->incQueryCount(oindex, true);
      exp->setQueryDate(oindex, QDateTime::currentDateTime(), true);
    }
  }

  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered), i18n("Stopping Query"));
        return;
      }
      else {
        random_expr1.erase (random_expr1.begin() + random_query_nr);

        //When you get it wrong Leisner style, it ends up in the back of the line
        if (Prefs::altLearn())
          random_expr1.append(qer);
        else
          random_expr2.append(qer);

        if (oindex == 0) {
          exp->incBadCount(tindex, false);
          exp->setGrade(tindex, KV_LEV1_GRADE, false);  // unknown: reset grade
        }
        else {
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
      }
      else {
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
          }
          else {
            exp->incGrade(oindex, true);
          }
          break;
        default:
          kError() << "You should not be able to answer correctly more than 4 times\n";
          slotStopQuery(true);
          return;
        }

        if (random_expr1.count() == 0 && correct_1_times.count() == 0 && correct_2_times.count() == 0 && correct_3_times.count() == 0) {
          slotStopQuery (true);
          return;
        }

      }
      else { //not Prefs::altLearn()
      query_num--;
      if (query_cycle <= 1) {
        if (oindex == 0) {
          exp->incGrade(tindex, false); // incr grade only in first cycle
        }
        else {
          exp->incGrade(oindex, true);
        }
      }
      else {
        if (oindex == 0) {
          exp->setGrade(tindex, KV_LEV1_GRADE, false); // reset grade
        }
        else {
          exp->setGrade(oindex, KV_LEV1_GRADE, true);
        }
      }

      exp->setInQuery(false);
      random_expr1.erase(random_expr1.begin() + random_query_nr);
      if (!(random_expr1.count() != 0 || random_expr2.count() != 0 || queryList.count() != 0)) {
        slotStopQuery (true);
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
      QueryEntryRef t_qer = correct_3_times[0];
      correct_3_times.erase(correct_3_times.begin());
      random_query_nr = random_expr1.count();
      random_expr1.append(t_qer);
      query_cycle = 4;
    }
    else if (correct_2_times.count() > 5 || (correct_2_times.count() > 0 && correct_1_times.count() == 0 && random_expr1.count() == 0)) {
      QueryEntryRef t_qer = correct_2_times[0];
      correct_2_times.erase(correct_2_times.begin());
      random_query_nr = random_expr1.count();
      random_expr1.append(t_qer);
      query_cycle = 3;
    }
    else if (correct_1_times.count() > 5 || (correct_1_times.count() > 0  && random_expr1.count() == 0)) {
      QueryEntryRef t_qer = correct_1_times[0];
      correct_1_times.erase(correct_1_times.begin());
      random_query_nr = random_expr1.count();
      random_expr1.append(t_qer);
      query_cycle = 2;
    }
    else {
      //else we just pick from random_expr1 then
      if (random_expr1.count() == 0) {
        slotStopQuery(true);
        return;
      }
      query_cycle = 1;

      random_query_nr = m_randomSequence.getLong(random_expr1.count());
    }

  }
  else {  // not Prefs::altLearn()
  if (random_expr1.count() == 0) {
    if (random_expr2.count() == 0 && queryList.count() == 0) {
      slotStopQuery(true);
      return;
    }

    if (random_expr2.count() != 0) {  // next cycle with current lesson
      random_expr1 = random_expr2;
      random_expr2.clear();
      query_cycle++;
    }
    else {  // next lesson
      query_cycle = 1;
      random_expr1 = queryList[0];
      queryList.erase(queryList.begin());
    }
  }

  if (random_expr1.count() == 0) { // should not happen !!
    kError() << "KVocTrainApp::slotTimeOutQuery: random_expr1.count() == 0\n";
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

    if (!querymanager.validate(exp, m_currentLesson, oindex, tindex)) {
      int tmp = oindex;  // must use other direction which is the only valid
      oindex = tindex;
      tindex = tmp;
    }
  }

  if (oindex == 0) {  // usual: give original, ask for translation x
    q_org = exp->original();
    q_trans = exp->translation(tindex);
  }
  else {   // alternative: give translation x, ask for original
    q_org = exp->translation(oindex);
    q_trans = exp->original();
  }

  if (m_queryType == KVTQuery::RandomQuery) {
    if (randomQueryDlg == 0) {
      kError() << "randomQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    randomQueryDlg->setQuery(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, exp, m_doc);
    randomQueryDlg->initFocus();
  }
  else if (m_queryType == KVTQuery::MultipleChoiceQuery) {
    if (mcQueryDlg == 0) {
      kError() << "mcQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    mcQueryDlg->setQuery(q_org, q_trans, random_expr1[random_query_nr].nr, oindex, tindex, query_cycle, query_num, query_startnum, exp, m_doc);
      mcQueryDlg->initFocus();
  }
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotStopQuery(bool)
{
  delete simpleQueryDlg;
  delete mcQueryDlg;
  delete verbQueryDlg;
  delete randomQueryDlg;
  delete adjQueryDlg;
  delete artQueryDlg;

  simpleQueryDlg = 0;
  mcQueryDlg = 0;
  verbQueryDlg = 0;
  randomQueryDlg = 0;
  adjQueryDlg = 0;
  artQueryDlg = 0;

  querying = false;
  querymode = false;
  show();
  kapp->setTopWidget(this);
  kapp->setMainWidget(this);
}
