/***************************************************************************

    $Id$

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin                : Sat Dec 4 15:09:18 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#include "AdjQueryDlg.h"
#include "MyProgress.h"

#include <kv_resource.h>
#include <QueryManager.h>
#include <langset.h>

#include <kapplication.h> 
#include <kstandarddirs.h>
#include <klocale.h>

#include <qtimer.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>

AdjQueryDlg::AdjQueryDlg
(
        QString type,
        int entry,
        int col,
        int query_cycle,
        int query_num,
        int query_startnum,
	QFont &font,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        const Comparison &_comp,
        int   mqtime,
        bool _show,
        kvq_timeout_t type_to,
        QWidget* parent,
        const char* name
)
	:
	AdjQueryDlgForm( parent, name, false),
        QueryDlgBase(font)
{
   connect( b_edit, SIGNAL(clicked()), SLOT(editClicked()) );
   connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
   connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
   connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
   connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
   connect( show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );

   connect( lev1Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev2Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev3Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );

   connect( lev1Field, SIGNAL(textChanged(const QString&)), SLOT(lev1Changed(const QString&)) );
   connect( lev2Field, SIGNAL(textChanged(const QString&)), SLOT(lev2Changed(const QString&)) );
   connect( lev3Field, SIGNAL(textChanged(const QString&)), SLOT(lev3Changed(const QString&)) );

   qtimer = 0;
   setCaption (kapp->makeStdCaption(i18n("Comparison Training")));

   lev1Label->setBuddy(lev1Field);
   lev2Label->setBuddy(lev2Field);
   lev3Label->setBuddy(lev3Field);

   setQuery (type, entry, col,
             query_cycle, query_num, query_startnum,
             exp, doc, _comp, mqtime, _show, type_to);
   setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void AdjQueryDlg::setQuery(QString,
                           int entry,
                           int col,
                           int q_cycle,
                           int q_num,
                           int q_start,
                           kvoctrainExpr *exp,
                           kvoctrainDoc  *doc,
                           const Comparison &_comp,
                           int mqtime,
                           bool _show,
                           kvq_timeout_t type_to)
{
   type_timeout = type_to;
   kv_doc = doc;
   kv_exp = exp;
   q_row = entry;
   q_ocol = col;
   showCounter = _show,
   timebar->setEnabled(showCounter);
   timelabel->setEnabled(showCounter);
   comp = _comp;
   show_all->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   progCount->setText (s);

   lev1Field->setText ("");
   lev2Field->setText ("");
   lev3Field->setText ("");

   int sel = getRandom(3);
   switch (sel) {
     case 0: lev1Field->setText (comp.l1() );
     break;

     case 1: lev2Field->setText (comp.l2() );
     break;

     case 2: lev3Field->setText (comp.l3() );
     break;
   }

   lev1Field->setEnabled(!comp.l1().isEmpty() );
   lev2Field->setEnabled(!comp.l2().isEmpty() );
   lev3Field->setEnabled(!comp.l3().isEmpty() );

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
   resetAllFields();
}


void AdjQueryDlg::initFocus() const
{
  lev1Field->setFocus();
}


void AdjQueryDlg::showAllClicked()
{
  resetAllFields();
  lev1Field->setText (comp.l1() );
  lev2Field->setText (comp.l2() );
  lev3Field->setText (comp.l3() );

  verifyField (lev1Field, comp.l1());
  verifyField (lev2Field, comp.l2());
  verifyField (lev3Field, comp.l3());

  dont_know->setDefault(true);
}


void AdjQueryDlg::verifyClicked()
{
  bool all_known = true;

  if (!verifyField (lev1Field, comp.l1()) )
    all_known = false;

  if (!verifyField (lev2Field, comp.l2()) )
    all_known = false;

  if (!verifyField (lev3Field, comp.l3()) )
    all_known = false;

  if (all_known)
//  know_it->setDefault(true);
    knowItClicked();
  else
    dont_know->setDefault(true);
}


void AdjQueryDlg::resetAllFields()
{
  resetField(lev1Field);
  resetField(lev2Field);
  resetField(lev3Field);
}


void AdjQueryDlg::knowItClicked()
{
  emit sigQueryChoice(Known);
}


void AdjQueryDlg::timeoutReached()
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
     if (type_timeout == kvq_show ) {
       showAllClicked();
       dont_know->setDefault(true);
     }
     else if (type_timeout == kvq_cont )
       emit sigQueryChoice(Timeout);
   }
}


void AdjQueryDlg::dontKnowClicked()
{
   emit sigQueryChoice(Unknown);
}


void AdjQueryDlg::stopItClicked()
{
   emit sigQueryChoice(StopIt);
}


void AdjQueryDlg::editClicked()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);
}


void AdjQueryDlg::keyPressEvent( QKeyEvent *e )
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
      else if (show_all->isDefault() )
        showAllClicked();
      else if (verify->isDefault() )
        verifyClicked();
    break;
      
    default:
      e->ignore();
    break;
  }
}


void AdjQueryDlg::returnPressed()
{
}


void AdjQueryDlg::lev1Changed(const QString&)
{
  verify->setDefault(true);
  resetField (lev1Field);
}


void AdjQueryDlg::lev2Changed(const QString&)
{
  verify->setDefault(true);
  resetField (lev2Field);
}


void AdjQueryDlg::lev3Changed(const QString&)
{
  verify->setDefault(true);
  resetField (lev3Field);
}


void AdjQueryDlg::closeEvent (QCloseEvent*)
{
  emit sigQueryChoice(StopIt);
}


#include "AdjQueryDlg.moc"
