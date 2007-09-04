/***************************************************************************

                             kvtnewdocumentwizardlanguagepage

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "languagedialog.h"

#include "editlanguagedialogpage.h"

#include <keduvocdocument.h>

#include <QLabel>
#include <KLocale>


/**
 *
 * @param parent
 */
LanguageDialog::LanguageDialog(KEduVocDocument* doc, QWidget * parent)
    : KPageDialog(parent)
{
    m_doc = doc;

    setFaceType( List );

    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {

        EditLanguageDialogPage* editPageWidget = new EditLanguageDialogPage(m_doc, i, this);

        KPageWidgetItem *editPage = new KPageWidgetItem( editPageWidget,  m_doc->identifier(i).name() );
        editPage->setHeader( i18nc("Edit language properties", "Properties for %1", m_doc->identifier(i).name() ) );

        ///@todo get the icon
        editPage->setIcon( KIcon( ) );


        addPage( editPage );

        connect(this, SIGNAL(signalCommitData()), editPageWidget, SLOT(commitData()));
    }
}

LanguageDialog::~LanguageDialog()
{}

void LanguageDialog::accept()
{
    emit signalCommitData();
    QDialog::accept();
}


#include "languagedialog.moc"
