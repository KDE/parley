/***************************************************************************

    $Id$

                              progress dialog

    -----------------------------------------------------------------------

    begin                : Mon Aug 16 17:41:11 1999
                                           
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

#include "ProgressDlg.h"

#define Inherited ProgressDlgData


#include <kapp.h>

#include <kvoctraindoc.h>
#include <compat_2x.h>

ProgressDlg::ProgressDlg
(
        const QString &doctitle,
        const QString &filename,
        const QString &title,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
     doc = 0;
     setCaption (title);
     l_title->setText (doctitle);
     l_file->setText (filename);
     setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void ProgressDlg::setValue( kvoctrainDoc *new_doc, int val)
{
   progress->setValue(val);
   if (doc == 0 && new_doc != 0) {
     doc = new_doc;
     QString s = doc->getFileName();
     int pos;
     bool cut = false;
     while ((pos = s.find ('/')) >= 0) {
       s.remove (0, pos+1);
       cut = true;
     }
/*
     if (cut)
       s.insert (0, ".../");
*/
     l_title->setText (doc->getTitle());
     l_file->setText (s);
   }

}


