/***************************************************************************

    $Id$

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 11:45:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.5  2001/10/28 10:16:31  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.4  2001/10/28 09:17:06  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:45:05  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "MCQueryDlg.h"
#include "QueryDlgBase.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <QueryManager.h>
#include <eadebug.h>
#include <compat_2x.h>
#include "MyProgress.h"

#include <kapp.h>

#include <qtimer.h>
#include <qkeycode.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>

#include <iostream.h>
#include <algo.h>


#define MAX_TRANS  5  // select one out of x

MCQueryDlg::MCQueryDlg(
                   QString org,
                   QString trans,
                   int entry,
                   int orgcol,
                   int transcol,
                   int q_cycle,
                   int q_num,
                   int q_start,
                   kvoctrainExpr *exp,
                   kvoctrainDoc  *doc,
                   int mqtime,
                   bool _show,
                   kvq_timeout_t type_to,
                   QWidget *parent,
                   char *name)
	: MCQueryDlgForm(parent, name, true),
	  QueryDlgBase()
{
   transgroup->insert(rb_trans1);
   transgroup->insert(rb_trans2);
   transgroup->insert(rb_trans3);
   transgroup->insert(rb_trans4);
   transgroup->insert(rb_trans5);

   connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
   connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
   connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
// connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
   connect( show_it, SIGNAL(clicked()), SLOT(showItClicked()) );
   connect( rb_trans5, SIGNAL(clicked()), SLOT(trans5clicked()) );
   connect( rb_trans4, SIGNAL(clicked()), SLOT(trans4clicked()) );
   connect( rb_trans3, SIGNAL(clicked()), SLOT(trans3clicked()) );
   connect( rb_trans2, SIGNAL(clicked()), SLOT(trans2clicked()) );
   connect( rb_trans1, SIGNAL(clicked()), SLOT(trans1clicked()) );
   connect( b_edit, SIGNAL(clicked()), SLOT(editOrgClicked()) );

   qtimer = 0;
   setCaption (kapp->makeStdCaption(i18n("Multiple choice")));
   setQuery (org, trans, entry, orgcol, transcol,
             q_cycle, q_num, q_start,
             exp, doc, mqtime, _show, type_to);
   setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void MCQueryDlg::setQuery(QString org,
                         QString trans,
                         int entry,
                         int orgcol,
                         int transcol,
                         int q_cycle,
                         int q_num,
                         int q_start,
                         kvoctrainExpr *exp,
                         kvoctrainDoc  *doc,
                         int mqtime,
                         bool _show,
                         kvq_timeout_t type_to)
{
   type_timeout = type_to;
   kv_doc = doc;
   q_row = entry;
   q_ocol = orgcol;
   q_tcol = transcol;
   translation = trans;
   showCounter = _show,
   timebar->setEnabled(showCounter);
   timelabel->setEnabled(showCounter);
   orgField->setText (org);
   show_it->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   progCount->setText (s);
//   remark->setText (exp->getRemark(orgcol));
//   falseFriend->setText (exp->getFauxAmi(orgcol));
/*
   type->setText ("");
   vector<TypeRelation> all_types = QueryManager::getRelation(false);
   for (int i = 0; i < (int) all_types.size(); i++) {
     if ( exp->getType(orgcol) == all_types[i].shortStr())
       type->setText(all_types[i].longStr());
   }
*/
   countbar->setData (q_start, q_start-q_num+1, true);
   countbar->repaint();

   if (mqtime >= 1000) { // more than 1000 milli-seconds
     if (qtimer == 0) {
       qtimer = new QTimer( this );
       connect( qtimer, SIGNAL(timeout()), this, SLOT(timeoutReached()) );
     }

     if (type_timeout != kvq_notimeout) {
       timercount = mqtime/1000;
       timebar->setData (timercount, timercount, false);
       timebar->repaint();
       qtimer->start(1000, TRUE);
     }
     else
       timebar->setEnabled(false);
   }
   else
     timebar->setEnabled(false);

   vector<QString> strings;
   button_ref.clear();
   button_ref.push_back(RB_Label(rb_trans1, trans1));
   button_ref.push_back(RB_Label(rb_trans2, trans2));
   button_ref.push_back(RB_Label(rb_trans3, trans3));
   button_ref.push_back(RB_Label(rb_trans4, trans4));
   button_ref.push_back(RB_Label(rb_trans5, trans5));
   random_shuffle(button_ref.begin(), button_ref.end() );
   solution = 0;

   // FIXME: check for false friend and use it
   if (doc->numEntries() <= MAX_TRANS) {
     for (int i = 0; i < doc->numEntries(); i++ ) {
       kvoctrainExpr *act = doc->getEntry(i);
       if (q_tcol == 0)
         strings.push_back(act->getOriginal());
       else 
         strings.push_back(act->getTranslation(q_tcol));

       if (act == exp)
         solution = i;
     }
   }
   else {
     vector<kvoctrainExpr*> exprlist;
     solution = 0;
     exprlist.push_back(exp);

     srand((unsigned int)time((time_t *)NULL));
     int count = MAX_TRANS-1;
     // gather random pointers for the choice
     while (count > 0) {
       int nr = (int) (doc->numEntries() * ((1.0*rand())/RAND_MAX));
       // append if new in list
       bool newex = true;
       for (int i = 0; newex && i < (int) exprlist.size(); i++) {
         if (exprlist[i] == doc->getEntry(nr))
           newex = false;
       }
       if (newex) {
         count--;
         exprlist.push_back(doc->getEntry(nr));
       }
     }

     for (int i = 0; i < (int) exprlist.size(); i++) {
       if (q_tcol == 0)
         strings.push_back(exprlist[i]->getOriginal());
       else
         strings.push_back(exprlist[i]->getTranslation(q_tcol));
     }

   }
   for (int i = strings.size(); i < MAX_TRANS; i++ )
     strings.push_back("");

   if (solution >= MAX_TRANS) {
       solution = 0;
   }

   button_ref[0].rb->setEnabled(!strings[0].isEmpty() );
   button_ref[1].rb->setEnabled(!strings[1].isEmpty() );
   button_ref[2].rb->setEnabled(!strings[2].isEmpty() );
   button_ref[3].rb->setEnabled(!strings[3].isEmpty() );
   button_ref[4].rb->setEnabled(!strings[4].isEmpty() );

   button_ref[0].label->setEnabled(!strings[0].isEmpty() );
   button_ref[1].label->setEnabled(!strings[1].isEmpty() );
   button_ref[2].label->setEnabled(!strings[2].isEmpty() );
   button_ref[3].label->setEnabled(!strings[3].isEmpty() );
   button_ref[4].label->setEnabled(!strings[4].isEmpty() );

   button_ref[0].label->setText(strings[0]);
   button_ref[1].label->setText(strings[1]);
   button_ref[2].label->setText(strings[2]);
   button_ref[3].label->setText(strings[3]);
   button_ref[4].label->setText(strings[4]);

   rb_trans1->setChecked (false);
   rb_trans2->setChecked (false);
   rb_trans3->setChecked (false);
   rb_trans4->setChecked (false);
   rb_trans5->setChecked (false);

   show_it->setFocus();
}


void MCQueryDlg::initFocus() const
{
  rb_trans1->setFocus();
}


void MCQueryDlg::showItClicked()
{
  resetButton(button_ref[0].rb, button_ref[0].label);
  resetButton(button_ref[1].rb, button_ref[1].label);
  resetButton(button_ref[2].rb, button_ref[2].label);
  resetButton(button_ref[3].rb, button_ref[3].label);
  resetButton(button_ref[4].rb, button_ref[4].label);

  button_ref[solution].rb->setFocus();
  button_ref[solution].rb->setChecked(true);
  verifyButton(button_ref[solution].rb, true, button_ref[solution].label);
  dont_know->setDefault(true);
}


void MCQueryDlg::verifyClicked()
{

  bool known = false;
  if (solution == 0)
    known = button_ref[0].rb->isChecked ();
  if (solution == 1)
    known = button_ref[1].rb->isChecked ();
  if (solution == 2)
    known = button_ref[2].rb->isChecked ();
  if (solution == 3)
    known = button_ref[3].rb->isChecked ();
  if (solution == 4)
    known = button_ref[4].rb->isChecked ();

  if (button_ref[0].rb->isChecked() ) {
    verifyButton(button_ref[0].rb, known, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);
  }
  else if (button_ref[1].rb->isChecked() ) {
    resetButton(button_ref[0].rb, button_ref[0].label);
    verifyButton(button_ref[1].rb, known, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);
  }
  else if (button_ref[2].rb->isChecked() ) {
    resetButton(button_ref[0].rb, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    verifyButton(button_ref[2].rb, known, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);
  }
  else if (button_ref[3].rb->isChecked() ) {
    resetButton(button_ref[0].rb, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    verifyButton(button_ref[3].rb, known, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);
  }
  else if (button_ref[4].rb->isChecked() ) {
    resetButton(button_ref[0].rb, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    verifyButton(button_ref[4].rb, known, button_ref[4].label);
  }

  if (known)
//    know_it->setDefault(true);
    knowItClicked();
  else
    dont_know->setDefault(true);
}


void MCQueryDlg::knowItClicked()
{
   done (Known);
}


void MCQueryDlg::timeoutReached()
{
   if (timercount > 0) {
     timercount--;
     timebar->setData (-1, timercount, false);
     timebar->repaint();
     qtimer->start(1000, TRUE);
   }

   if (timercount <= 0) {
     timebar->setData (-1, 0, false);
     timebar->repaint();
     if (type_timeout == kvq_show) {
       showItClicked();
//     verifyClicked();
       dont_know->setDefault(true);
     }
     else if (type_timeout == kvq_cont)
       done (Timeout);
   }
}


void MCQueryDlg::dontKnowClicked()
{
   done (Unknown);
}


void MCQueryDlg::stopItClicked()
{
   done (StopIt);
}


void MCQueryDlg::editOrgClicked()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);
}


void MCQueryDlg::editTransClicked()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_tcol);
}


void MCQueryDlg::keyPressEvent( QKeyEvent *e )
{
  switch( e->key() )
  {
    case Key_Escape:
      dontKnowClicked();
    break;
      
    case Key_Return:
    case Key_Enter:
      if (dont_know->isDefault() )
        dontKnowClicked();
      else if (know_it->isDefault() )
        knowItClicked();
      else if (show_it->isDefault() )
        showItClicked();
//    else if (verify->isDefault() )
//      verifyClicked();
    break;
      
    default:
      e->ignore();
    break;
  }
}


void MCQueryDlg::trans1clicked()
{
  verifyClicked();
}


void MCQueryDlg::trans2clicked()
{
  verifyClicked();
}


void MCQueryDlg::trans3clicked()
{
  verifyClicked();
}


void MCQueryDlg::trans4clicked()
{
  verifyClicked();
}


void MCQueryDlg::trans5clicked()
{
  verifyClicked();
}

#include "MCQueryDlg.moc"
