/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "TitlePage.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#include <keduvocdocument.h>

TitlePage::TitlePage(KEduVocDocument * doc, QWidget* parent) : QWidget(parent)
{
    m_doc = doc;
    setupUi(this);

    titleLineEdit->setText(doc->title());
    authorTextEdit->setText(doc->author());
    licenseComboBox->setEditText(doc->license());
    commentTextEdit->setText(doc->documentComment());
    categoryComboBox->setEditText(doc->category());
}


void TitlePage::commitData()
{
    m_doc->setTitle(titleLineEdit->text());
    m_doc->setAuthor(authorTextEdit->toPlainText());
    m_doc->setLicense(licenseComboBox->currentText());
    m_doc->setDocumentComment(commentTextEdit->toPlainText());
    m_doc->setCategory(categoryComboBox->currentText());
}

#include "TitlePage.moc"
