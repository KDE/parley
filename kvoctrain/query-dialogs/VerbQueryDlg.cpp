/***************************************************************************

    $Id$

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin                : Fri Dec 3 18:28:18 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
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

#include "VerbQueryDlg.h"
#include "QueryDlgBase.h"
#include "MyProgress.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <QueryManager.h>
#include <eadebug.h>

#include <compat_2x.h>

#include <kapp.h> 

#include <qtimer.h>
#include <qkeycode.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qlineedit.h>

#include <iostream.h>


#include <kapp.h>

VerbQueryDlg::VerbQueryDlg
(
        SpecFont_t *font,
	QString type,
        int entry,
        int col,
        int query_cycle,
        int query_num,
        int query_startnum,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        const Conjugation &prefix,
        const Conjugation &conjug,
        int mqtime,
        bool _show,
        kvq_timeout_t type_to,
        QWidget *parent,
        char *name)
	:
	VerbQueryDlgForm( parent, name ),
        QueryDlgBase()
{
	connect( stop_it, SIGNAL(clicked()), SLOT(stopItClicked()) );
        connect( b_edit, SIGNAL(clicked()), SLOT(editClicked()) );
	connect( dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
	connect( know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
	connect( verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
	connect( show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );

	connect( p3pmField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p3pnField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p3snField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p3smField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p3pfField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p3sfField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p2pField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p2sField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p1pField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
	connect( p1sField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );

	connect( p3pmField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pmChanged(const QString&)) );
	connect( p3snField, SIGNAL(textChanged(const QString&)), SLOT(slotP3snChanged(const QString&)) );
	connect( p3pnField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pnChanged(const QString&)) );
	connect( p3smField, SIGNAL(textChanged(const QString&)), SLOT(slotP3smChanged(const QString&)) );
	connect( p3pfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pfChanged(const QString&)) );
	connect( p3sfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3sfChanged(const QString&)) );
	connect( p2pField, SIGNAL(textChanged(const QString&)), SLOT(slotP2pChanged(const QString&)) );
	connect( p2sField, SIGNAL(textChanged(const QString&)), SLOT(slotP2sChanged(const QString&)) );
	connect( p1pField, SIGNAL(textChanged(const QString&)), SLOT(slotP1pChanged(const QString&)) );
	connect( p1sField, SIGNAL(textChanged(const QString&)), SLOT(slotP1sChanged(const QString&)) );

   qtimer = 0;
   setCaption (kapp->makeStdCaption(i18n("Training with verbs")));
   setQuery (type, entry, col,
             query_cycle, query_num, query_startnum,
             exp, doc, prefix, conjug, mqtime, _show, type_to);
   setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));

   if (font != 0 && font->specfont) {
     QFont specfont = font->limitedFont();
     p1sLabel->setFont (specfont);
     p2sLabel->setFont (specfont);
     p3sfLabel->setFont (specfont);
     p3smLabel->setFont (specfont);
     p3snLabel->setFont (specfont);
  
     p1pLabel->setFont (specfont);
     p2pLabel->setFont (specfont);
     p3pfLabel->setFont (specfont);
     p3pmLabel->setFont (specfont);
     p3pnLabel->setFont (specfont);

     p3pmField->setFont (specfont);
     p3snField->setFont (specfont);
     p3pnField->setFont (specfont);
     p3smField->setFont (specfont);
     p3pfField->setFont (specfont);
     p3sfField->setFont (specfont);
     p2pField->setFont (specfont);
     p2sField->setFont (specfont);
     p1pField->setFont (specfont);
     p1sField->setFont (specfont);
  }

}


void VerbQueryDlg::initFocus() const
{
  p1sField->setFocus();
}


void VerbQueryDlg::setQuery(QString type,
                            int entry,
                            int col,
                            int q_cycle,
                            int q_num,
                            int q_start,
                            kvoctrainExpr *exp,
                            kvoctrainDoc  *doc,
                            const Conjugation &prefix,
                            const Conjugation &conjug,
                            int mqtime,
                            bool _show,
                            kvq_timeout_t type_to)
{
   type_timeout = type_to;
   kv_doc = doc;
   kv_exp = exp;
   q_row = entry;
   q_ocol = col;
   query_time = mqtime;
   showCounter = _show,
   timebar->setEnabled(showCounter);
   timelabel->setEnabled(showCounter);
   show_all->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   progCount->setText (s);

   conjugations = conjug;

   p1sLabel->setText  (prefix.pers1Singular(CONJ_PREFIX));
   p2sLabel->setText  (prefix.pers2Singular(CONJ_PREFIX));
   p3sfLabel->setText (prefix.pers3FemaleSingular(CONJ_PREFIX));
   p3smLabel->setText (prefix.pers3MaleSingular(CONJ_PREFIX));
   p3snLabel->setText (prefix.pers3NaturalSingular(CONJ_PREFIX));

   p1pLabel->setText  (prefix.pers1Plural(CONJ_PREFIX));
   p2pLabel->setText  (prefix.pers2Plural(CONJ_PREFIX));
   p3pfLabel->setText (prefix.pers3FemalePlural(CONJ_PREFIX));
   p3pmLabel->setText (prefix.pers3MalePlural(CONJ_PREFIX));
   p3pnLabel->setText (prefix.pers3NaturalPlural(CONJ_PREFIX));
/*
   p1sAccel->setBuddy(p1sField);
   p2sAccel->setBuddy(p2sField);
   p3snAccel->setBuddy(p3snField);
   p3smAccel->setBuddy(p3smField);
   p3sfAccel->setBuddy(p3sfField);
*/
/*
   p1pLabel->setBuddy(p1pField);
   p2pLabel->setBuddy(p2pField);
   p3pnLabel->setBuddy(p3pnField);
   p3pmLabel->setBuddy(p3pmField);
   p3pfLabel->setBuddy(p3pfField);
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
   all_known = true;
   current = -1;
   next();
}


bool VerbQueryDlg::next()
{
  resetAllFields();
  QString s, type;
  if (q_ocol == 0)
    s = kv_exp->getOriginal();
  else
    s = kv_exp->getTranslation(q_ocol);

  if (current < (int) conjugations.numEntries()-1 )
    current++;

  type = conjugations.getType(current);
  QString format = i18n("Enter %1 for \"%2\"");
  QString msg = format.arg(conjugations.getName(type)).arg(s);

  groupTitle->setTitle (msg);

  p1sField->setText("");
  p1sField->setEnabled (!conjugations.pers1Singular(type).isEmpty());

  p2sField->setText("");
  p2sField->setEnabled (!conjugations.pers2Singular(type).isEmpty());

  p3sfField->setText("");
  p3sfField->setEnabled (!conjugations.pers3FemaleSingular(type).isEmpty());

  p3smField->setText("");
  p3smField->setEnabled (!conjugations.pers3MaleSingular(type).isEmpty());

  p3snField->setText("");
  p3snField->setEnabled (!conjugations.pers3NaturalSingular(type).isEmpty());

  p1pField->setText("");
  p1pField->setEnabled (!conjugations.pers1Plural(type).isEmpty());

  p2pField->setText("");
  p2pField->setEnabled (!conjugations.pers2Plural(type).isEmpty());

  p3pfField->setText("");
  p3pfField->setEnabled (!conjugations.pers3FemalePlural(type).isEmpty());

  p3pmField->setText("");
  p3pmField->setEnabled (!conjugations.pers3MalePlural(type).isEmpty());

  p3pnField->setText("");
  p3pnField->setEnabled (!conjugations.pers3NaturalPlural(type).isEmpty());

  bool common = conjugations.pers3SingularCommon(type);
  if (common) {
    p3smField->setEnabled(false);
    p3snField->setEnabled(false);
  }

  common = conjugations.pers3PluralCommon(type);
  if (common) {
    p3pmField->setEnabled(false);
    p3pnField->setEnabled(false);
  }

  return false;
}


void VerbQueryDlg::showAllClicked()
{
  resetAllFields();
  if (current >= (int) conjugations.numEntries()-1 ) {
    dont_know->setDefault(true);
  }
  else
    next();
 
  QString type = conjugations.getType (current);

  p1sField->setText  (conjugations.pers1Singular(type));
  p2sField->setText  (conjugations.pers2Singular(type));
  p3sfField->setText (conjugations.pers3FemaleSingular(type));
  p3smField->setText (conjugations.pers3MaleSingular(type));
  p3snField->setText (conjugations.pers3NaturalSingular(type));

  p1pField->setText  (conjugations.pers1Plural(type));
  p2pField->setText  (conjugations.pers2Plural(type));
  p3pfField->setText (conjugations.pers3FemalePlural(type));
  p3pmField->setText (conjugations.pers3MalePlural(type));
  p3pnField->setText (conjugations.pers3NaturalPlural(type));

  verifyField (p1sField, conjugations.pers1Singular(type));
  verifyField (p2sField, conjugations.pers2Singular(type));
  verifyField (p3sfField, conjugations.pers3FemaleSingular(type));

  bool common = conjugations.pers3SingularCommon(type);
  if (!common) {
    verifyField (p3smField, conjugations.pers3MaleSingular(type));
    verifyField (p3snField, conjugations.pers3NaturalSingular(type));
  }

  verifyField (p1pField, conjugations.pers1Plural(type));
  verifyField (p2pField, conjugations.pers2Plural(type));
  verifyField (p3pfField, conjugations.pers3FemalePlural(type));

  common = conjugations.pers3PluralCommon(type);
  if (!common) {
    verifyField (p3pmField, conjugations.pers3MalePlural(type));
    verifyField (p3pnField, conjugations.pers3NaturalPlural(type));
  }

}


void VerbQueryDlg::verifyClicked()
{
  QString type = conjugations.getType(current);

  bool all_known = true;

  if (!verifyField (p1sField, conjugations.pers1Singular(type)))
    all_known = false;

  if (!verifyField (p2sField, conjugations.pers2Singular(type)))
    all_known = false;

  if (!verifyField (p3sfField, conjugations.pers3FemaleSingular(type)))
    all_known = false;

  bool common = conjugations.pers3SingularCommon(type);
  if (!common) {
    if (!verifyField (p3smField, conjugations.pers3MaleSingular(type)))
      all_known = false;
  
    if (!verifyField (p3snField, conjugations.pers3NaturalSingular(type)))
      all_known = false;
  }

  if (!verifyField (p1pField, conjugations.pers1Plural(type)))
    all_known = false;

  if (!verifyField (p2pField, conjugations.pers2Plural(type)))
    all_known = false;

  if (!verifyField (p3pfField, conjugations.pers3FemalePlural(type)))
    all_known = false;

  common = conjugations.pers3PluralCommon(type);
  if (!common) {
    if (!verifyField (p3pmField, conjugations.pers3MalePlural(type)))
      all_known = false;
  
    if (!verifyField (p3pnField, conjugations.pers3NaturalPlural(type)))
      all_known = false;
  }

  if (all_known)
//    know_it->setDefault(true);
    knowItClicked();
  else
    dont_know->setDefault(true);
}


void VerbQueryDlg::resetAllFields()
{
  resetField(p1sField);
  resetField(p2sField);
  resetField(p3sfField);
  resetField(p3smField);
  resetField(p3snField);

  resetField(p1pField);
  resetField(p2pField);
  resetField(p3pfField);
  resetField(p3pmField);
  resetField(p3pnField);
}


void VerbQueryDlg::knowItClicked()
{
   resetAllFields();
   if (current >= (int) conjugations.numEntries()-1 ) {
     if (all_known)
       done (Known);
     else
       done (Unknown);
   }
   else
     next();
}


void VerbQueryDlg::timeoutReached()
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
     if (++current >= (int) conjugations.numEntries() )
       if (type_timeout == kvq_show) {
         showAllClicked();
         verifyClicked();
         dont_know->setDefault(true);
       }
       else if (type_timeout == kvq_cont)
         done (Timeout);
     else {
       next();
       qtimer->start(1000, TRUE);
       timercount = query_time/1000;
     }
   }
}


void VerbQueryDlg::dontKnowClicked()
{
   all_known = false;
   if (current >= (int) conjugations.numEntries()-1 )
     done (Unknown);
   else
     next();
}


void VerbQueryDlg::stopItClicked()
{
   done (StopIt);
}


void VerbQueryDlg::editClicked()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);
}


void VerbQueryDlg::keyPressEvent( QKeyEvent *e )
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


void VerbQueryDlg::slotP3pfChanged(const QString&)
{
  verify->setDefault(true);

  resetField(p3pfField);
}


void VerbQueryDlg::slotP3snChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p3snField);
}


void VerbQueryDlg::slotReturnPressed()
{
}


void VerbQueryDlg::slotP3smChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p3smField);
}


void VerbQueryDlg::slotP3pnChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p3pnField);
}


void VerbQueryDlg::slotP3sfChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p3sfField);
}


void VerbQueryDlg::slotP1sChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p1sField);
}


void VerbQueryDlg::slotP2sChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p2sField);
}


void VerbQueryDlg::slotP3pmChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p3pmField);
}


void VerbQueryDlg::slotP1pChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p1pField);
}


void VerbQueryDlg::slotP2pChanged(const QString&)
{
  verify->setDefault(true);
  resetField(p2pField);
}

#include "VerbQueryDlg.moc"
