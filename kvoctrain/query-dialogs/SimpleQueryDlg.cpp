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

#include <kv_resource.h>
#include <eadebug.h>

#include "QueryDlgBase.h"

#define Inherited SimpleQueryDlgData

#include <qtimer.h>
#include <qkeycode.h>

#include <iostream.h>

#include <kapp.h>

SimpleQueryDlg::SimpleQueryDlg(
        SpecFont_t *font,
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
	Inherited( parent, name )
{
   connect( b_edit, SIGNAL(clicked()), SLOT(editClicked()) );
   connect( options, SIGNAL(clicked()), SLOT(optionsClicked()) );
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
   setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));

   if (font != 0 && font->specfont) {
     queryField->setFont (font->limitedFont() );
     answerField->setFont (font->limitedFont() );
   }
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
       answerLabel->setText (i18n("Synonym"));
       queryLabel->setText (i18n("Expression"));
       s = i18n("Query of synonym");
       groupName->setTitle (s);
       setCaption (kapp->makeStdCaption(s));
       answerstring = exp->getSynonym(column);
       queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Antonym: {
       answerLabel->setText(i18n("Antonym"));
       queryLabel->setText (i18n("Expression"));
       s = i18n("Query of antonym");
       groupName->setTitle (s);
       setCaption (kapp->makeStdCaption(s));
       answerstring = exp->getAntonym(column);
       queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Paraphrase: {
       answerLabel->setText (i18n("Expression"));
       queryLabel->setText (i18n("Paraphrase"));
       s = i18n("Query by paraphrase");
       groupName->setTitle (s);
       setCaption (kapp->makeStdCaption(s));
       queryField->setText (exp->getParaphrase(column));
       answerstring = column == 0 ? exp->getOriginal()
                                  : exp->getTranslation(column);
     }
     break;

     case QT_Example: {
       answerLabel->setText (i18n("Expression"));
       queryLabel->setText (i18n("Example sentence"));
       s = i18n("Query by example");
       groupName->setTitle (s);
       setCaption (kapp->makeStdCaption(s));
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
//       verifyClicked();
       dont_know->setDefault(true);
     }
     else if (type_timeout == kvq_cont )
       done (Timeout);
   }
}


void SimpleQueryDlg::showMoreClicked()
{
  resetField (answerField);
  if (showCounter < (int)answerstring.length() ) {
    answerField->setText (answerstring.left(++showCounter));
    dont_know->setDefault(true);
  }

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
   done (QueryDlgBase::Known);
}


void SimpleQueryDlg::dontKnowClicked()
{
   done (QueryDlgBase::Unknown);
}


void SimpleQueryDlg::stopItClicked()
{
   done (QueryDlgBase::StopIt);
}


void SimpleQueryDlg::optionsClicked()
{
   emit sigOptions();
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
#include "SimpleQueryDlg.moc"
