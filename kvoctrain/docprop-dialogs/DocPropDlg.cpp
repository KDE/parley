/***************************************************************************

    $Id$

                       options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.9  2003/02/27 00:41:21  antlarr
    Made _many_ changes to kvoctrain when trying to fix the comparisons to "" and found
    some _very_ strange uses of QString and i18n, together with some very wrong
    and strange usages.

    CCMAIL:ewald@ewald-arnold.de

    Revision 1.8  2002/01/04 21:07:58  binner
    CVS_SILENT Fixed capitalisation.

    Revision 1.7  2001/12/26 15:11:08  mueller
    CVSSILINT: fixincludes

    Revision 1.6  2001/11/10 22:27:55  arnold
    removed compatibility for kde1

    Revision 1.5  2001/11/10 21:12:40  arnold
    removed icons and compatibility for kde1

    Revision 1.4  2001/10/21 15:22:00  arnold
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


#include "DocPropDlg.h"
#include "DocOptionsPage.h"
#include "TitlePage.h"
#include "TypeOptPage.h"
#include "LangPropPage.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <langset.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <qcombobox.h>
#include <qpixmap.h>

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
