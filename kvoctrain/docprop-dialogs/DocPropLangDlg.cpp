/***************************************************************************

                   document language dialog class

    -----------------------------------------------------------------------

    begin          : Sat Jun 2 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#include <qstringlist.h>
#include <qlayout.h>
#include <qvaluelist.h>
#include <qtabwidget.h>

#include <kapplication.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include <kvoctraindoc.h>
#include <langset.h>


class kvoctraindoc;

DocPropsLangDlg::DocPropsLangDlg(kvoctrainDoc *doc, LangSet *langset, QWidget *parent, const char *name, bool modal)
  :
  KDialogBase(Tabbed, i18n("Language Properties"), Ok|Cancel, Ok, parent, name, modal)
{
  QFrame * page;
  QVBoxLayout * topLayout;
  LangPropPage* lpp;

  for (int i = 0; i < (int) doc->numLangs(); i++)
  {
    QString s;
    if (i == 0)
      s = doc->getOriginalIdent();
    else
      s = doc->getIdent(i);

    int idx = langset->indexShortId(s);

    QString tabCaption;

    if (idx >= 0)
      tabCaption = (langset->longId(idx));
    else
      tabCaption = (s);

    page = addPage(s, s, QPixmap(langset->PixMapFile(idx))); ///@todo The pixmaps don't show up in tabbed dialog
    topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
    lpp = new LangPropPage (doc, s, doc->getConjugation(i), doc->getArticle(i), page, name);
    topLayout->addWidget( lpp );

    langPages.append (lpp);
  }
}


Conjugation DocPropsLangDlg::getConjugation(int idx) const
{
  if (idx < (int) langPages.size())
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

#include "DocPropLangDlg.moc"
