/***************************************************************************

                   document language dialog class

    -----------------------------------------------------------------------

    begin                : Sat Jun 2 20:50:53 MET 1999

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


#include "DocPropLangDlg.h"
#include "LangPropPage.h"

#include <kvoctraindoc.h>
#include <langset.h>

#include <kapplication.h>
#include <klocale.h>
#include <kstandarddirs.h>


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
  setCaption( kapp->makeStdCaption(i18n("Language Properties")));

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

    LangPropPage* lpp = new LangPropPage (doc, s,
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

  for (int i = 0; i < (int) own_tabs.size(); i++) {
    QString s = own_tabs[i];
    QPixmap pix (own_pixmaps[i]);
    QIconSet iconset (pix);
    addTab (langPages[i], iconset, s);
  }

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );
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
