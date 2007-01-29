/***************************************************************************

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin         : Sat Dec 4 15:09:18 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QKeyEvent>

#include <kapplication.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include "AdjQueryDlg.h"
#include <kv_resource.h>
#include <kvtlanguages.h>

AdjQueryDlg::AdjQueryDlg
(
  QString type,
  int entry,
  int col,
  int query_cycle,
  int query_num,
  int query_startnum,
  KEduVocExpression *exp,
  KEduVocDocument  *doc,
  const KEduVocComparison &_comp)
  : QueryDlgBase(i18n("Comparison Training"))
{
  mw = new Ui::AdjQueryDlgForm();
  mw->setupUi(mainWidget());

  connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()) );
  connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()) );
  connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()) );
  connect(mw->show_all, SIGNAL(clicked()), SLOT(showAllClicked()) );

  connect(mw->lev1Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
  connect(mw->lev2Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
  connect(mw->lev3Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );

  connect(mw->lev1Field, SIGNAL(textChanged(const QString&)), SLOT(lev1Changed(const QString&)) );
  connect(mw->lev2Field, SIGNAL(textChanged(const QString&)), SLOT(lev2Changed(const QString&)) );
  connect(mw->lev3Field, SIGNAL(textChanged(const QString&)), SLOT(lev3Changed(const QString&)) );

  qtimer = 0;

  mw->lev1Label->setBuddy(mw->lev1Field);
  mw->lev2Label->setBuddy(mw->lev2Field);
  mw->lev3Label->setBuddy(mw->lev3Field);

  setQuery (type, entry, col, query_cycle, query_num, query_startnum, exp, doc, _comp);
  mw->countbar->setFormat("%v/%m");
  mw->timebar->setFormat("%v");

  KSharedConfig::Ptr cfg = KGlobal::config();
  cfg->setGroup("AdjQueryDlg");
  restoreDialogSize(cfg.data());
}


AdjQueryDlg::~AdjQueryDlg()
{
  KSharedConfig::Ptr cfg = KGlobal::config();
  cfg->setGroup("AdjQueryDialog");
  KDialog::saveDialogSize(cfg.data());
}


void AdjQueryDlg::setQuery(QString,
                           int entry,
                           int col,
                           int q_cycle,
                           int q_num,
                           int q_start,
                           KEduVocExpression *exp,
                           KEduVocDocument  *doc,
                           const KEduVocComparison &_comp)
{
   //type_timeout = type_to;
   kv_doc = doc;
   kv_exp = exp;
   q_row = entry;
   q_ocol = col;
   mw->timebar->setEnabled(Prefs::showCounter());
   mw->timelabel->setEnabled(Prefs::showCounter());
   comp = _comp;
   mw->show_all->setDefault(true);
   QString s;
   s.setNum (q_cycle);
   mw->progCount->setText (s);

   mw->lev1Field->setText ("");
   mw->lev2Field->setText ("");
   mw->lev3Field->setText ("");

   int sel = getRandom(3);
   switch (sel) {
     case 0: mw->lev1Field->setText (comp.l1() );
     break;

     case 1: mw->lev2Field->setText (comp.l2() );
     break;

     case 2: mw->lev3Field->setText (comp.l3() );
     break;
   }

   mw->lev1Field->setEnabled(!comp.l1().isEmpty() );
   mw->lev2Field->setEnabled(!comp.l2().isEmpty() );
   mw->lev3Field->setEnabled(!comp.l3().isEmpty() );

   mw->countbar->setMaximum(q_start);
   mw->countbar->setValue(q_start - q_num + 1);

   int mqtime = Prefs::maxTimePer();
   if (mqtime > 0) {
     if (qtimer == 0) {
       qtimer = new QTimer( this );
       qtimer->setSingleShot(true);
       connect( qtimer, SIGNAL(timeout()), this, SLOT(timeoutReached()) );
     }

     if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
       timercount = mqtime;
       mw->timebar->setMaximum(timercount);
       mw->timebar->setValue(timercount);
       qtimer->start(1000);
     }
     else
       mw->timebar->setEnabled(false);
   }
   else
     mw->timebar->setEnabled(false);
   resetAllFields();
}


void AdjQueryDlg::initFocus() const
{
  mw->lev1Field->setFocus();
}


void AdjQueryDlg::showAllClicked()
{
  resetAllFields();
  mw->lev1Field->setText (comp.l1() );
  mw->lev2Field->setText (comp.l2() );
  mw->lev3Field->setText (comp.l3() );

  verifyField (mw->lev1Field, comp.l1());
  verifyField (mw->lev2Field, comp.l2());
  verifyField (mw->lev3Field, comp.l3());

  mw->dont_know->setDefault(true);
}


void AdjQueryDlg::verifyClicked()
{
  bool all_known = true;

  if (!verifyField (mw->lev1Field, comp.l1()) )
    all_known = false;

  if (!verifyField (mw->lev2Field, comp.l2()) )
    all_known = false;

  if (!verifyField (mw->lev3Field, comp.l3()) )
    all_known = false;

  if (all_known)
//  know_it->setDefault(true);
    knowItClicked();
  else
    mw->dont_know->setDefault(true);
}


void AdjQueryDlg::resetAllFields()
{
  resetField(mw->lev1Field);
  resetField(mw->lev2Field);
  resetField(mw->lev3Field);
}


void AdjQueryDlg::knowItClicked()
{
  emit sigQueryChoice(Known);
}


void AdjQueryDlg::timeoutReached()
{
   if (timercount > 0) {
     timercount--;
     mw->timebar->setValue(timercount);
     qtimer->start(1000);
   }

   if (timercount <= 0) {
     mw->timebar->setValue(0);
     if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
       showAllClicked();
       mw->dont_know->setDefault(true);
     }
     else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
       emit sigQueryChoice(Timeout);
   }
}


void AdjQueryDlg::dontKnowClicked()
{
   emit sigQueryChoice(Unknown);
}


void AdjQueryDlg::slotUser2()
{

   if (qtimer != 0)
     qtimer->stop();

   emit sigEditEntry (q_row, KV_COL_ORG+q_ocol);
}


void AdjQueryDlg::keyPressEvent( QKeyEvent *e )
{
  switch( e->key() )
  {
    case Qt::Key_Escape:
      dontKnowClicked();
    break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
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


void AdjQueryDlg::returnPressed()
{
}


void AdjQueryDlg::lev1Changed(const QString&)
{
  mw->verify->setDefault(true);
  resetField (mw->lev1Field);
}


void AdjQueryDlg::lev2Changed(const QString&)
{
  mw->verify->setDefault(true);
  resetField (mw->lev2Field);
}


void AdjQueryDlg::lev3Changed(const QString&)
{
  mw->verify->setDefault(true);
  resetField (mw->lev3Field);
}


#include "AdjQueryDlg.moc"
