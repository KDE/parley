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
    Revision 1.4  2001/11/10 21:12:18  arnold
    removed icons and compatibilty for kde1

    Revision 1.3  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.2  2001/10/12 19:14:28  arnold
    switched dialog files to qt-designer

    Revision 1.1  2001/10/05 15:37:45  arnold
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

#include "ProgressDlg.h"

#include <kapp.h>

#include <qprogressbar.h>
#include <qlabel.h>

#include <kvoctraindoc.h>
#include <kstddirs.h>
#include <klocale.h>

ProgressDlg::ProgressDlg
(
        const QString &doctitle,
        const QString &filename,
        const QString &title,
	QWidget* parent,
	const char* name
)
	:
	ProgressDlgForm( parent, name )
{
     doc = 0;
     setCaption (title);
     l_title->setText (doctitle);
     l_file->setText (filename);
     setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
     progress-> setTotalSteps(100);
}


void ProgressDlg::setValue( kvoctrainDoc *new_doc, int val)
{
   progress->setProgress(val);
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


#include "ProgressDlg.moc"
