/***************************************************************************

    $Id$

                       query dialog for properties

    -----------------------------------------------------------------------

    begin                : Sun Apr 9 18:55:34 2000

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.15  2002/11/14 22:52:35  mueller
    compiler warning fixes

    Revision 1.14  2002/09/24 15:07:40  binner
    CVS_SILENT Style guide text fix, use "cvslastchange" or X-WebCVS header to view

    Revision 1.13  2002/01/20 11:41:02  arnold
    fixed issues with modeless dialogs

    Revision 1.12  2001/12/26 15:12:38  mueller
    CVSSILINT: fixincludes

    Revision 1.11  2001/12/13 18:37:28  arnold
    fixed and improved query dialogs

    Revision 1.10  2001/12/01 11:28:54  arnold
    fixed flickering in query dialogs

    Revision 1.9  2001/11/16 16:26:23  arnold
    improved dialogs

    Revision 1.8  2001/11/10 22:29:40  arnold
    removed compatibility for kde1

    Revision 1.7  2001/11/10 21:14:30  arnold
    removed icons and compatibility for kde1

    Revision 1.6  2001/11/09 10:41:18  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/10/28 10:16:31  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.4  2001/10/28 09:17:06  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.3  2001/10/17 21:41:16  waba
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

#include "SimpleQueryDlg.h"
#include "QueryDlgBase.h"

#include <kv_resource.h>

#include <qtimer.h>
#include <qkeycode.h>
#include <qmultilineedit.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>

#include <kapplication.h>
#include <kstandarddirs.h>
#include <klocale.h>

SimpleQueryDlg::SimpleQueryDlg(
        QueryType querytype,
        int entry,
        int column,
        int q_cycle,
        int q_num,
        int q_start,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        int mqtime,
        bool showcounter,
        kvq_timeout_t type_to,
	QWidget* parent,
	const char* name
)
	:
	SimpleQueryDlgForm( parent, name, false),
        QueryDlgBase()
{
   connect( b_edit, SIGNAL(clicked()), SLOT(editClicked()) );
   connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
   connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
   connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
   connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
   connect( show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );
   connect( show_more, SIGNAL(clicked()), SLOT(showMoreClicked()) );

   connect( answerField, SIGNAL(textChanged()), SLOT(slotAnswerChanged()) );

   kv_doc = 0;
   qtimer = 0;
   setQuery (querytype, entry, column,
             q_cycle, q_num, q_start,
             exp, doc, mqtime, showcounter, type_to);
   setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void SimpleQueryDlg::initFocus() const
{
  answerField->setFocus();
}


void SimpleQueryDlg::setQuery(QueryType _querytype,
                         int entry,
                         int column,
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
   querytype = _querytype;
   kv_doc = doc;
   q_row = entry;
   q_ocol = column;
   showCounter = _show,
   timebar->setEnabled(showCounter);
   timelabel->setEnabled(showCounter);
   answerField->setText ("");

   QString s;
   switch (querytype) {
     case QT_Synonym: {
       queryLabel->setText (i18n("Expression"));
       s = i18n("Enter the synonym:");
       instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Synonym Training")));
       answerstring = exp->getSynonym(column);
       queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Antonym: {
       queryLabel->setText (i18n("Expression"));
       s = i18n("Enter the antonym:");
       instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Antonym Training")));
       answerstring = exp->getAntonym(column);
       queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Paraphrase: {
       queryLabel->setText (i18n("Paraphrase"));
       s = i18n("Enter the word:");
       instructionLabel->setText(s);
       setCaption (kapp->makeStdCaption(i18n("Paraphrase Training")));
       queryField->setText (exp->getParaphrase(column));
       answerstring = column == 0 ? exp->getOriginal()
                                  : exp->getTranslation(column);
     }
     break;

     case QT_Example: {
       queryLabel->setText (i18n("Example sentence"));
       s = i18n("Fill in the missing word:");
       instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Example Training")));
       QString s = exp->getExample(column);
       answerstring = column == 0 ? exp->getOriginal().stripWhiteSpace()
                                  : exp->getTranslation(column).stripWhiteSpace();
       int pos = -1;
       while ((pos = s.find(answerstring)) > 0) {
         s.remove(pos, answerstring.length());
         s.insert (pos, "..");
       }
       queryField->setText (s);
     }
     break;

     default:
       ;
   }

   show_all->setDefault(true);
   s.setNum (q_cycle);
   progCount->setText (s);

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
   resetField (answerField);
}


void SimpleQueryDlg::slotAnswerChanged()
{
  verify->setDefault(true);
  resetField (answerField);
}


void SimpleQueryDlg::timeoutReached()
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
       emit sigQueryChoice (Timeout);
   }
}


void SimpleQueryDlg::showMoreClicked()
{
  if (answerField->text().length() < answerstring.length()) {
    answerField->setText (answerstring.left(answerField->text().length()+1));
    dont_know->setDefault(true);
  }
  resetField (answerField);
}


void SimpleQueryDlg::showAllClicked()
{
  answerField->setText (answerstring);
  verifyField (answerField, answerstring,
               querytype == QT_Synonym || querytype == QT_Antonym);
  dont_know->setDefault(true);
}


void SimpleQueryDlg::verifyClicked()
{
  if (verifyField (answerField, answerstring,
                   querytype == QT_Synonym || querytype == QT_Antonym))
//    know_it->setDefault(true);
    knowItClicked();
  else
    dont_know->setDefault(true);
}


void SimpleQueryDlg::knowItClicked()
{
   emit sigQueryChoice (Known);
}


void SimpleQueryDlg::dontKnowClicked()
{
   emit sigQueryChoice (Unknown);
}


void SimpleQueryDlg::stopItClicked()
{
   emit sigQueryChoice (StopIt);
}


void SimpleQueryDlg::editClicked()
{
   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);

   kvoctrainExpr *exp = kv_doc->getEntry(q_row);
//   queryField->setText (exp->getTranslation(q_ocol));

   switch (querytype) {
     case QT_Synonym: {
       answerstring = exp->getSynonym(q_ocol);
       queryField->setText (q_ocol == 0 ? exp->getOriginal()
                                        : exp->getTranslation(q_ocol));
     }
     break;

     case QT_Antonym: {
       answerstring = exp->getAntonym(q_ocol);
       queryField->setText (q_ocol == 0 ? exp->getOriginal()
                                        : exp->getTranslation(q_ocol));
     }
     break;

     case QT_Paraphrase: {
       queryField->setText (exp->getParaphrase(q_ocol));
       answerstring = q_ocol == 0 ? exp->getOriginal()
                                  : exp->getTranslation(q_ocol);
     }
     break;

     case QT_Example: {
       QString s = exp->getExample(q_ocol);
       queryField->setText (s);
       answerstring = q_ocol == 0 ? exp->getOriginal()
                                  : exp->getTranslation(q_ocol);
     }
     break;

     default:
       ;      
   }
}


void SimpleQueryDlg::keyPressEvent( QKeyEvent *e )
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


void SimpleQueryDlg::closeEvent (QCloseEvent*e)
{
   emit sigQueryChoice (StopIt);
}


#include "SimpleQueryDlg.moc"
