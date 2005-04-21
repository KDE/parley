/***************************************************************************

                       query dialog for properties

    -----------------------------------------------------------------------

    begin          : Sun Apr 9 18:55:34 2000

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter@peterandlinda.com>

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

#include "SimpleQueryDlg.h"

#include <kv_resource.h>

#include <qtimer.h>
#include <qmultilineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <kapplication.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kprogress.h>

SimpleQueryDlg::SimpleQueryDlg(
        QueryType querytype,
        int entry,
        int column,
        int q_cycle,
        int q_num,
        int q_start,
        QFont font,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        int mqtime,
        bool showcounter)
  : QueryDlgBase("")
{
  mw = new SimpleQueryDlgForm(this);
  setMainWidget(mw);

  connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
  connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
  connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
  connect(mw->show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );
  connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()) );
  connect(mw->answerField, SIGNAL(textChanged()), SLOT(slotAnswerChanged()) );

  kv_doc = 0;
  qtimer = 0;
  setQuery (querytype, entry, column, q_cycle, q_num, q_start, exp, doc, mqtime, showcounter);
  mw->countbar->setFormat("%v/%m");
  mw->timebar->setFormat("%v");
}


void SimpleQueryDlg::initFocus() const
{
  mw->answerField->setFocus();
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
                         bool _show)
{
   //type_timeout = type_to;
   querytype = _querytype;
   kv_doc = doc;
   q_row = entry;
   q_ocol = column;
   showCounter = _show,
   mw->timebar->setEnabled(showCounter);
   mw->timelabel->setEnabled(showCounter);
   mw->queryField->setFont(Prefs::tableFont());
   mw->answerField->setFont(Prefs::tableFont());
   mw->answerField->setText ("");

   QString s;
   switch (querytype) {
     case QT_Synonym: {
       mw->queryLabel->setText (i18n("Expression"));
       s = i18n("Enter the synonym:");
       mw->instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Synonym Training")));
       answerstring = exp->getSynonym(column);
       mw->queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Antonym: {
       mw->queryLabel->setText (i18n("Expression"));
       s = i18n("Enter the antonym:");
       mw->instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Antonym Training")));
       answerstring = exp->getAntonym(column);
       mw->queryField->setText (column == 0 ? exp->getOriginal()
                                        : exp->getTranslation(column));
     }
     break;

     case QT_Paraphrase: {
       mw->queryLabel->setText (i18n("Paraphrase"));
       s = i18n("Enter the word:");
       mw->instructionLabel->setText(s);
       setCaption (kapp->makeStdCaption(i18n("Paraphrase Training")));
       mw->queryField->setText (exp->getParaphrase(column));
       answerstring = column == 0 ? exp->getOriginal()
                                  : exp->getTranslation(column);
     }
     break;

     case QT_Example: {
       mw->queryLabel->setText (i18n("Example sentence"));
       s = i18n("Fill in the missing word:");
       mw->instructionLabel->setText (s);
       setCaption (kapp->makeStdCaption(i18n("Example Training")));
       QString s = exp->getExample(column);
       answerstring = column == 0 ? exp->getOriginal().stripWhiteSpace()
                                  : exp->getTranslation(column).stripWhiteSpace();
       int pos = -1;
       while ((pos = s.find(answerstring)) > 0) {
         s.remove(pos, answerstring.length());
         s.insert (pos, "..");
       }
       mw->queryField->setText (s);
     }
     break;

     default:
       ;
   }

   mw->show_all->setDefault(true);
   s.setNum (q_cycle);
   mw->progCount->setText (s);

   mw->countbar->setTotalSteps(q_start);
   mw->countbar->setProgress(q_start - q_num + 1);

   if (mqtime >= 1000) { // more than 1000 milli-seconds
     if (qtimer == 0) {
       qtimer = new QTimer( this );
       connect( qtimer, SIGNAL(timeout()), this, SLOT(timeoutReached()) );
     }

     if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
       timercount = mqtime/1000;
       mw->timebar->setTotalSteps(timercount);
       mw->timebar->setProgress(timercount);
       qtimer->start(1000, TRUE);
     }
     else
       mw->timebar->setEnabled(false);
   }
   else
     mw->timebar->setEnabled(false);
   resetField (mw->answerField);
}


void SimpleQueryDlg::slotAnswerChanged()
{
  mw->verify->setDefault(true);
  resetField (mw->answerField);
}


void SimpleQueryDlg::timeoutReached()
{
   if (timercount > 0) {
     timercount--;
     mw->timebar->setProgress(timercount);
     qtimer->start(1000, TRUE);
   }

   if (timercount <= 0) {
     mw->timebar->setProgress(0);
     if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
       showAllClicked();
       mw->dont_know->setDefault(true);
     }
     else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
       emit sigQueryChoice (Timeout);
   }
}


void SimpleQueryDlg::showMoreClicked()
{
  if (mw->answerField->text().length() < answerstring.length()) {
    mw->answerField->setText (answerstring.left(mw->answerField->text().length()+1));
    mw->dont_know->setDefault(true);
  }
  resetField (mw->answerField);
}


void SimpleQueryDlg::showAllClicked()
{
  mw->answerField->setText (answerstring);
  verifyField (mw->answerField, answerstring,
               querytype == QT_Synonym || querytype == QT_Antonym);
  mw->dont_know->setDefault(true);
}


void SimpleQueryDlg::verifyClicked()
{
  if (verifyField (mw->answerField, answerstring,
                   querytype == QT_Synonym || querytype == QT_Antonym))
//    know_it->setDefault(true);
    knowItClicked();
  else
    mw->dont_know->setDefault(true);
}


void SimpleQueryDlg::knowItClicked()
{
   emit sigQueryChoice (Known);
}


void SimpleQueryDlg::dontKnowClicked()
{
   emit sigQueryChoice (Unknown);
}


void SimpleQueryDlg::slotUser2()
{
   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);

   kvoctrainExpr *exp = kv_doc->getEntry(q_row);
//   queryField->setText (exp->getTranslation(q_ocol));

   switch (querytype) {
     case QT_Synonym: {
       answerstring = exp->getSynonym(q_ocol);
       mw->queryField->setText (q_ocol == 0 ? exp->getOriginal()
                                        : exp->getTranslation(q_ocol));
     }
     break;

     case QT_Antonym: {
       answerstring = exp->getAntonym(q_ocol);
       mw->queryField->setText (q_ocol == 0 ? exp->getOriginal()
                                        : exp->getTranslation(q_ocol));
     }
     break;

     case QT_Paraphrase: {
       mw->queryField->setText (exp->getParaphrase(q_ocol));
       answerstring = q_ocol == 0 ? exp->getOriginal()
                                  : exp->getTranslation(q_ocol);
     }
     break;

     case QT_Example: {
       QString s = exp->getExample(q_ocol);
       mw->queryField->setText (s);
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
      if (mw->dont_know->isDefault() )
        dontKnowClicked();
      else if (mw->know_it->isDefault() )
        knowItClicked();
      else if (mw->show_all->isDefault() )
        showAllClicked();
      else if (mw->verify->isDefault() )
        verifyClicked();
    break;

    default:
      e->ignore();
    break;
  }
}


#include "SimpleQueryDlg.moc"
