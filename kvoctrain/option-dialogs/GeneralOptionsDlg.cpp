/***************************************************************************

                     general options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

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


#include "GeneralOptionsDlg.h"

#include <langset.h>

#include <klocale.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kapplication.h>


#define Inherited QTabDialog


class kvoctraindoc;

GeneralOptionsDlg::GeneralOptionsDlg
(
        QString       deflang,
        QString       sep,
        int           btime,
        LangSet      &_langset,
        QString      &lastPix,
        QComboBox    *,
        QStringList   paste_list,
        bool          useCurrent,
        kvoctrainDoc *doc,
        QFont         tablefont,
        QFont         ipafont,
        QueryManager *manager,
        bool          smartAppend,
        bool          autoapply,
        QWidget      *parent,
        const char   *name
)
:
  Inherited( parent, name, true ), langset(_langset)
{
  setCaption(kapp->makeStdCaption(i18n("General Options")));

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  genOptPage =  new GenOptPage  (btime, smartAppend, autoapply, this, name);
  langOptPage = new LangOptPage (deflang, langset, lastPix, this, name);
  viewOptPage = new ViewOptPage (tablefont, ipafont, manager, this, name);
  pasteOptPage = new PasteOptPage (sep, langset, paste_list, useCurrent, doc, this, name);

  addTab( genOptPage,  i18n("&General"));
  addTab( langOptPage, i18n("L&anguage"));
  addTab( viewOptPage, i18n("&View "));   // keep SPACE after "View"
  addTab( pasteOptPage, i18n("&Clipboard"));

  connect (this, SIGNAL(aboutToShow()), genOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), langOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), viewOptPage, SLOT(initFocus() ));
  connect (this, SIGNAL(aboutToShow()), pasteOptPage, SLOT(initFocus() ));

  connect (this, SIGNAL(selected(const QString&)), pasteOptPage, SLOT(slotSyncLangSet(const QString&)));
  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );
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

    default:
      kdError() << "GeneralOptionsDlg::selectPage(int) index out of range" << endl;
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
