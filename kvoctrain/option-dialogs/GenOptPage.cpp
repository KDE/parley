/***************************************************************************

    $Id$

                       general options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:08:56 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "GenOptPage.h"
#include <qkeycode.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qvalidator.h>
#include <stdlib.h>
#include <kapplication.h>
#include <klocale.h>

#include <QueryManager.h>

GenOptPage::GenOptPage
(
        int        _btime,
        kvoctrainView::Resizer res,
        bool        _smart,
        bool        _autosaveopts,
        bool        _autoapply,
	QWidget    *parent,
	const char *name
)
	:
	GenOptPageForm( parent, name )
{
  setCaption(i18n("Options" ));
  resizer = res;
  chk_autoapply->setChecked(_autoapply);

  group_resize->insert(hb_auto);
  group_resize->insert(hb_percent);
  group_resize->insert(hb_fixed);

  connect( chk_autoapply, SIGNAL(toggled(bool)), this, SLOT(slotAutoApplyChecked(bool)) );
  connect( c_smart, SIGNAL(toggled(bool)), SLOT(slotSmartAppend(bool)) );
  connect( c_btime, SIGNAL(toggled(bool)), SLOT(slotBTimeUsed(bool)) );
  connect( c_saveopt, SIGNAL(toggled(bool)), SLOT(slotAutoSaveOpts(bool)) );
  connect( e_btime, SIGNAL(textChanged(const QString&)), SLOT(slotChangeBTime(const QString&)) );
  connect( hb_fixed, SIGNAL(clicked()), SLOT(slotHBfixed()) );
  connect( hb_percent, SIGNAL(clicked()), SLOT(slotHBpercent()) );
  connect( hb_auto, SIGNAL(clicked()), SLOT(slotHBauto()) );

  btime = _btime;
  smart = _smart;
  autosaveopts = _autosaveopts;

  c_smart->setChecked(smart);
  c_saveopt->setChecked(autosaveopts);

  QString s;
  validator = new QIntValidator (0, 60*60*24*7, 0); // at least once a week

  e_btime->setValidator (validator);
  c_btime->setChecked(btime > 0);
  slotBTimeUsed(btime > 0);

  switch (resizer) {
    case kvoctrainView::Automatic :
       hb_auto->setChecked( true );
    break;

    case kvoctrainView::Fixed :
       hb_fixed->setChecked( true );
    break;

    case kvoctrainView::Percent :
       hb_percent->setChecked( true );
    break;

    default: {
      hb_auto->setChecked( true );
      resizer = kvoctrainView::Automatic;
    }
  }
}


GenOptPage::~GenOptPage()
{
  delete validator;
}


void GenOptPage::initFocus() const
{
  c_btime->setFocus();
}


void GenOptPage::slotBTimeUsed(bool on)
{
  e_btime->setEnabled(on);

  if (on) {
    if (btime < 0 )
      btime = -btime;
    if (btime == 0)
      btime = 10;
  }
  else {
    if (btime > 0 )
      btime = -btime;
  }

  QString s;
  s.setNum (abs(btime));
  bool b = e_btime->signalsBlocked();
  e_btime->blockSignals(true);
  e_btime->setText (s);
  e_btime->blockSignals(b);
}


void GenOptPage::slotChangeBTime(const QString& s)
{
   btime = atoi (s.local8Bit());
}


void GenOptPage::slotSmartAppend(bool b)
{
  smart = b;
}


void GenOptPage::slotAutoSaveOpts(bool b)
{
  autosaveopts = b;
}


void GenOptPage::slotHBauto()
{
  resizer = kvoctrainView::Automatic;
}


void GenOptPage::slotHBfixed()
{
  resizer = kvoctrainView::Fixed;
}


void GenOptPage::slotHBpercent()
{
  resizer = kvoctrainView::Percent;
}


void GenOptPage::slotAutoApplyChecked(bool ena)
{
  autoapply = ena;
}


void GenOptPage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
}


#include "GenOptPage.moc"
