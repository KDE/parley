/***************************************************************************

    $Id$

                    dialog when in query mode

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
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

#include "QueryDlg.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <QueryManager.h>
#include <compat_2x.h>

#include "../kvoctrain.h"
#include "../rowtable.h"

#include <kapp.h> 

#include <qtimer.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qkeycode.h>
#include <qpainter.h>

#include <iostream.h>


QueryDlg::QueryDlg(
                   SpecFont_t *transfont,
                   SpecFont_t *orgfont,
                   QString org,
                   QString trans,
                   int entry,
                   int orgcol,
                   int transcol,
                   int q_cycle,
                   int q_num,
                   int q_start,
                   kvoctrainExpr *exp,
                   kvoctrainApp  *app,
                   kvoctrainDoc  *doc,
                   int mqtime,
                   bool show,
                   kvq_timeout_t type_to,
                   QWidget *parent,
                   char *name)
	: QueryDlgData(parent,name)
{
   connect( c_type, SIGNAL(clicked()), SLOT(slotTypeClicked()) );
   connect( c_remark, SIGNAL(clicked()), SLOT(slotRemClicked()) );
   connect( c_falsefriend, SIGNAL(clicked()), SLOT(slotFFClicked()) );
   connect( b_edittrans, SIGNAL(clicked()), SLOT(editTransClicked()) );
   connect( options, SIGNAL(clicked()), SLOT(optionsClicked()) );
   connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
   connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
   connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
   connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
   connect( show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );
   connect( show_more, SIGNAL(clicked()), SLOT(showMoreClicked()) );
   connect( b_editorg, SIGNAL(clicked()), SLOT(editOrgClicked()) );

   connect( transField, SIGNAL(textChanged(const QString&)), SLOT(slotTransChanged(const QString&)) );

   kv_doc = 0;
   qtimer = 0;
   setCaption (kvoctrainApp::generateCaption(i18n("Random query"), true));
   setQuery (org, trans, entry, orgcol, transcol,
             q_cycle, q_num, q_start,
             exp, app, doc, mqtime, show, type_to);
   setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));

   if (transfont != 0 && transfont->specfont) {
     transField->setFont (transfont->limitedFont() );
     falseFriend->setFont (transfont->limitedFont() );
   }

   if (orgfont != 0 && orgfont->specfont) {
     orgField->setFont (orgfont->limitedFont() );
     remark->setFont (orgfont->limitedFont() );
//     type->setFont (orgfont->limitedFont() );
   }
}


void QueryDlg::	setQuery(QString org,
                         QString trans,
                         int entry,
                         int orgcol,
                         int transcol,
                         int q_cycle,
                         int q_num,
                         int q_start,
                         kvoctrainExpr *exp,
                         kvoctrainApp  *_app,
                         kvoctrainDoc  *doc,
                         int mqtime,
                         bool _show,
                         kvq_timeout_t type_to)
{
   type_timeout = type_to;
   kv_app = _app;
   kv_doc = doc;
   q_row = entry;
   q_ocol = orgcol;
   q_tcol = transcol;
   translation = trans;
   showCounter = _show,
   timebar->setEnabled(showCounter);
   timelabel->setEnabled(showCounter);
   transField->setText ("");
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

   remark->setText (exp->getRemark(orgcol));
   falseFriend->setText (exp->getFauxAmi(orgcol, orgcol != 0));

   type->setText ("");
   vector<TypeRelation> all_types = QueryManager::getRelation(false);
   for (int i = 0; i < (int) all_types.size(); i++) {
     if ( exp->getType(orgcol) == all_types[i].shortStr())
       type->setText(all_types[i].longStr());
   }

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


void QueryDlg::initFocus() const
{
  transField->setFocus();
}


void QueryDlg::optionsClicked()
{
   emit sigOptions();
}


void QueryDlg::verifyClicked()
{
  if (verifyField (transField, translation))
//    know_it->setDefault(true);
    knowItClicked();
  else
    dont_know->setDefault(true);
}


void QueryDlg::showMoreClicked()
{
  resetField (transField);
  if (QString(transField->text()).length() < translation.length() ) {
    transField->setText (translation.left(QString(transField->text()).length()+1));
    dont_know->setDefault(true);
  }
}


void QueryDlg::showAllClicked()
{
  transField->setText (translation);
  verifyField (transField, translation);
  dont_know->setDefault(true);
}


void QueryDlg::slotTransChanged(const QString&)
{
  verify->setDefault(true);
  resetField (transField);
}


void QueryDlg::knowItClicked()
{
   done (QueryDlg::Known);
}


void QueryDlg::timeoutReached()
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
       showAllClicked();
//       verifyClicked();
       dont_know->setDefault(true);
     }
     else if (type_timeout == kvq_cont)
       done (QueryDlg::Timeout);
   }
}


void QueryDlg::dontKnowClicked()
{
   done (QueryDlg::Unknown);
}


void QueryDlg::stopItClicked()
{
   done (QueryDlg::StopIt);
}


void QueryDlg::editOrgClicked()
{
   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);

   kvoctrainExpr *exp = kv_doc->getEntry(q_row);
   orgField->setText (q_ocol == 0
                        ? exp->getOriginal()
                        : exp->getTranslation(q_ocol));
   remark->setText(exp->getRemark(q_ocol));
   falseFriend->setText(exp->getFauxAmi(q_ocol, q_ocol != 0));

   type->setText ("");
   vector<TypeRelation> all_types = QueryManager::getRelation(false);
   for (int i = 0; i < (int) all_types.size(); i++) {
     if ( exp->getType(q_ocol) == all_types[i].shortStr())
       type->setText(all_types[i].longStr());
   }
}


void QueryDlg::editTransClicked()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_tcol);
   kvoctrainExpr *exp = kv_doc->getEntry(q_row);

   if (transField->text() != "" )
     transField->setText (q_tcol == 0
                          ? exp->getOriginal()
                          : exp->getTranslation(q_ocol));
}


void QueryDlg::slotFFClicked()
{
   if (c_falsefriend->isChecked() )
     falseFriend->show();
   else
     falseFriend->hide();
}


void QueryDlg::slotRemClicked()
{
   if (c_remark->isChecked())
     remark->show();
   else
     remark->hide();
}


void QueryDlg::slotTypeClicked()
{
   if (c_type->isChecked() )
     type->show();
   else
     type->hide();
}


void QueryDlg::keyPressEvent( QKeyEvent *e )
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

#include "QueryDlg.moc"
