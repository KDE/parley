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
#include <qvalidator.h>
#include <qkeycode.h>
#include <stdlib.h>
#include <kapp.h>

#include <QueryManager.h>

#define Inherited GenOptPageData

GenOptPage::GenOptPage
(
        int        _btime,
        bool        _smart,
        bool        _autosaveopts,
	QWidget    *parent,
	const char *name
)
	:
	Inherited( parent, name )
{
  setCaption(i18n("Options" ));

  connect( c_smart, SIGNAL(toggled(bool)), SLOT(slotSmartAppend(bool)) );
  connect( c_saveopt, SIGNAL(toggled(bool)), SLOT(slotAutoSaveOpts(bool)) );

  connect( e_btime, SIGNAL(textChanged(const QString&)), SLOT(slotChangeBTime(const QString&)) );

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
