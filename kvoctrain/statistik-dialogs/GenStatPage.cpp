/***************************************************************************

    $Id$

                     general statistics dialog page

    -----------------------------------------------------------------------

    begin                : Thu Sep 21 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

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

#include "GenStatPage.h"

#include <kvoctraindoc.h>

#include <kapplication.h>

#include <qkeycode.h>
#include <qlabel.h>

GenStatPage::GenStatPage
(
        kvoctrainDoc *doc,
	QWidget* parent,
	const char* name
)
	:
	GenStatPageForm( parent, name )
{
    l_filename->setText(doc->getFileName());
    l_title->setText(doc->getTitle());
    l_author->setText(doc->getAuthor());
    QString s;
    s.setNum(doc->numEntries());
    l_entries->setText(s);
    vector<QString> lesson = doc->getLessonDescr();
    s.setNum(lesson.size());
    l_lessons->setText(s);
}


void GenStatPage::keyPressEvent( QKeyEvent *e )
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
#include "GenStatPage.moc"
