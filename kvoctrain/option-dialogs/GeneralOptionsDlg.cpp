/***************************************************************************

    $Id$

                     general options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.8  2001/12/13 18:40:12  arnold
    added phonetic alphabet stuff

    Revision 1.7  2001/11/10 22:29:11  arnold
    removed compatibility for kde1

    Revision 1.6  2001/11/10 21:13:42  arnold
    removed icons and compatibilty for kde1

    Revision 1.5  2001/11/02 10:18:58  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.4  2001/10/25 18:04:26  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:44:04  arnold
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


#include "GeneralOptionsDlg.h"

#include <kv_resource.h>
#include <langset.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>

#include <qcombobox.h>
#include <qpixmap.h>

#define Inherited QTabDialog


class kvoctraindoc;

GeneralOptionsDlg::GeneralOptionsDlg
(
        QString       deflang,
        QString       sep,
        int           btime,
        LangSet      &_langset,
        QString      &lastPix,
        QComboBox    *lessons,
        QStringList  &paste_list,
        bool          useCurrent,
        kvoctrainDoc *doc,
        QFont        &tablefont,
        QFont        &ipafont,
        QueryManager *manager,
        GradeCols    &gradecols,
        kvoctrainView::Resizer resizer,
        bool          smartAppend,
        bool          autosaveopts,
	QWidget      *parent,
	const char   *name
)
	:
	Inherited( parent, name, true ),
        langset(_langset)
{
  setCaption(kapp->makeStdCaption(i18n("General Options")));

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  genOptPage =  new GenOptPage  (btime, resizer, smartAppend, autosaveopts, this, name);
  langOptPage = new LangOptPage (deflang, langset, lastPix, this, name);
  viewOptPage = new ViewOptPage (tablefont, ipafont, gradecols, manager, this, name);
  pasteOptPage = new PasteOptPage (sep, langset, paste_list, useCurrent, doc, this, name);

  addTab( genOptPage,  i18n( "&General" ));
  addTab( langOptPage, i18n( "L&anguage" ));
  addTab( viewOptPage, i18n( "&View " ));   // keep SPACE after "View"
  addTab( pasteOptPage, _OptDlg_CLIPBOARD);

  connect (this, SIGNAL(aboutToShow()), genOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), langOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), viewOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), pasteOptPage, SLOT(initFocus() ));

  connect (this, SIGNAL(selected(const QString&)), pasteOptPage, SLOT(slotSyncLangSet(const QString&)));
  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );

  setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void GeneralOptionsDlg::selectPage(int index)
{
  switch (index) {
    case 0: showPage(genOptPage);
    break;

    case 1: showPage(langOptPage);
    break;

    case 2: showPage(viewOptPage);
    break;

    case 3: showPage(pasteOptPage);
    break;

  }
}


void GeneralOptionsDlg::cancelButton()
{
  emit reject();
}


void GeneralOptionsDlg::okButton()
{
  emit accept();
}

#include "GeneralOptionsDlg.moc"
