/***************************************************************************

    $Id$

                    dialog when in query mode

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
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

#include "RandomQueryDlg.h"
#include "MyProgress.h"

#include <kv_resource.h>
#include <QueryManager.h>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kapplication.h>

#include <qtimer.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>

RandomQueryDlg::RandomQueryDlg(
                   QString org,
                   QString trans,
                   int entry,
                   int orgcol,
                   int transcol,
                   int q_cycle,
                   int q_num,
                   int q_start,
		   QFont & font,
                   kvoctrainExpr *exp,
                   kvoctrainDoc  *doc,
                   int mqtime,
                   bool show,
                   kvq_timeout_t type_to,
                   QWidget *parent,
                   char *name)
	: QueryDlgForm(parent, name, false),
	  QueryDlgBase(font)
{
   connect( c_type, SIGNAL(clicked()), SLOT(slotTypeClicked()) );
   connect( c_remark, SIGNAL(clicked()), SLOT(slotRemClicked()) );
   connect( c_falsefriend, SIGNAL(clicked()), SLOT(slotFFClicked()) );
   connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
   connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
   connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
   connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
   connect( show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );
   connect( show_more, SIGNAL(clicked()), SLOT(showMoreClicked()) );
   connect( b_edit, SIGNAL(clicked()), SLOT(editEntryClicked()) );
   connect( transField, SIGNAL(textChanged(const QString&)), SLOT(slotTransChanged(const QString&)) );

   kv_doc = 0;
   qtimer = 0;
   setCaption (kapp->makeStdCaption(i18n("Random Query")));
   setQuery (org, trans, entry, orgcol, transcol,
             q_cycle, q_num, q_start,
             exp, doc, mqtime, show, type_to);
   setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void RandomQueryDlg::setQuery(QString org,
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
   transField->setFont(word_font);
   transField->setText ("");
   orgField->setFont(word_font);
   orgField->setText (org);
   show_all->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   progCount->setText (s);

   remark->hide();
   falseFriend->hide();
   type->hide();

   c_remark->setChecked(false);
   c_falsefriend->setChecked(false);
   c_type->setChecked(false);
   setHintFields();

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
   resetField (transField);
}


void RandomQueryDlg::initFocus() const
{
  transField->setFocus();
}


void RandomQueryDlg::verifyClicked()
{
  if (verifyField (transField, translation)) {
    status->setText(getOKComment(countbar->getPercentage()));
    knowItClicked();
  }
  else {
    status->setText(getNOKComment(countbar->getPercentage()));
    dont_know->setDefault(true);
  }
}


void RandomQueryDlg::showMoreClicked()
{
  resetField (transField);
  if (QString(transField->text()).length() < translation.length() ) {
    transField->setText (translation.left(QString(transField->text()).length()+1));
    dont_know->setDefault(true);
  }
}


void RandomQueryDlg::showAllClicked()
{
  transField->setText (translation);
  verifyField (transField, translation);
  dont_know->setDefault(true);
}


void RandomQueryDlg::slotTransChanged(const QString&)
{
  verify->setDefault(true);
  resetField (transField);
}


void RandomQueryDlg::knowItClicked()
{
   status->setText("");
   emit sigQueryChoice (Known);
}


void RandomQueryDlg::timeoutReached()
{
   if (timercount > 0) {
     timercount--;
     timebar->setData (-1, timercount, false);
     timebar->repaint();
     qtimer->start(1000, TRUE);
   }

   if (timercount <= 0) {
     status->setText(getTimeoutComment(countbar->getPercentage()));
     timebar->setData (-1, 0, false);
     timebar->repaint();
     if (type_timeout == kvq_show) {
       showAllClicked();
       dont_know->setDefault(true);
     }
     else if (type_timeout == kvq_cont) {
       emit sigQueryChoice (Timeout);
     }
   }
   else
     status->setText("");

}


void RandomQueryDlg::dontKnowClicked()
{
   status->setText("");
   emit sigQueryChoice (Unknown);
}


void RandomQueryDlg::stopItClicked()
{
   emit sigQueryChoice (StopIt);
}


void RandomQueryDlg::setHintFields()
{
   QString s;
   kvoctrainExpr *exp = kv_doc->getEntry(q_row);

   s = exp->getRemark(q_ocol);
   remark->setText (s);
   c_remark->setEnabled(!s.isEmpty() );

   s = exp->getFauxAmi(q_ocol, q_ocol != 0);
   falseFriend->setText (s);
   c_falsefriend->setEnabled(!s.isEmpty() );

   s = "";
   vector<TypeRelation> all_types = QueryManager::getRelation(false);
   for (int i = 0; i < (int) all_types.size(); i++) {
     if ( exp->getType(q_ocol) == all_types[i].shortStr()) {
       s = all_types[i].longStr();
       break;
     }
   }
   type->setText (s);
   c_type->setEnabled(!s.isEmpty() );
}


void RandomQueryDlg::editEntryClicked()
{
   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);

   kvoctrainExpr *exp = kv_doc->getEntry(q_row);
   orgField->setText (q_ocol == 0
                        ? exp->getOriginal()
                        : exp->getTranslation(q_ocol));
   transField->setText ("");

   setHintFields();
}


void RandomQueryDlg::slotFFClicked()
{
   if (c_falsefriend->isChecked() )
     falseFriend->show();
   else
     falseFriend->hide();
}


void RandomQueryDlg::slotRemClicked()
{
   if (c_remark->isChecked())
     remark->show();
   else
     remark->hide();
}


void RandomQueryDlg::slotTypeClicked()
{
   if (c_type->isChecked() )
     type->show();
   else
     type->hide();
}


void RandomQueryDlg::keyPressEvent( QKeyEvent *e )
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


void RandomQueryDlg::closeEvent (QCloseEvent*e)
{
   emit sigQueryChoice (StopIt);
}


#include "RandomQueryDlg.moc"
