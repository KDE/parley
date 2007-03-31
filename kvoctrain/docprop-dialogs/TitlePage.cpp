/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#include <keduvocdocument.h>

#include "TitlePage.h"

TitlePage::TitlePage(KEduVocDocument * doc, QWidget* parent) : QWidget(parent)
{
    setupUi(this);

    title = doc->title();
    author = doc->author();
    license = doc->license();
    doc_remark = doc->documentRemark();

    e_title->setText(title);
    e_title->setFocus();
    e_title->selectAll();

    e_author->setText(author);
    e_license->setText(license);
    e_remark->setText(doc_remark);

    connect(e_title,   SIGNAL(textChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&)));
    connect(e_author,  SIGNAL(textChanged()),               this, SLOT(slotAuthorChanged()));
    connect(e_license, SIGNAL(textChanged(const QString&)), this, SLOT(slotLicenseChanged(const QString&)));
    connect(e_remark,  SIGNAL(textChanged()),               this, SLOT(slotDocRemarkChanged()));
}


void TitlePage::slotTitleChanged(const QString& s)
{
    title = s;
}


void TitlePage::slotAuthorChanged()
{
    author = e_author->toPlainText();
}


void TitlePage::slotLicenseChanged(const QString& s)
{
    license = s;
}


void TitlePage::slotDocRemarkChanged()
{
    doc_remark = e_remark->toPlainText();
}

#include "TitlePage.moc"
