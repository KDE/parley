/***************************************************************************

    $Id$

                   document language dialog class

    -----------------------------------------------------------------------

    begin                : Sat Jun 2 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

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


#include "DocPropLangDlg.h"
#include "LangPropPage.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <langset.h>
#include <compat_2x.h>

#include <../rowtable.h>

#include <kapp.h>
#include <qcombobox.h>
#include <qpixmap.h>

#define Inherited QTabDialog

class kvoctraindoc;

DocPropsLangDlg::DocPropsLangDlg
(
        kvoctrainDoc    *doc,
        LangSet         *langset,
	QWidget         *parent,
	const char      *name
)
	:
	Inherited( parent, name, true )
{
  setCaption( kapp->makeStdCaption(i18n("Language properties")));

  vector<QString> tabs;
  vector<QString> own_tabs;
  vector<QString> own_pixmaps;
  for (int i = 0; i < (int) doc->numLangs(); i++) {
    QString s;
    if (i == 0)
      s = doc->getOriginalIdent();
    else
      s = doc->getIdent(i);

    int idx = langset->indexShortId(s);

    SpecFont_t font;
    if (idx >= 0)
      langset->Font(idx, font.font, font.specfont);
    else
      font.specfont = false; // FIXME:: use table font

    LangPropPage* lpp = new LangPropPage (&font, doc, s,
                                          doc->getConjugation(i),
                                          doc->getArticle(i)
					  );
    connect (this, SIGNAL(aboutToShow()), lpp, SLOT(initFocus() ));

    langPages.push_back (lpp);

    own_pixmaps.push_back (langset->PixMapFile(idx));
    if (idx >= 0)
      own_tabs.push_back (langset->longId(idx));
    else
      own_tabs.push_back (s);
  }

  int accel;
  for (int i = 0; i < (int) own_tabs.size(); i++) {
    QString s = own_tabs[i];
    if (RowTable::createMenuNames(tabs, own_tabs, i, accel))
      s.insert (accel, "&");
    QPixmap pix (own_pixmaps[i]);
    QIconSet iconset (pix);
    addTab (langPages[i], iconset, s);

  }

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );

  resize( 600+16,290+76 );
  setMinimumSize( 600+16,370+76 );
  setMaximumSize( 600+16,370+76 );

  setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


Conjugation DocPropsLangDlg::getConjugation(int idx) const
{
   if (idx < (int) langPages.size() )
     return langPages[idx]->getConjugation();
   else
     return Conjugation();
}


Article DocPropsLangDlg::getArticle(int idx) const
{
   if (idx < (int) langPages.size() )
     return langPages[idx]->getArticle();
   else
     return Article();
}

void DocPropsLangDlg::cancelButton()
{
  emit reject();
}


void DocPropsLangDlg::okButton()
{
  emit accept();
}

#include "DocPropLangDlg.moc"
