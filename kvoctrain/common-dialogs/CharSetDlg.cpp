/***************************************************************************

    $Id$

                     substitute charset for document

    -----------------------------------------------------------------------

    begin                : Mon Jun 12 09:51:32 2000
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/12 19:13:50  arnold
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

#include "CharSetDlg.h"

#include <kapp.h>

#include <kvoctraindoc.h>
#include "../kvoctrain.h"

#include <qlistbox.h>
#include <qlineedit.h>

CharSetDlg::CharSetDlg
    (
        bool          flag,
        QString       message,
        QStrList     &rawnames,
        QWidget      *parent,
        const char   *name
    )  : CharSetDlgForm( parent, name, true )
{
   setCaption (kvoctrainApp::generateCaption(i18n("Choose charset"), true));
   connect( listbox, SIGNAL(selected(int)), SLOT(slotSelected(int)));
   connect( e_sample, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( b_cancel, SIGNAL(clicked()), SLOT(reject()) );
   connect( b_ok, SIGNAL(clicked()), SLOT(accept()) );
   connect( listbox, SIGNAL(highlighted(int)), SLOT(slotItemSelected(int)) );

   b_ok->setDefault(true);

   l_sample->setBuddy(e_sample);
   l_charset->setBuddy(listbox);

   sel = 0;
   QString prev_name;
   QString act_name;
   for (int i = 0; i < (int) rawnames.count(); i++) {
     QFont font;
     QString qfontname = rawnames.at(i);
     int dash = qfontname.find ('-', 1, true); // find next dash
     if (dash == -1) continue;
     QString vendor = qfontname.mid(1, dash -1);
     if (vendor.length() > 0)
       vendor.replace(0, 1, vendor.left(1).upper());

     // the font name is between the second and third dash so:
     // let's find the third dash:
  
     int dash_two = qfontname.find ('-', dash + 1 , true);
     if (dash == -1) continue;
     // fish the name of the font info string
     QString family = qfontname.mid(dash +1, dash_two - dash -1);
     font.setFamily(family);

     // get the charset...
     dash = qfontname.findRev('-');
     dash = qfontname.findRev('-', dash-1);
     QString xname = qfontname.right(qfontname.length()-dash-1);
     font.setCharSet (kvoctrainDoc::XName2CharSet(xname));
     font.setPointSize(14);

     if (family.length() > 0)
       family.replace(0, 1, family.left(1).upper());
     act_name = family+" ("+vendor+")";
     if (prev_name != act_name) {
       prev_name = act_name;
       listbox->insertItem (act_name);
       fonts.push_back(font);
     }
   }

   l_text->setText (message);

   listbox->setFocus();
   listbox->setCurrentItem (sel);

   l_pixmap->setPixmap(QPixmap(EA_KDEDATADIR("", "kvoctrain/fonts.xpm")));
   e_sample->setText ("The quick brown fox jumps over öäüÖÄÜß");
   e_sample->setFont (fonts[sel]);

   setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void CharSetDlg::slotItemSelected (int item)
{
   sel = item;
   if (sel < (int) fonts.size() )
     e_sample->setFont (fonts[sel]);
}


QFont CharSetDlg::getFont () const
{
  if (sel < (int) fonts.size() )
    return fonts[sel];
  else
    return QFont();
}


void CharSetDlg::slotSelected(int)
{
   accept();
}

#include "CharSetDlg.moc"
