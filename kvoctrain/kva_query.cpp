/***************************************************************************

    $Id$

                       query parts of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.7  2001/11/10 22:27:08  arnold
    removed compatibility for kde1

    Revision 1.6  2001/11/09 14:18:00  arnold
    fixed and improved some dialog pages

    Revision 1.5  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.4  2001/10/28 10:15:46  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.3  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/************x***************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include <qtimer.h>

#include <klocale.h>

#include "kvoctrain.h"
#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"

#include "queryoption-dialogs/QueryOptionsDlg.h"

#define MAX_QUERY_TIMEOUT 3

#ifndef i18n_noop
#define i18n_noop
#endif

static const char * not_answered = i18n_noop(
    "The query dialog was not answered for several times in series.\n"
    "\n"
    "It is assumed that there is currently no person in front of\n"
    "the screen and for that reason the query is stopped.\n");


void kvoctrainApp::slotQueryOptions() /*FOLD00*/
{
   qtimer->stop();

   vector<int> old_liq = doc->getLessonsInQuery();
   QueryOptionsDlg qodlg (
                    maxqueryTime /1000,
                    showcounter,
                    type_querytimeout,
                    doc,
                    lessons,
                    &querymanager,
                    swap_querydir,
                    block,
                    expire,
                    presettings);

   int res = qodlg.exec();
   if (res == QDialog::Accepted) {

      type_querytimeout = qodlg.getTypeQueryTimeout();
      maxqueryTime = qodlg.getMQueryTime()*1000;
      showcounter = qodlg.getShowCounter();
      querymanager = qodlg.getQueryManager();
      swap_querydir = qodlg.getSwapDir();
      block = qodlg.getBlock();
      expire = qodlg.getExpire();
      presettings = qodlg.getPreSetting();
      if (old_liq != qodlg.getQueryManager().lessonItems()) {
        doc->setModified();
        doc->setLessonsInQuery(qodlg.getQueryManager().lessonItems() );
        querymanager.setLessonItems(qodlg.getQueryManager().lessonItems() );
      }

//      view->getTable()->updateContents();
     
      slotStatusMsg(IDS_DEFAULT);
   }
   if (querymode && !querying)
     qtimer->start(0, TRUE);
}


void kvoctrainApp::slotStartPropertyQuery(int col, QueryType property)
{
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
   queryList.erase(&queryList[0], &queryList[0+1]);
   query_startnum = (int) random_expr1.size();
  }
  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  view->getTable()->updateContents();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    KMessageBox::information(this,
      i18n("Your selection does not contain any\n"
           "expressions for the query"),
      kapp->makeStdCaption(i18n("Random query")), i18n("&OK"));
    return;
  }

  hide();
  srand((unsigned int)time((time_t *)NULL));
  delete qtimer;
  qtimer = new QTimer( this );
  connect( qtimer, SIGNAL(timeout()), this, SLOT(slotTimeOutProperty()) );
  qtimer->start(0, TRUE);
  querymode = true;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutProperty()
{
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
      queryList.erase(&queryList[0], &queryList[0+1]);
    }
  }
  hide();

  if (random_expr1.size() == 0) { // should not happen !!
//    KDEBUG (KDEBUG_INFO, 0, "kvoctrainApp::slotTimeProperty: random_expr1.size() == 0");
    cout << "kvoctrainApp::slotTimeProperty: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  querying = true;
  int nr = (int) (random_expr1.size() * ((1.0*rand())/RAND_MAX));
  kvoctrainExpr *exp = random_expr1[nr].exp;

  simpleQueryDlg = new SimpleQueryDlg (
                           queryType,
                           random_expr1[nr].nr,
                           act_query_col,
                           query_cycle,
                           query_num,
                           query_startnum,
                           exp,
                           doc,
                           maxqueryTime,
                           showcounter,
                           type_querytimeout);

  simpleQueryDlg->initFocus();
  connect( simpleQueryDlg, SIGNAL(sigEditEntry(int,int)),
           this, SLOT(slotEditEntry(int,int)));
  int res = simpleQueryDlg->exec();
  delete simpleQueryDlg;
  simpleQueryDlg = 0;

  // FIXME: keep track of knowledge ?

  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(""), i18n("&OK"));
        slotStopQuery(true);
      }
      else {
        random_expr2.push_back (random_expr1[nr]);
        random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
        qtimer->start(0, TRUE);
      }
    break;

    case QueryDlgBase::Unknown :
//      doc->setModified();
      num_queryTimeout = 0;
      random_expr2.push_back (random_expr1[nr]);
      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
      qtimer->start(0, TRUE);
    break;

    case QueryDlgBase::Known :
//      doc->setModified();
      num_queryTimeout = 0;
      query_num--;

      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
      if (   random_expr1.size() != 0
          || random_expr2.size() != 0
          || queryList.size() != 0 ) {
        qtimer->start(0, TRUE);
      }
      else {
        slotStopQuery (true);
      }
    break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
    break;

    default :
      cerr << "unknown result from QueryDlg\n";
  }
  querying = false;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotStartTypeQuery(int col, QString type) /*FOLD00*/
{
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
   queryList.erase(&queryList[0], &queryList[0+1]);
   query_startnum = (int) random_expr1.size();
  }
  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  view->getTable()->updateContents();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    KMessageBox::information(this,
      i18n("Your selection does not contain any\n"
           "expressions for the query"),
      kapp->makeStdCaption(i18n("Random query")), i18n("&OK"));
    return;
  }

  hide();
  srand((unsigned int)time((time_t *)NULL));
  delete qtimer;
  qtimer = new QTimer( this );
  connect( qtimer, SIGNAL(timeout()), this, SLOT(slotTimeOutType()) );
  qtimer->start(0, TRUE);
  querymode = true;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutType() /*FOLD00*/
{
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
      queryList.erase(&queryList[0], &queryList[0+1]);
    }
  }
  hide();

  if (random_expr1.size() == 0) { // should not happen !!
//    KDEBUG (KDEBUG_INFO, 0, "kvoctrainApp::slotTimeSpecial: random_expr1.size() == 0");
    cout << "kvoctrainApp::slotTimeSpecial: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  querying = true;
  int nr = (int) (random_expr1.size() * ((1.0*rand())/RAND_MAX));
  kvoctrainExpr *exp = random_expr1[nr].exp;

  int res = 0;
  if (queryType == QT_Conjugation) {
    verbQueryDlg = new VerbQueryDlg (
                             exp->getType(act_query_col),
                             random_expr1[nr].nr,
                             act_query_col,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             doc->getConjugation(act_query_col),
                             exp->getConjugation(act_query_col),
                             maxqueryTime,
                             showcounter,
                             type_querytimeout);

           verbQueryDlg->initFocus();
           connect( verbQueryDlg, SIGNAL(sigEditEntry(int,int)),
                    this, SLOT(slotEditEntry(int,int)));
           res = verbQueryDlg->exec();
           delete verbQueryDlg;
           verbQueryDlg = 0;

  }
  else if (queryType == QT_Articles) {
    artQueryDlg = new ArtQueryDlg (
                             exp->getType(act_query_col),
                             random_expr1[nr].nr,
                             act_query_col,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             doc->getArticle(act_query_col),
                             maxqueryTime,
                             showcounter,
                             type_querytimeout);
           artQueryDlg->initFocus();
           connect( artQueryDlg, SIGNAL(sigEditEntry(int,int)),
                    this, SLOT(slotEditEntry(int,int)));
           res = artQueryDlg->exec();
           delete artQueryDlg;
           artQueryDlg = 0;

  }
  else if (queryType == QT_Comparison) {
    adjQueryDlg = new AdjQueryDlg (
                             exp->getType(act_query_col),
                             random_expr1[nr].nr,
                             act_query_col,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             exp->getComparison(act_query_col),
                             maxqueryTime,
                             showcounter,
                             type_querytimeout);

           adjQueryDlg->initFocus();
           connect( adjQueryDlg, SIGNAL(sigEditEntry(int,int)),
                    this, SLOT(slotEditEntry(int,int)));
           res = adjQueryDlg->exec();
           delete adjQueryDlg;
           adjQueryDlg = 0;

  }
  else {
//  KDEBUG (KDEBUG_ERROR, 0, "[kvoctrainApp::slotTimeOutType]: unknown type");
    cout << "kvoctrainApp::slotTimeOutType: unknown type\n";
    return;
  }

  // FIXME: keep track of knowledge ?

  switch (res) {
    case QueryDlgBase::Timeout:
      if (++num_queryTimeout >= MAX_QUERY_TIMEOUT) {
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(""), i18n("&OK"));
        slotStopQuery(true);
      }
      else {
        random_expr2.push_back (random_expr1[nr]);
        random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
        qtimer->start(0, TRUE);
      }
    break;

    case QueryDlgBase::Unknown :
//      doc->setModified();
      num_queryTimeout = 0;
      random_expr2.push_back (random_expr1[nr]);
      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
      qtimer->start(0, TRUE);
    break;

    case QueryDlgBase::Known :
//      doc->setModified();
      num_queryTimeout = 0;
      query_num--;

      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
      if (   random_expr1.size() != 0
          || random_expr2.size() != 0
          || queryList.size() != 0 ) {
        qtimer->start(0, TRUE);
      }
      else {
        slotStopQuery (true);
      }
    break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
    break;

    default :
      cerr << "unknown result from QueryDlg\n";
  }
  querying = false;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotRestartQuery()
{
  slotStartQuery(act_query_trans, act_query_org);
}


void kvoctrainApp::slotStartQuery(QString translang, QString orglang) /*FOLD00*/
{
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
  queryList = querymanager.select (doc, act_lesson, oindex, tindex,
                                   swap_querydir, block, expire);

  query_startnum = 0;
  if (queryList.size() > 0) {
   random_expr1 = queryList[0];
   queryList.erase(&queryList[0], &queryList[0+1]);
   query_startnum = (int) random_expr1.size();
  }
  for (int i = 0; i < (int) queryList.size(); i++) {
    int n = queryList[i].size();
    query_startnum += n;
  }
  query_num = query_startnum;

  QApplication::restoreOverrideCursor();
  removeProgressBar();
  view->getTable()->updateContents();
  query_cycle = 1;

  // something left to query ?
  if (query_startnum == 0) {
    KMessageBox::information(this,
      i18n("Your selection does not contain any\n"
           "expressions for the query"),
      kapp->makeStdCaption(i18n("Random query")), i18n("&OK"));
    return;
  }

  hide();
  srand((unsigned int)time((time_t *)NULL));
  delete qtimer;
  qtimer = new QTimer( this );
  connect( qtimer, SIGNAL(timeout()), this, SLOT(slotTimeOutQuery()) );
  qtimer->start(0, TRUE);
  querymode = true;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotTimeOutRandomQuery() /*FOLD00*/
{
  queryType = QT_Random;
  slotTimeOutQuery();
}


void kvoctrainApp::slotTimeOutMultipleChoice() /*FOLD00*/
{
  queryType = QT_Multiple;
  slotTimeOutQuery();
}


void kvoctrainApp::slotTimeOutQuery() /*FOLD00*/
{
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
      queryList.erase(&queryList[0], &queryList[0+1]);
    }
  }
  hide();

  if (random_expr1.size() == 0) { // should not happen !!
//    KDEBUG (KDEBUG_INFO, 0, "kvoctrainApp::slotTimeOutQuery: random_expr1.size() == 0");
    cout << "kvoctrainApp::slotTimeOutQuery: random_expr1.size() == 0\n";
    slotStopQuery(true);
    return;
  }

  querying = true;
  int nr = (int) (random_expr1.size() * ((1.0*rand())/RAND_MAX));
  kvoctrainExpr *exp = random_expr1[nr].exp;
  int tindex = view->getTable()->findIdent(act_query_trans);
  int oindex = view->getTable()->findIdent(act_query_org);
  QString q_org,
          q_trans;
  grade_t q_grade;

  if (swap_querydir) {
    float rnd = ((1.0*rand())/RAND_MAX);
    bool rand_swap = rnd >= 0.5 ? true : false;

    if (rand_swap) {        // random direction
      int tmp = oindex;
      oindex = tindex;
      tindex = tmp;
    }

    if (!querymanager.validate (exp, act_lesson, oindex, tindex,
                                block, expire)) {
      int tmp = oindex;  // must use other direction which is the only valid
      oindex = tindex;
      tindex = tmp;
    }
  }

  if (oindex == 0) {  // usual: give original, ask for translation x
    q_org = exp->getOriginal();
    q_trans = exp->getTranslation(tindex);
    q_grade  =  exp->getGrade(tindex, false);
  }
  else {   // alternative: give translation x, ask for original
    q_org = exp->getTranslation(oindex);
    q_trans = exp->getOriginal();
    q_grade = exp->getGrade(oindex, true);
  }

  int res;
  if (queryType == QT_Random) {
    randomQueryDlg = new RandomQueryDlg (
                             q_org,
                             q_trans,
                             random_expr1[nr].nr,
                             oindex,
                             tindex,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             maxqueryTime,
                             showcounter,
                             type_querytimeout);
      randomQueryDlg->initFocus();
      connect( randomQueryDlg, SIGNAL(sigEditEntry(int,int)),
               this, SLOT(slotEditEntry(int,int)));

      res = randomQueryDlg->exec();
      delete randomQueryDlg;
      randomQueryDlg = 0;

  }
  else if (queryType == QT_Multiple) {
    mcQueryDlg = new MCQueryDlg(
                             q_org,
                             q_trans,
                             random_expr1[nr].nr,
                             oindex,
                             tindex,
                             query_cycle,
                             query_num,
                             query_startnum,
                             exp,
                             doc,
                             maxqueryTime,
                             showcounter,
                             type_querytimeout);
      mcQueryDlg->initFocus();
      connect( mcQueryDlg, SIGNAL(sigEditEntry(int,int)),
               this, SLOT(slotEditEntry(int,int)));

      res = mcQueryDlg->exec();
      delete mcQueryDlg;
      mcQueryDlg = 0;

  }

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
        KMessageBox::information(this, i18n(not_answered),
                                 kapp->makeStdCaption(""), i18n("&OK"));
        slotStopQuery(true);
      }
      else {
        random_expr2.push_back (random_expr1[nr]);
        random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
  
        if (oindex == 0) {
          exp->incBadCount(tindex, false);
          exp->setGrade(tindex, KV_LEV1_GRADE, false);  // unknown: reset grade
        }
        else {
          exp->incBadCount(oindex, true);
          exp->setGrade(oindex, KV_LEV1_GRADE, true);
        }
        qtimer->start(0, TRUE);
      }
    break;

    case QueryDlgBase::Unknown :
      num_queryTimeout = 0;
      random_expr2.push_back (random_expr1[nr]);
      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);

      if (oindex == 0) {
        exp->incBadCount(tindex, false);
        exp->setGrade(tindex, KV_LEV1_GRADE, false);  // unknown: reset grade
      }
      else {
        exp->incBadCount(oindex, true);
        exp->setGrade(oindex, KV_LEV1_GRADE, true);
      }

      qtimer->start(0, TRUE);
    break;

    case QueryDlgBase::Known :
      num_queryTimeout = 0;
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

      exp->setSelected(false);
      random_expr1.erase (&random_expr1[nr], &random_expr1[nr+1]);
      if (   random_expr1.size() != 0
          || random_expr2.size() != 0
          || queryList.size() != 0 ) {
        qtimer->start(0, TRUE);
      }
      else {
        slotStopQuery (true);
      }
    break;

    case QueryDlgBase::StopIt :
        num_queryTimeout = 0;
        slotStopQuery(true);
    break;

    default :
      cerr << "unknown result from QueryDlg\n";
  }

  querying = false;
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotStopQuery(bool show_view) /*FOLD00*/
{
    querymode = false;
    qtimer->stop();
    if (show_view) {
      show();
      kapp->setTopWidget(this);
      kapp->setMainWidget( this );
    }
}
