/***************************************************************************

    $Id$

                   internal doc options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 12:00:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:38:38  arnold
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


#include "DocOptionsPage.h"

#include <kapp.h>

#include <qkeycode.h>
#include <qcheckbox.h>

DocOptionsPage::DocOptionsPage
(
        bool         sort,
	QWidget     *parent,
 	const char  *name
)
	:
	DocOptionsPageForm( parent, name )
{
   setCaption(i18n("Options" ));
   sorter = sort;
   docsort->setChecked(sort);

   connect( docsort, SIGNAL(toggled(bool)), SLOT(docSortToggled(bool)) );
}


void DocOptionsPage::initFocus() const
{
  docsort->setFocus();
}


void DocOptionsPage::docSortToggled(bool state)
{
  sorter = state;
}


void DocOptionsPage::keyPressEvent( QKeyEvent *e )
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
#include "DocOptionsPage.moc"
