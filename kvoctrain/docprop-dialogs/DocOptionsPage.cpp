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

#define Inherited DocOptionsPageData

#include <kapp.h>

#include <qkeycode.h>

struct EncRef {
          const char        *text;
          const KV_Encoding  num;
       };

static EncRef encode_list [] =
      {
        {KES_8BIT,      KE_8BIT},
        {KES_UTF8,      KE_UTF8},
        {0 ,            (KV_Encoding) 0}
      };

DocOptionsPage::DocOptionsPage
(
        bool         sort,
        KV_Encoding  encoding,
	QWidget     *parent,
 	const char  *name
)
	:
	Inherited( parent, name )
{
   setCaption(i18n("Options" ));
   sorter = sort;
   encoder = encoding;
   docsort->setChecked(sort);

   connect( c_coding, SIGNAL(highlighted(int)), SLOT(slotEncSelected(int)) );
   connect( docsort, SIGNAL(toggled(bool)), SLOT(docSortToggled(bool)) );

   EncRef *ref = encode_list;
   int index = 0;

   c_coding->clear();
   while (ref->text != 0 ) {
     c_coding->insertItem (i18n(ref->text));
     if (encoder == ref->num)
       index = ref - encode_list;
     ref++;
   }
   c_coding->setCurrentItem (index);
   slotEncSelected (index);
   l_coding->setBuddy (c_coding);
}


void DocOptionsPage::initFocus() const
{
  docsort->setFocus();
}


void DocOptionsPage::docSortToggled(bool state)
{
  sorter = state;
}


void DocOptionsPage::slotEncSelected(int i)
{
  encoder = encode_list[i].num;
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
