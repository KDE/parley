/***************************************************************************

    $Id$

                       general options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:08:56 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

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
#include <qvalidator.h>
#include <qkeycode.h>
#include <stdlib.h>
#include <kapp.h>

#include <QueryManager.h>

#define Inherited GenOptPageData

GenOptPage::GenOptPage
(
        int        _btime,
        kvoctrainView::Resizer res,
        bool        _smart,
        bool        _autosaveopts,
	QWidget    *parent,
	const char *name
)
	:
	Inherited( parent, name ),
        resizer(res)
{
  setCaption(i18n("Options" ));

  group_resize->insert(hb_auto);
  group_resize->insert(hb_percent);
  group_resize->insert(hb_fixed);

  connect( c_smart, SIGNAL(toggled(bool)), SLOT(slotSmartAppend(bool)) );
  connect( c_saveopt, SIGNAL(toggled(bool)), SLOT(slotAutoSaveOpts(bool)) );
  connect( hb_fixed, SIGNAL(clicked()), SLOT(slotHBfixed()) );
  connect( hb_percent, SIGNAL(clicked()), SLOT(slotHBpercent()) );
  connect( hb_auto, SIGNAL(clicked()), SLOT(slotHBauto()) );

  connect( e_btime, SIGNAL(textChanged(const char*)), SLOT(slotChangeBTime(const char*)) );

  btime = _btime;
  smart = _smart;
  autosaveopts = _autosaveopts;

  c_smart->setChecked(smart);
  c_saveopt->setChecked(autosaveopts);

  QString s;
  validator = new QIntValidator (0, 60*60*24*7, 0); // at least once a week

  e_btime->setValidator (validator);
  s.setNum (btime);
  e_btime->setText (s);
  label_btime->setBuddy (e_btime);

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
  e_btime->setFocus();
}


void GenOptPage::slotChangeBTime(const QString& s)
{
   btime = atoi (s.local8Bit());
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


void GenOptPage::slotSmartAppend(bool b)
{
  smart = b;
}


void GenOptPage::slotAutoSaveOpts(bool b)
{
  autosaveopts = b;
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
