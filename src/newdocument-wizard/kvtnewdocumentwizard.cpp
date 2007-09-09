/***************************************************************************

                             kvtnewdocumentwizard

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

#include "kvtnewdocumentwizard.h"

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <KLocale>
#include <KComboBox>


KVTNewDocumentWizard::KVTNewDocumentWizard(KEduVocDocument* doc, QWidget *parent)
 : QWizard(parent)
{
    m_doc = doc;

    resize(600,600);

    setPage(Intro, new KVTNewDocumentWizardIntroPage);

    setPage(AuthorTitle, new AuthorTitlePage(this));

    setPage(FirstIdentifierLanguage, new FirstIdentifierLanguagePage(this));

    setPage(SecondIdentifierLanguage, new SecondIdentifierLanguagePage(this));

    setPage(Final, new WizardFinalPage(this));

    setStartId(Intro);

    setWindowTitle(i18n("Create a New Parley Document"));
}


int KVTNewDocumentWizard::nextId() const
{
    switch (currentId()) {
    case Intro:
            return AuthorTitle;
    case AuthorTitle:
        return FirstIdentifierLanguage;
    case FirstIdentifierLanguage:
        return SecondIdentifierLanguage;
    case SecondIdentifierLanguage:
        return Final;
    case Final:
    default:
        return -1;
    }
}

// QList<WizardIdentifier> KVTNewDocumentWizard::identifiers()
// {
//     QList<WizardIdentifier> ident;
//     if ( hasVisitedPage(WizardFirstLanguagePage) ) {
//         kDebug() << "adding first page";
//         ident.append(WizardIdentifier(true,
//             field("firstLanguagePage.language").toString(),
//             field("firstLanguagePage.languageShort").toString() ));
//     }
//     if ( hasVisitedPage(WizardSecondLanguagePage) ) {
//         kDebug() << "adding second page";
//         ident.append(WizardIdentifier(true,
//             field("secondLanguagePage.language").toString(),
//             field("secondLanguagePage.languageShort").toString() ));
//     }
//     if ( hasVisitedPage(WizardOtherPage) ) {
//         ident.append(WizardIdentifier(false,
//             field("otherPage.firstIdentifier").toString(),
//             field("otherPage.firstIdentifier").toString() ));
//         ident.append(WizardIdentifier(false,
//             field("otherPage.secondIdentifier").toString(),
//             field("otherPage.secondIdentifier").toString() ));
//     }
//     return ident;
// }

void KVTNewDocumentWizard::accept()
{
    QString author = field("authorField").toString();
    m_doc->setAuthor(author);
    m_doc->setTitle( field("titleField").toString() );
    m_doc->setLicense( field("licenseField").toString() );
    m_doc->setDocumentComment( field("commentField").toString() );
    m_doc->setCategory( field("categoryField").toString() );

    m_doc->appendIdentifier();
    m_doc->appendIdentifier();
    m_doc->identifier(0).setName( field("firstIdentifierName").toString() );
    m_doc->identifier(0).setLocale( KGlobal::locale()->allLanguagesList().value(field("firstLocale").toInt()) );
    m_doc->identifier(1).setName( field("secondIdentifierName").toString() );
    m_doc->identifier(1).setLocale( KGlobal::locale()->allLanguagesList().value(field("secondLocale").toInt()) );


    QDialog::accept();
}




#include "kvtnewdocumentwizard.moc"
