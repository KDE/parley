/***************************************************************************

    $Id$

                      statistics dialog class

    -----------------------------------------------------------------------

    begin                : Sun Sep 19 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.3  2001/10/17 21:41:16  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:30  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:47:12  arnold
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


#include "StatistikDlg.h"
#include "StatistikPage.h"
#include "GenStatPage.h"

#include <kapp.h>
#include <qcombobox.h>

#include <compat_2x.h>
#include <langset.h>
#include <kvoctraindoc.h>
#include <kv_resource.h>

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
  setCaption( kapp->makeStdCaption(i18n("Document statistics")));

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

  setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void StatistikDlg::okButton()
{
  emit accept();
}
#include "StatistikDlg.moc"
