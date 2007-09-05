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
#include "languagesettings.h"

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

    setCaption(i18n("Configure languages"));
    setFaceType( List );
    setButtons(User1|User2|Ok|Cancel);

    setButtonText(User2, i18n("Add language"));
//     setButtonIcon(User2, KIcon("insert_table_col"));
    setButtonText(User1, i18n("Remove language"));
//     setButtonIcon(User1, KIcon("delete_table_col"));

    connect( this, SIGNAL(user2Clicked()), this, SLOT(slotAppendIdentifier()));
    connect( this, SIGNAL(user1Clicked()), this, SLOT(slotDeleteIdentifier()));

    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {

        EditLanguageDialogPage* editPageWidget = new EditLanguageDialogPage(m_doc, i, this);

        KPageWidgetItem *editPage = new KPageWidgetItem( editPageWidget,  m_doc->identifier(i).name() );
        editPage->setHeader( i18nc("Edit language properties", "Properties for %1", m_doc->identifier(i).name() ) );


        // icons
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString currentIcon = currentSettings.icon();
        editPage->setIcon( KIcon( currentIcon ) );


        addPage( editPage );

        connect(this, SIGNAL(signalCommitData()), editPageWidget, SLOT(commitData()));

        connect(editPageWidget, SIGNAL(iconSelected(const QString&)), this, SLOT(pageIconChanged(const QString&)));
    }
}

LanguageDialog::~LanguageDialog()
{}

void LanguageDialog::accept()
{
    emit signalCommitData();
    QDialog::accept();
}

void LanguageDialog::slotAppendIdentifier()
{
    kDebug() << "Append identifier";

    int i = m_doc->appendIdentifier();

    EditLanguageDialogPage* editPageWidget = new EditLanguageDialogPage(m_doc, i, this);

    KPageWidgetItem *editPage = new KPageWidgetItem( editPageWidget,  m_doc->identifier(i).name() );
    editPage->setHeader( i18nc("Edit language properties", "Properties for %1", m_doc->identifier(i).name() ) );


    // icons
    LanguageSettings currentSettings(m_doc->identifier(i).locale());
    currentSettings.readConfig();
    QString currentIcon = currentSettings.icon();
    editPage->setIcon( KIcon( currentIcon ) );


    addPage( editPage );

    connect(this, SIGNAL(signalCommitData()), editPageWidget, SLOT(commitData()));

    setCurrentPage( editPage );
}

void LanguageDialog::slotDeleteIdentifier()
{
}

void LanguageDialog::pageIconChanged(const QString & newIcon)
{
    currentPage()->setIcon( KIcon(newIcon) );
}


#include "languagedialog.moc"
