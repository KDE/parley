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
#include "../kvoctrain.h"
#include "../rowtable.h"

#include <kv_resource.h>
#include <kvoctraindoc.h>
#include <langset.h>
#include <compat_2x.h>

#include <kapp.h>
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
  setCaption( kvoctrainApp::generateCaption(i18n("Document properties"), true));

  lessOptPage = new LessOptPage (lessons, doc, this, name);
  titleOptPage = new TitlePage (title, author, license, doc_remark, this, name);
  typeOptPage = new TypeOptPage (types, doc, this, name);
  tenseOptPage = new TenseOptPage (tenses, doc, this, name);
  useOptPage = new UsageOptPage (usages, doc, this, name);
  docOptPage = new DocOptionsPage (doc->isAllowedSorting(),
                                   doc->getEncoding(), this, name);
  vector<QString> tabs;
  addTab( titleOptPage, _DocDlg_TITLE );
  tabs.push_back(_DocDlg_TITLE);
  connect (this, SIGNAL(aboutToShow()), titleOptPage, SLOT(initFocus() ));

  addTab( lessOptPage, _DocDlg_LESSONS);
  tabs.push_back(_DocDlg_LESSONS);
  connect (this, SIGNAL(aboutToShow()), lessOptPage, SLOT(initFocus() ));

  addTab( typeOptPage, _DocDlg_TYPES);
  tabs.push_back(_DocDlg_TYPES );
  connect (this, SIGNAL(aboutToShow()), typeOptPage, SLOT(initFocus() ));

  addTab( tenseOptPage, _DocDlg_TENSES);
  tabs.push_back(_DocDlg_TENSES );
  connect (this, SIGNAL(aboutToShow()), tenseOptPage, SLOT(initFocus() ));

  addTab( useOptPage, _DocDlg_USAGES);
  tabs.push_back(_DocDlg_USAGES );
  connect (this, SIGNAL(aboutToShow()), useOptPage, SLOT(initFocus() ));

  addTab( docOptPage, _DocDlg_OPTIONS);
  tabs.push_back(_DocDlg_OPTIONS );
  connect (this, SIGNAL(aboutToShow()), docOptPage, SLOT(initFocus() ));

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );

  resize( 600+16,290+80 );
  setMinimumSize( 600+16,370+80 );
  setMaximumSize( 600+16,370+80 );

  setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void DocPropsDlg::cancelButton()
{
  emit reject();
}


void DocPropsDlg::okButton()
{
  emit accept();
}

