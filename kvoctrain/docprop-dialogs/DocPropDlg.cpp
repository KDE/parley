/***************************************************************************

    $Id$

                       options dialog class

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


#include "DocPropDlg.h"

#include <langset.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>

#define Inherited QTabDialog


class kvoctraindoc;

DocPropsDlg::DocPropsDlg
(
        kvoctrainDoc    *doc,
        const char      *start_page,
        QComboBox       *lessons,
        QString          title,
        QString          author,
        QString          license,
        QString          doc_remark,
        vector<QString>  types,
        vector<QString>  tenses,
        vector<QString>  usages,
	QWidget         *parent,
	const char      *name
)
	:
	Inherited( parent, name, true )
{
  setCaption( kapp->makeStdCaption(i18n("Document Properties")));

  lessOptPage = new LessOptPage (lessons, doc, this, name);
  titleOptPage = new TitlePage (title, author, license, doc_remark, this, name);
  typeOptPage = new TypeOptPage (types, doc, this, name);
  tenseOptPage = new TenseOptPage (tenses, doc, this, name);
  useOptPage = new UsageOptPage (usages, doc, this, name);
  docOptPage = new DocOptionsPage (doc->isAllowedSorting(), this, name);
  addTab( titleOptPage, i18n("&General"));
  connect (this, SIGNAL(aboutToShow()), titleOptPage, SLOT(initFocus() ));

  addTab( lessOptPage, i18n("L&essons"));
  connect (this, SIGNAL(aboutToShow()), lessOptPage, SLOT(initFocus() ));

  addTab( typeOptPage, i18n("word types","T&ypes"));
  connect (this, SIGNAL(aboutToShow()), typeOptPage, SLOT(initFocus() ));

  addTab( tenseOptPage, i18n("Te&nses"));
  connect (this, SIGNAL(aboutToShow()), tenseOptPage, SLOT(initFocus() ));

  addTab( useOptPage, i18n("usage (area) of an expression", "&Usage"));
  connect (this, SIGNAL(aboutToShow()), useOptPage, SLOT(initFocus() ));

  addTab( docOptPage, i18n("&Options"));
  connect (this, SIGNAL(aboutToShow()), docOptPage, SLOT(initFocus() ));

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );

  setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void DocPropsDlg::cancelButton()
{
  emit reject();
}


void DocPropsDlg::okButton()
{
  emit accept();
}

#include "DocPropDlg.moc"
