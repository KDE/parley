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
    Revision 1.7  2001/11/10 22:29:11  arnold
    removed compatibility for kde1

    Revision 1.6  2001/11/02 10:18:58  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.5  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.4  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.3  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.2  2001/10/05 17:34:55  arnold
    FIX: slot signature changed to KDE2

    Revision 1.1  2001/10/05 15:44:04  arnold
    import of version 0.7.0pre8 to kde-edu


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
	QWidget    *parent,
	const char *name
)
	:
	GenOptPageForm( parent, name )
{
  setCaption(i18n("Options" ));
  resizer = res;

  group_resize->insert(hb_auto);
  group_resize->insert(hb_percent);
  group_resize->insert(hb_fixed);

  connect( c_smart, SIGNAL(toggled(bool)), SLOT(slotSmartAppend(bool)) );
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
