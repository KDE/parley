/***************************************************************************

    $Id$

                      statistics dialog class

    -----------------------------------------------------------------------

    begin                : Sun Sep 19 20:50:53 MET 1999
                                           
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


#include "StatistikDlg.h"
#include "StatistikPage.h"
#include "GenStatPage.h"

#include <kapplication.h>
#include <klocale.h>
#include <kstandarddirs.h>


#include <langset.h>
#include <kvoctraindoc.h>

#define Inherited QTabDialog

StatistikDlg::StatistikDlg
(
        LangSet         &langset,
        kvoctrainDoc    *doc,
        GradeCols       *gc,
	QWidget         *parent,
	const char      *name
)
	:
	Inherited( parent, name, true )
{
  setCaption( kapp->makeStdCaption(i18n("Document Statistics")));

  GenStatPage *gspage = new GenStatPage (doc, this, name);
  addTab( gspage, i18n("General"));
  setOkButton(i18n("&OK"));

  for (int i = 1; i < (int) doc->numLangs(); i++) {
    StatistikPage *spage = new StatistikPage (i, doc, gc, this, name);
    QString s = langset.findLongId(doc->getIdent(i));
    if (s.isEmpty() )
      s = doc->getIdent(i);
    else
      s = i18n(s.local8Bit());
    addTab( spage, s);
  }

  setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void StatistikDlg::okButton()
{
  emit accept();
}
#include "StatistikDlg.moc"
