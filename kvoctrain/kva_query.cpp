/***************************************************************************

                       query parts of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

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


#include <kdebug.h>

#include "kvoctrain.h"
#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"

#include "queryoption-dialogs/QueryOptionsDlg.h"
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

/*
void kvoctrainApp::slotQueryOptions()
{
   slotQueryOptions(-1);
}


void kvoctrainApp::slotQueryOptions(int pageindex)
{
   vector<int> old_liq = doc->getLessonsInQuery();
   QueryOptionsDlg qodlg (
                    Prefs::maxTimePer() /1000,
                    Prefs::showCounter(),
                    doc,
                    lessons,
                    &querymanager,
                    Prefs::swapDirection(),
                    Prefs::suggestions(),
                    Prefs::split(),
                    Prefs::periods(),
                    Prefs::colons(),
                    Prefs::semicolons(),
                    Prefs::commas(),
                    Prefs::fields(),
                    Prefs::showMore(),
                    Prefs::iKnow(),
                    Prefs::altLearn(),
                    Prefs::block(),
                    Prefs::expire(),
                    presettings);

   if (pageindex >= 0)
     qodlg.selectPage(pageindex);

   int res = qodlg.exec();
   if (res == QDialog::Accepted) {

      //type_querytimeout = qodlg.getTypeQueryTimeout();
      Prefs::setMaxTimePer(qodlg.getMQueryTime()*1000);
      Prefs::setShowCounter(qodlg.getShowCounter());
      querymanager = qodlg.getQueryManager();
      Prefs::setSwapDirection(qodlg.getSwapDir());
      Prefs::setSuggestions(qodlg.getSuggestions());
      Prefs::setSplit(qodlg.getSplit());
      Prefs::setPeriods(qodlg.getPeriods());
      Prefs::setColons(qodlg.getColons());
      Prefs::setSemicolons(qodlg.getSemicolons());
      Prefs::setCommas(qodlg.getCommas());
      Prefs::setFields(qodlg.getFields());
      Prefs::setShowMore(qodlg.getShowMore());
      Prefs::setIKnow(qodlg.getIKnow());
      Prefs::setAltLearn(qodlg.getAltLearn());
      Prefs::setBlock(qodlg.getBlock());
      Prefs::setExpire(qodlg.getExpire());
      presettings = qodlg.getPreSetting();
      if (old_liq != qodlg.getQueryManager().lessonItems()) {
        doc->setModified();
        doc->setLessonsInQuery(qodlg.getQueryManager().lessonItems() );
        querymanager.setLessonItems(qodlg.getQueryManager().lessonItems() );
      }

      slotStatusMsg(IDS_DEFAULT);
   }
}

*/
void kvoctrainApp::slotStartPropertyQuery(int col, QueryType property)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting property query..."));
  queryType = property;
  querymode = false;
  num_queryTimeout = 0;
  act_query_col = col;

  if (view->getTable()->count() < 1)
    return;

  prepareProgressBar();
  QApplication::setOverrideCursor( waitCursor );
  random_expr2.clear();
  queryList = querymanager.select (doc, act_lesson, act_query_col, property);

  query_startnum = 0;
  if (queryList.size() > 0) {
   random_expr1 = queryList[0];
   queryList.erase(queryList.begin());
   query_startnum = (int) random_expr1.size();
  }

  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                                i18n(not_contain),
                                kapp->makeStdCaption(i18n("Starting Query"))))
       slotGeneralOptionsPage(5);
     return;
  }

  hide();
  querymode = true;

  random_query_nr = random.getLong(random_expr1.size());
  kvoctrainExpr *exp = random_expr1[random_query_nr].exp;

  simpleQueryDlg = new SimpleQueryDlg (
                           queryType,
                           random_expr1[random_query_nr].nr,
                           act_query_col,
                           query_cycle,
                           query_num,
                           query_startnum,
                           exp,
                           doc,
                           Prefs::maxTimePer() * 1000,
                           Prefs::showCounter());

  connect( simpleQueryDlg, SIGNAL(sigEditEntry(int,int)),
           this, SLOT(slotEditEntry(int,int)));

  connect( simpleQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
           this, SLOT(slotTimeOutProperty(QueryDlgBase::Result)));

  simpleQueryDlg->initFocus();
  simpleQueryDlg->show();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutProperty(QueryDlgBase::Result res)
{

  if (simpleQueryDlg == 0) {
    kdError() << "simpleQueryDlg == 0\n";
    slotStopQuery(true);
    return;
  }

  // FIXME: keep track of knowledge ?

  doc->setModified();
  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(i18n("Stopping Query")));
        return;
      }
      else {
        random_expr2.push_back (random_expr1[random_query_nr]);
        random_expr1.erase (random_expr1.begin() + random_query_nr);
      }
    break;

    case QueryDlgBase::Unknown :
      num_queryTimeout = 0;
      random_expr2.push_back (random_expr1[random_query_nr]);
      random_expr1.erase (random_expr1.begin() + random_query_nr);
    break;

    case QueryDlgBase::Known :
      num_queryTimeout = 0;
      query_num--;

      random_expr1.erase (random_expr1.begin() + random_query_nr);
      if (   random_expr1.size() != 0
          || random_expr2.size() != 0
          || queryList.size() != 0 ) {
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
      kdError() << "unknown result from QueryDlg\n";
      slotStopQuery(true);
      return;
  }

  if (random_expr1.size() == 0 ) {
    if (   random_expr2.size() == 0
        && queryList.size() == 0) {
      slotStopQuery(true);
      return;
    }

    if (random_expr2.size() != 0) {  // next cycle with current lesson
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

  if (random_expr1.size() == 0) { // should not happen !!
    kdError() << "kvoctrainApp::slotTimeProperty: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  random_query_nr = random.getLong(random_expr1.size());
  kvoctrainExpr *exp = random_expr1[random_query_nr].exp;

  simpleQueryDlg->setQuery(queryType,
                           random_expr1[random_query_nr].nr,
                           act_query_col,
                           query_cycle,
                           query_num,
                           query_startnum,
                           exp,
                           doc,
                           Prefs::maxTimePer() * 1000,
                           Prefs::showCounter());

  simpleQueryDlg->initFocus();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotStartTypeQuery(int col, const QString & type)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting special query..."));
  querymode = false;
  num_queryTimeout = 0;
  act_query_col = col;

  if (view->getTable()->count() < 1)
    return;

  prepareProgressBar();
  QApplication::setOverrideCursor( waitCursor );
  random_expr2.clear();

  queryList = querymanager.select (doc, act_lesson, act_query_col, type);

  query_startnum = 0;
  if (queryList.size() > 0) {
   random_expr1 = queryList[0];
   queryList.erase(queryList.begin());
   query_startnum = (int) random_expr1.size();
  }
  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                                i18n(not_contain),
                                kapp->makeStdCaption(i18n("Starting Query"))))
       slotGeneralOptionsPage(5);
     return;
  }

  random_query_nr = random.getLong(random_expr1.size());
  kvoctrainExpr *exp = random_expr1[random_query_nr].exp;

  hide();
  querymode = true;
  if (queryType == QT_Conjugation) {
    verbQueryDlg = new VerbQueryDlg (exp->getType(act_query_col),
                            random_expr1[random_query_nr].nr,
                            act_query_col,
                            query_cycle,
                            query_num,
                            query_startnum,
                            exp,
                            doc,
                            doc->getConjugation(act_query_col),
                            exp->getConjugation(act_query_col),
                            Prefs::maxTimePer() * 1000,
                            Prefs::showCounter());

    verbQueryDlg->initFocus();
    connect( verbQueryDlg, SIGNAL(sigEditEntry(int,int)),
             this, SLOT(slotEditEntry(int,int)));

    connect(verbQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
                   this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    verbQueryDlg->show();
  }
  else if (queryType == QT_Articles) {
    artQueryDlg = new ArtQueryDlg(exp->getType(act_query_col),
                          random_expr1[random_query_nr].nr,
                          act_query_col,
                          query_cycle,
                          query_num,
                          query_startnum,
                          exp,
                          doc,
                          doc->getArticle(act_query_col),
                          Prefs::maxTimePer() * 1000,
                          Prefs::showCounter());
    artQueryDlg->initFocus();
    connect( artQueryDlg, SIGNAL(sigEditEntry(int,int)),
             this, SLOT(slotEditEntry(int,int)));
    connect(artQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
                      this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    artQueryDlg->show();
  }
  else if (queryType == QT_Comparison) {
    adjQueryDlg = new AdjQueryDlg(exp->getType(act_query_col),
                          random_expr1[random_query_nr].nr,
                          act_query_col,
                          query_cycle,
                          query_num,
                          query_startnum,
                          exp,
                          doc,
                          exp->getComparison(act_query_col),
                          Prefs::maxTimePer() * 1000,
                          Prefs::showCounter());
    adjQueryDlg->initFocus();
    connect( adjQueryDlg, SIGNAL(sigEditEntry(int,int)),
              this, SLOT(slotEditEntry(int,int)));

    connect(adjQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
                    this, SLOT(slotTimeOutType(QueryDlgBase::Result)));
    adjQueryDlg->show();
  }
  else {
    kdError() << "kvoctrainApp::slotTimeOutType: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutType(QueryDlgBase::Result res)
{

  // FIXME: keep track of knowledge ?

  doc->setModified();
  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(i18n("Stopping Query")));
        return;
      }
      else {
        random_expr2.push_back (random_expr1[random_query_nr]);
        random_expr1.erase (random_expr1.begin() + random_query_nr);
      }
    break;

    case QueryDlgBase::Unknown :
      num_queryTimeout = 0;
      random_expr2.push_back (random_expr1[random_query_nr]);
      random_expr1.erase (random_expr1.begin() + random_query_nr);
    break;

    case QueryDlgBase::Known :
      num_queryTimeout = 0;
      query_num--;

      random_expr1.erase (random_expr1.begin() + random_query_nr);
      if (   random_expr1.size() != 0
          || random_expr2.size() != 0
          || queryList.size() != 0 ) {
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
      kdError() << "unknown result from QueryDlg\n";
      slotStopQuery(true);
      return;
  }


  if (random_expr1.size() == 0 ) {
    if (   random_expr2.size() == 0
        && queryList.size() == 0) {
      slotStopQuery(true);
      return;
    }

    if (random_expr2.size() != 0) {  // next cycle with current lesson
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

  if (random_expr1.size() == 0) { // should not happen !!
    kdError() << "kvoctrainApp::slotTimeSpecial: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  random_query_nr = random.getLong(random_expr1.size());
  kvoctrainExpr *exp = random_expr1[random_query_nr].exp;

  if (queryType == QT_Conjugation) {
    if (verbQueryDlg == 0) {
      kdError() << "verbQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    verbQueryDlg->setQuery (exp->getType(act_query_col),
                            random_expr1[random_query_nr].nr,
                            act_query_col,
                            query_cycle,
                            query_num,
                            query_startnum,
                            exp,
                            doc,
                            doc->getConjugation(act_query_col),
                            exp->getConjugation(act_query_col),
                            Prefs::maxTimePer() * 1000,
                            Prefs::showCounter());

    verbQueryDlg->initFocus();
  }
  else if (queryType == QT_Articles) {
    if (artQueryDlg == 0) {
      kdError() << "artQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    artQueryDlg->setQuery(exp->getType(act_query_col),
                          random_expr1[random_query_nr].nr,
                          act_query_col,
                          query_cycle,
                          query_num,
                          query_startnum,
                          exp,
                          doc,
                          doc->getArticle(act_query_col),
                          Prefs::maxTimePer() * 1000,
                          Prefs::showCounter());
    artQueryDlg->initFocus();
  }
  else if (queryType == QT_Comparison) {
    if (adjQueryDlg == 0) {
      kdError() << "adjQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    adjQueryDlg->setQuery(exp->getType(act_query_col),
                          random_expr1[random_query_nr].nr,
                          act_query_col,
                          query_cycle,
                          query_num,
                          query_startnum,
                          exp,
                          doc,
                          exp->getComparison(act_query_col),
                          Prefs::maxTimePer() * 1000,
                          Prefs::showCounter());
    adjQueryDlg->initFocus();
  }
  else {
    kdError() << "kvoctrainApp::slotTimeOutType: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotRestartQuery()
{
  if (random_expr1.size() != 0) {
    queryList.insert(queryList.begin(), random_expr1);
    random_expr1.clear();
  }
  slotStartQuery(act_query_trans, act_query_org, false);
}


void kvoctrainApp::slotStartQuery(const QString & translang, const QString & orglang, bool create_new)
{
  removeEntryDlg();
  slotStatusMsg(i18n("Starting random query..."));
  querymode = false;
  num_queryTimeout = 0;

  if (view->getTable()->count() < 1)
    return;

  int tindex = view->getTable()->findIdent(translang);
  if (tindex < 0)
    return;

  int oindex = view->getTable()->findIdent(orglang);
  if (oindex < 0)
    return;

  act_query_trans = translang;
  act_query_org = orglang;

  prepareProgressBar();
  QApplication::setOverrideCursor( waitCursor );
  random_expr2.clear();

  if (create_new || queryList.size() == 0)
    queryList = querymanager.select (doc, act_lesson, oindex, tindex,
                                     Prefs::swapDirection(), Prefs::altLearn(), Prefs::block(), Prefs::expire());

  query_startnum = 0;
  if (queryList.size() > 0) {
   random_expr1 = queryList[0];
   queryList.erase(queryList.begin());
   query_startnum = (int) random_expr1.size();
  }
  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                                i18n(not_contain),
                                kapp->makeStdCaption(i18n("Starting Query"))))
       slotGeneralOptionsPage(5);
     return;
  }

  hide();
  querymode = true;

  random_query_nr = random.getLong(random_expr1.size());
  kvoctrainExpr *exp = random_expr1[random_query_nr].exp;

  QString q_org,
          q_trans;

  if (oindex == 0) {  // usual: give original, ask for translation x
    q_org = exp->getOriginal();
    q_trans = exp->getTranslation(tindex);
  }
  else {   // alternative: give translation x, ask for original
    q_org = exp->getTranslation(oindex);
    q_trans = exp->getOriginal();
  }

  if (queryType == QT_Random) {
    randomQueryDlg = new RandomQueryDlg (
                             q_org,
                             q_trans,
                             random_expr1[random_query_nr].nr,
                             oindex,
                             tindex,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             Prefs::maxTimePer() * 1000,
                             Prefs::showCounter(),
                             Prefs::suggestions(),
                             Prefs::split(),
                             Prefs::periods(),
                             Prefs::colons(),
                             Prefs::semicolons(),
                             Prefs::commas(),
                             Prefs::fields(),
                             Prefs::showMore(),
                             Prefs::iKnow(),
                             Prefs::swapDirection());
      randomQueryDlg->initFocus();
      connect( randomQueryDlg, SIGNAL(sigEditEntry(int,int)),
               this, SLOT(slotEditEntry(int,int)));
      connect(randomQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
               this, SLOT(slotTimeOutRandomQuery(QueryDlgBase::Result)));
      randomQueryDlg->show();
  }
  else if (queryType == QT_Multiple) {
    mcQueryDlg = new MCQueryDlg(
                             q_org,
                             q_trans,
                             random_expr1[random_query_nr].nr,
                             oindex,
                             tindex,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             Prefs::maxTimePer() * 1000,
                             Prefs::showCounter());
      mcQueryDlg->initFocus();
      connect( mcQueryDlg, SIGNAL(sigEditEntry(int,int)),
               this, SLOT(slotEditEntry(int,int)));
      connect(mcQueryDlg, SIGNAL(sigQueryChoice(QueryDlgBase::Result)),
               this, SLOT(slotTimeOutMultipleChoice(QueryDlgBase::Result)));
      mcQueryDlg->show();
  }
  else {
    kdError() << "kvoctrainApp::slotStartQuery: unknown type\n";
    slotStopQuery(true);
    return;
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutRandomQuery(QueryDlgBase::Result res)
{
  queryType = QT_Random;
  slotTimeOutQuery(res);
}


void kvoctrainApp::slotTimeOutMultipleChoice(QueryDlgBase::Result res)
{
  queryType = QT_Multiple;
  slotTimeOutQuery(res);
}


void kvoctrainApp::slotTimeOutQuery(QueryDlgBase::Result res)
{
  doc->setModified();


  int tindex = view->getTable()->findIdent(act_query_trans);
  int oindex = view->getTable()->findIdent(act_query_org);
  QueryEntryRef qer = random_expr1[random_query_nr];
  kvoctrainExpr *exp = qer.exp;

  if (res != QueryDlgBase::StopIt) {
    doc->setModified();
    time_t now = time(0);
    if (oindex == 0) {
      exp->incQueryCount(tindex, false);
      exp->setQueryDate(tindex, now, false);
    }
    else {
      exp->incQueryCount(oindex, true);
      exp->setQueryDate(oindex, now, true);
    }
  }

  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        slotStopQuery(true);
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(i18n("Stopping Query")));
        return;
      }
      else {
        random_expr1.erase (random_expr1.begin() + random_query_nr);

        //When you get it wrong Leisner style, it ends up in the back
        //of the line
        if (Prefs::altLearn())
          random_expr1.push_back (qer);
        else
          random_expr2.push_back (qer);

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
      random_expr1.erase (random_expr1.begin() + random_query_nr);

      //When you get it wrong Leisner style, it ends up in the back
      //of the line
      if (Prefs::altLearn())
        random_expr1.push_back (qer);
      else
        random_expr2.push_back (qer);

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
        random_expr1.erase (random_expr1.begin() + random_query_nr);

        //The user guessed right (or she actually knew the
        //answer). Move the exp up to next level.
        switch (query_cycle) {
        case 1:
          correct_1_times.push_back (qer);
          break;
        case 2:
          correct_2_times.push_back (qer);
          break;
        case 3:
          correct_3_times.push_back (qer);
          break;
        case 4:
          //The user has answered correctly four times in a row. She
          //is good!
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
          kdError() << "You should not be able to answer correctly more than 4 times\n";
          slotStopQuery(true);
          return;
        }

        if (random_expr1.size() == 0
            && correct_1_times.size() == 0
            && correct_2_times.size() == 0
            && correct_3_times.size() == 0) {
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
      random_expr1.erase (random_expr1.begin() + random_query_nr);
      if (!(   random_expr1.size() != 0
            || random_expr2.size() != 0
            || queryList.size() != 0 )) {
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
      kdError() << "unknown result from QueryDlg\n";
      slotStopQuery(true);
      return;
  }

  if (Prefs::altLearn()) {

    if (correct_3_times.size() > 7 ||
        (correct_3_times.size() > 0
         && correct_2_times.size() == 0
         && correct_1_times.size() == 0
         && random_expr1.size() == 0)) {
      QueryEntryRef t_qer = correct_3_times[0];
      correct_3_times.erase(correct_3_times.begin());
      random_query_nr = random_expr1.size();
      random_expr1.push_back(t_qer);
      query_cycle = 4;
    }
    else if (correct_2_times.size() > 5 ||
             (correct_2_times.size() > 0
              && correct_1_times.size() == 0
              && random_expr1.size() == 0)) {
      QueryEntryRef t_qer = correct_2_times[0];
      correct_2_times.erase(correct_2_times.begin());
      random_query_nr = random_expr1.size();
      random_expr1.push_back(t_qer);
      query_cycle = 3;
    }
    else if (correct_1_times.size() > 5 ||
             (correct_1_times.size() > 0
              && random_expr1.size() == 0)) {
      QueryEntryRef t_qer = correct_1_times[0];
      correct_1_times.erase(correct_1_times.begin());
      random_query_nr = random_expr1.size();
      random_expr1.push_back(t_qer);
      query_cycle = 2;
    }
    else {
      //else we just pick from random_expr1 then
      if (random_expr1.size() == 0 ) {
        slotStopQuery(true);
        return;
      }
      query_cycle = 1;

      random_query_nr = random.getLong(random_expr1.size());
    }

  }
  else {  // not Prefs::altLearn()
  if (random_expr1.size() == 0 ) {
    if (   random_expr2.size() == 0
        && queryList.size() == 0) {
      slotStopQuery(true);
      return;
    }

    if (random_expr2.size() != 0) {  // next cycle with current lesson
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

  if (random_expr1.size() == 0) { // should not happen !!
    kdError() << "kvoctrainApp::slotTimeOutQuery: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  random_query_nr = random.getLong(random_expr1.size());
  }

  exp = random_expr1[random_query_nr].exp;

  tindex = view->getTable()->findIdent(act_query_trans);
  oindex = view->getTable()->findIdent(act_query_org);
  QString q_org,
          q_trans;

  if (Prefs::swapDirection()) {
    bool rand_swap = random.getBool();

    if (rand_swap) {        // random direction
      int tmp = oindex;
      oindex = tindex;
      tindex = tmp;
    }

    if (!querymanager.validate (exp, act_lesson, oindex, tindex,
                                Prefs::block(), Prefs::expire())) {
      int tmp = oindex;  // must use other direction which is the only valid
      oindex = tindex;
      tindex = tmp;
    }
  }

  if (oindex == 0) {  // usual: give original, ask for translation x
    q_org = exp->getOriginal();
    q_trans = exp->getTranslation(tindex);
  }
  else {   // alternative: give translation x, ask for original
    q_org = exp->getTranslation(oindex);
    q_trans = exp->getOriginal();
  }

  if (queryType == QT_Random) {
    if (randomQueryDlg == 0) {
      kdError() << "randomQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    randomQueryDlg->setQuery(q_org,
                             q_trans,
                             random_expr1[random_query_nr].nr,
                             oindex,
                             tindex,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             Prefs::maxTimePer() * 1000,
                             Prefs::showCounter());
      randomQueryDlg->initFocus();
  }
  else if (queryType == QT_Multiple) {
    if (mcQueryDlg == 0) {
      kdError() << "mcQueryDlg == 0\n";
      slotStopQuery(true);
      return;
    }
    mcQueryDlg->setQuery(q_org,
                         q_trans,
                         random_expr1[random_query_nr].nr,
                         oindex,
                         tindex,
                         query_cycle,
                         query_num,
                         query_startnum,
                         exp,
                         doc,
                         Prefs::maxTimePer() * 1000,
                         Prefs::showCounter());
      mcQueryDlg->initFocus();
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotStopQuery(bool )
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
    kapp->setMainWidget( this );
}
