/***************************************************************************

    $Id$

                   enter document title and author

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
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

#include "TitlePage.h"

#include <kv_resource.h>

#include "../kvoctrain.h"

#include <kapp.h>

#include <qkeycode.h>

#define Inherited TitlePageData

TitlePage::TitlePage
(
        QString  _title,
        QString  _author,
        QString  _license,
        QString  _doc_remark,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	connect( e_title, SIGNAL(returnPressed()), SLOT(accept()) );
//	connect( e_remark, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( e_license, SIGNAL(returnPressed()), SLOT(accept()) );
//	connect( e_author, SIGNAL(returnPressed()), SLOT(accept()) );

        setCaption (kvoctrainApp::generateCaption(i18n("Input title and author")));
        title = _title;
        author = _author;
        license = _license;
        doc_remark = _doc_remark;

        e_author->setText (author);
        label_author->setBuddy(e_author);

        e_title->setText (title);
   	e_title->setFocus();
	e_title->selectAll();
        label_title->setBuddy(e_title);

        e_license->setText (license);
        label_license->setBuddy(e_license);

        e_remark->setText (doc_remark);
        label_remark->setBuddy(e_remark);

	connect( e_author, SIGNAL(textChanged()),  SLOT(slotAuthorChanged()) );
	connect( e_title, SIGNAL(textChanged(const QString&)),   SLOT(slotTitleChanged(const QString&)) );
	connect( e_license, SIGNAL(textChanged(const QString&)), SLOT(slotLicenseChanged(const QString&)) );
	connect( e_remark, SIGNAL(textChanged()),  SLOT(slotDocRemarkChanged()) );
}


void TitlePage::initFocus() const
{
  e_title->setFocus();
}


void TitlePage::slotTitleChanged(const QString& s)
{
  title = s;
}


void TitlePage::slotAuthorChanged()
{
  author = e_author->text();
}


void TitlePage::slotLicenseChanged(const QString& s)
{
  license = s;
}


void TitlePage::slotDocRemarkChanged()
{
  doc_remark = e_remark->text();
}


void TitlePage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
}
#include "TitlePage.moc"
