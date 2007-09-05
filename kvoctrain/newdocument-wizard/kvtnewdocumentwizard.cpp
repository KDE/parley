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
//     if ( options == ShowFileOpen ) {
//         m_showFileOpen = true;
//     } else {
//         m_showFileOpen = false;
//     }

    resize(600,600);
//     m_firstLanguagePage = new KVTNewDocumentWizardLanguagePage(true, this);
//     m_secondLanguagePage = new KVTNewDocumentWizardLanguagePage(false, this);

    setPage(WizardIntroPage, new KVTNewDocumentWizardIntroPage);

    setPage(WizardAuthorTitlePage, new AuthorTitlePage(this));

    setPage(WizardFirstIdentifierLanguagePage, new FirstIdentifierLanguagePage(this));

    setPage(WizardSecondIdentifierLanguagePage, new SecondIdentifierLanguagePage(this));


//     setPage(WizardChoiceLanguageOtherPage, new KVTNewDocumentWizardChoiceLanguageOther ( m_showFileOpen, this ) );
//
//     setPage(WizardFirstLanguagePage, m_firstLanguagePage);
//     setPage(WizardSecondLanguagePage, m_secondLanguagePage);
//
//     setPage(WizardOtherPage, new KVTNewDocumentWizardOtherPage);


    setPage(WizardFinalPage, new KVTNewDocumentWizardFinalPage);

    setStartId(WizardIntroPage);

    //setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(i18n("Create a new KVocTrain document"));
}


// // void KVTNewDocumentWizard::initializePage(int id)
// // {
// //     QWizard::initializePage(id);
// // }


int KVTNewDocumentWizard::nextId() const
{
    switch (currentId()) {
    case WizardIntroPage:
            return WizardAuthorTitlePage;
    case WizardAuthorTitlePage:
        return WizardFirstIdentifierLanguagePage;
    case WizardFirstIdentifierLanguagePage:
        return WizardSecondIdentifierLanguagePage;
    case WizardSecondIdentifierLanguagePage:
        return WizardFinalPage;

//     case WizardFirstIdentifierLanguagePage:
//         return WizardChoiceLanguageOtherPage;
//     case WizardChoiceLanguageOtherPage:
//         if (field("choiceLanguageOther.language").toBool()) {
//             return WizardFirstLanguagePage;
//         } else if (field("choiceLanguageOther.other").toBool()) {
//             return WizardOtherPage;
//         } else {
//             return WizardFinalPage;
//         }
//     case WizardFirstLanguagePage:
//         return WizardSecondLanguagePage;
//     case WizardSecondLanguagePage:
//         return WizardFinalPage;
//     case WizardOtherPage:
//         return WizardFinalPage;
    case WizardFinalPage:
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
