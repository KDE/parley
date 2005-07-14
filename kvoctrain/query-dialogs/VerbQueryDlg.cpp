/***************************************************************************

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin          : Fri Dec 3 18:28:18 1999

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

#include "VerbQueryDlg.h"

#include <kv_resource.h>

#include <kapplication.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kprogress.h>

#include <qtimer.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>

VerbQueryDlg::VerbQueryDlg
(
  QString type,
        int entry,
        int col,
        int query_cycle,
        int query_num,
        int query_startnum,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        const Conjugation &prefix,
        const Conjugation &conjug)
 : QueryDlgBase(i18n("Verb Training"))
{
  mw = new VerbQueryDlgForm(this);
  setMainWidget(mw);

  connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
  connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
  connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
  connect(mw->show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );

  connect(mw->p3pmField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p3pnField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p3snField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p3smField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p3pfField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p3sfField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p2pField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p2sField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p1pField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );
  connect(mw->p1sField, SIGNAL(returnPressed()), SLOT(slotReturnPressed()) );

  connect(mw->p3pmField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pmChanged(const QString&)) );
  connect(mw->p3snField, SIGNAL(textChanged(const QString&)), SLOT(slotP3snChanged(const QString&)) );
  connect(mw->p3pnField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pnChanged(const QString&)) );
  connect(mw->p3smField, SIGNAL(textChanged(const QString&)), SLOT(slotP3smChanged(const QString&)) );
  connect(mw->p3pfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pfChanged(const QString&)) );
  connect(mw->p3sfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3sfChanged(const QString&)) );
  connect(mw->p2pField, SIGNAL(textChanged(const QString&)), SLOT(slotP2pChanged(const QString&)) );
  connect(mw->p2sField, SIGNAL(textChanged(const QString&)), SLOT(slotP2sChanged(const QString&)) );
  connect(mw->p1pField, SIGNAL(textChanged(const QString&)), SLOT(slotP1pChanged(const QString&)) );
  connect(mw->p1sField, SIGNAL(textChanged(const QString&)), SLOT(slotP1sChanged(const QString&)) );

  qtimer = 0;

  setQuery (type, entry, col, query_cycle, query_num, query_startnum, exp, doc, prefix, conjug);
  mw->countbar->setFormat("%v/%m");
  mw->timebar->setFormat("%v");
  resize(configDialogSize("VerbQueryDialog"));
}


VerbQueryDlg::~ VerbQueryDlg( )
{
  saveDialogSize("VerbQueryDialog");
}


void VerbQueryDlg::initFocus() const
{
  mw->p1sField->setFocus();
}


void VerbQueryDlg::setQuery(QString,
                            int entry,
                            int col,
                            int q_cycle,
                            int q_num,
                            int q_start,
                            kvoctrainExpr *exp,
                            kvoctrainDoc  *doc,
                            const Conjugation &prefix,
                            const Conjugation &conjug)
{
   //type_timeout = type_to;
   kv_doc = doc;
   kv_exp = exp;
   q_row = entry;
   q_ocol = col;
   int mqtime = Prefs::maxTimePer();
   mw->timebar->setEnabled(Prefs::showCounter());
   mw->timelabel->setEnabled(Prefs::showCounter());
   mw->show_all->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   mw->progCount->setText (s);

   conjugations = conjug;

   mw->p1sLabel->setText  (prefix.pers1Singular(CONJ_PREFIX));
   mw->p2sLabel->setText  (prefix.pers2Singular(CONJ_PREFIX));
   mw->p3sfLabel->setText (prefix.pers3FemaleSingular(CONJ_PREFIX));
   mw->p3smLabel->setText (prefix.pers3MaleSingular(CONJ_PREFIX));
   mw->p3snLabel->setText (prefix.pers3NaturalSingular(CONJ_PREFIX));

   mw->p1pLabel->setText  (prefix.pers1Plural(CONJ_PREFIX));
   mw->p2pLabel->setText  (prefix.pers2Plural(CONJ_PREFIX));
   mw->p3pfLabel->setText (prefix.pers3FemalePlural(CONJ_PREFIX));
   mw->p3pmLabel->setText (prefix.pers3MalePlural(CONJ_PREFIX));
   mw->p3pnLabel->setText (prefix.pers3NaturalPlural(CONJ_PREFIX));

   mw->countbar->setTotalSteps(q_start);
   mw->countbar->setProgress(q_start - q_num + 1);

   if (mqtime > 0) { // more than 1000 milli-seconds
     if (qtimer == 0) {
       qtimer = new QTimer( this );
       connect( qtimer, SIGNAL(timeout()), this, SLOT(timeoutReached()) );
     }

     if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
       timercount = mqtime;
       mw->timebar->setTotalSteps(timercount);
       mw->timebar->setProgress(timercount);
       qtimer->start(1000, TRUE);
     }
     else
       mw->timebar->setEnabled(false);
   }
   else
     mw->timebar->setEnabled(false);
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
  QString format = i18n("Current tense is: %1.");
  QString msg = format.arg(conjugations.getName(type));

  mw->instructionLabel->setText (msg);
  mw->baseLabel->setText (s);

  mw->p1sField->setText("");
  mw->p1sField->setEnabled (!conjugations.pers1Singular(type).isEmpty());

  mw->p2sField->setText("");
  mw->p2sField->setEnabled (!conjugations.pers2Singular(type).isEmpty());

  mw->p3sfField->setText("");
  mw->p3sfField->setEnabled (!conjugations.pers3FemaleSingular(type).isEmpty());

  mw->p3smField->setText("");
  mw->p3smField->setEnabled (!conjugations.pers3MaleSingular(type).isEmpty());

  mw->p3snField->setText("");
  mw->p3snField->setEnabled (!conjugations.pers3NaturalSingular(type).isEmpty());

  mw->p1pField->setText("");
  mw->p1pField->setEnabled (!conjugations.pers1Plural(type).isEmpty());

  mw->p2pField->setText("");
  mw->p2pField->setEnabled (!conjugations.pers2Plural(type).isEmpty());

  mw->p3pfField->setText("");
  mw->p3pfField->setEnabled (!conjugations.pers3FemalePlural(type).isEmpty());

  mw->p3pmField->setText("");
  mw->p3pmField->setEnabled (!conjugations.pers3MalePlural(type).isEmpty());

  mw->p3pnField->setText("");
  mw->p3pnField->setEnabled (!conjugations.pers3NaturalPlural(type).isEmpty());

  bool common = conjugations.pers3SingularCommon(type);
  if (common) {
    mw->p3smField->setEnabled(false);
    mw->p3snField->setEnabled(false);
  }

  common = conjugations.pers3PluralCommon(type);
  if (common) {
    mw->p3pmField->setEnabled(false);
    mw->p3pnField->setEnabled(false);
  }

  return false;
}


void VerbQueryDlg::showAllClicked()
{
  resetAllFields();
  mw->dont_know->setDefault(true);

  QString type = conjugations.getType (current);

  mw->p1sField->setText  (conjugations.pers1Singular(type));
  mw->p2sField->setText  (conjugations.pers2Singular(type));
  mw->p3sfField->setText (conjugations.pers3FemaleSingular(type));
  mw->p3smField->setText (conjugations.pers3MaleSingular(type));
  mw->p3snField->setText (conjugations.pers3NaturalSingular(type));

  mw->p1pField->setText  (conjugations.pers1Plural(type));
  mw->p2pField->setText  (conjugations.pers2Plural(type));
  mw->p3pfField->setText (conjugations.pers3FemalePlural(type));
  mw->p3pmField->setText (conjugations.pers3MalePlural(type));
  mw->p3pnField->setText (conjugations.pers3NaturalPlural(type));

  verifyField (mw->p1sField, conjugations.pers1Singular(type));
  verifyField (mw->p2sField, conjugations.pers2Singular(type));
  verifyField (mw->p3sfField, conjugations.pers3FemaleSingular(type));

  bool common = conjugations.pers3SingularCommon(type);
  if (!common) {
    verifyField (mw->p3smField, conjugations.pers3MaleSingular(type));
    verifyField (mw->p3snField, conjugations.pers3NaturalSingular(type));
  }

  verifyField (mw->p1pField, conjugations.pers1Plural(type));
  verifyField (mw->p2pField, conjugations.pers2Plural(type));
  verifyField (mw->p3pfField, conjugations.pers3FemalePlural(type));

  common = conjugations.pers3PluralCommon(type);
  if (!common) {
    verifyField (mw->p3pmField, conjugations.pers3MalePlural(type));
    verifyField (mw->p3pnField, conjugations.pers3NaturalPlural(type));
  }

}


void VerbQueryDlg::verifyClicked()
{
  QString type = conjugations.getType(current);

  bool known = true;

  if (!verifyField (mw->p1sField, conjugations.pers1Singular(type)))
    known = false;

  if (!verifyField (mw->p2sField, conjugations.pers2Singular(type)))
    known = false;

  if (!verifyField (mw->p3sfField, conjugations.pers3FemaleSingular(type)))
    known = false;

  bool common = conjugations.pers3SingularCommon(type);
  if (!common) {
    if (!verifyField (mw->p3smField, conjugations.pers3MaleSingular(type)))
      known = false;

    if (!verifyField (mw->p3snField, conjugations.pers3NaturalSingular(type)))
      known = false;
  }

  if (!verifyField (mw->p1pField, conjugations.pers1Plural(type)))
    known = false;

  if (!verifyField (mw->p2pField, conjugations.pers2Plural(type)))
    known = false;

  if (!verifyField (mw->p3pfField, conjugations.pers3FemalePlural(type)))
    known = false;

  common = conjugations.pers3PluralCommon(type);
  if (!common) {
    if (!verifyField (mw->p3pmField, conjugations.pers3MalePlural(type)))
      known = false;

    if (!verifyField (mw->p3pnField, conjugations.pers3NaturalPlural(type)))
      known = false;
  }

  if (known)
    knowItClicked();
  else {
    all_known = false;
    mw->dont_know->setDefault(true);
  }
}


void VerbQueryDlg::resetAllFields()
{
  resetField(mw->p1sField);
  resetField(mw->p2sField);
  resetField(mw->p3sfField);
  resetField(mw->p3smField);
  resetField(mw->p3snField);

  resetField(mw->p1pField);
  resetField(mw->p2pField);
  resetField(mw->p3pfField);
  resetField(mw->p3pmField);
  resetField(mw->p3pnField);
}


void VerbQueryDlg::timeoutReached()
{
   if (timercount > 0) {
     timercount--;
     mw->timebar->setProgress(timercount);
     qtimer->start(1000, TRUE);
   }

   if (timercount <= 0) {
     mw->timebar->setProgress(0);
     if (current >= (int) conjugations.numEntries()-1 ) {
       qtimer->stop();
       if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
         showAllClicked();
         mw->dont_know->setDefault(true);
       }
       else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
         emit sigQueryChoice (Timeout);
     }
     else {
       if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
         qtimer->stop();
         showAllClicked();
         mw->dont_know->setDefault(true);
       }
       else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue) {
         next();
         qtimer->start(1000, TRUE);
         timercount = Prefs::maxTimePer();
       }
     }
   }
}


void VerbQueryDlg::knowItClicked()
{
   resetAllFields();
   if (current >= (int) conjugations.numEntries()-1 ) {
     if (all_known)
       emit sigQueryChoice (Known);
     else
       emit sigQueryChoice (Unknown);
   }
   else
     next();
}


void VerbQueryDlg::dontKnowClicked()
{
   all_known = false;
   if (current >= (int) conjugations.numEntries()-1 )
     emit sigQueryChoice (Unknown);
   else {
     qtimer->start(1000, TRUE);
     timercount = Prefs::maxTimePer();
     next();
   }
}


void VerbQueryDlg::slotUser2()
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


void VerbQueryDlg::slotP3pfChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3pfField);
}


void VerbQueryDlg::slotP3snChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3snField);
}


void VerbQueryDlg::slotReturnPressed()
{
}


void VerbQueryDlg::slotP3smChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3smField);
}


void VerbQueryDlg::slotP3pnChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3pnField);
}


void VerbQueryDlg::slotP3sfChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3sfField);
}


void VerbQueryDlg::slotP1sChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p1sField);
}


void VerbQueryDlg::slotP2sChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p2sField);
}


void VerbQueryDlg::slotP3pmChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p3pmField);
}


void VerbQueryDlg::slotP1pChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p1pField);
}


void VerbQueryDlg::slotP2pChanged(const QString&)
{
  mw->verify->setDefault(true);
  resetField(mw->p2pField);
}

#include "VerbQueryDlg.moc"
