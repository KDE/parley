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

#include <KDebug>
#include <KLineEdit>

#include <QTextEdit>
#include <QLabel>

#include <keduvocdocument.h>

TitlePage::TitlePage(KEduVocDocument * doc, bool languageSetup, QWidget* parent) : QWidget(parent)
{
    m_doc = doc;
    setupUi(this);

    m_showLanguages = languageSetup;

    titleLineEdit->setText(doc->title());
    authorLineEdit->setText(doc->author());
    contactLineEdit->setText(doc->authorContact());
    licenseComboBox->setEditText(doc->license());
    commentTextEdit->setText(doc->documentComment());
    categoryComboBox->setEditText(doc->category());

    if (languageSetup) {
        QStringList codes = KGlobal::locale()->allLanguagesList();

        QStringList languageNames;
        foreach (const QString &code, codes){
            languageNames.append( KGlobal::locale()->languageCodeToName(code) );
        }
        languageNames.sort();

        firstLanguageComboBox->addItems(languageNames);
        firstLanguageComboBox->completionObject()->insertItems(languageNames);
        secondLanguageComboBox->addItems(languageNames);
        secondLanguageComboBox->completionObject()->insertItems(languageNames);

        languageGroupBox->setVisible(true);

connect(fetchLanguageButton, SIGNAL(clicked()), SLOT(fetchGrammar()));

    } else {
        languageGroupBox->setVisible(false);
    }
}

void TitlePage::accept()
{
    m_doc->setTitle(titleLineEdit->text());
    m_doc->setAuthor(authorLineEdit->text());
    m_doc->setAuthorContact(contactLineEdit->text());
    m_doc->setLicense(licenseComboBox->currentText());
    m_doc->setDocumentComment(commentTextEdit->toPlainText());
    m_doc->setCategory(categoryComboBox->currentText());

    if (m_showLanguages) {
        QString firstLanguage = firstLanguageComboBox->currentText();
        QString firstLocale;
        QString secondLanguage = secondLanguageComboBox->currentText();
        QString secondLocale;
        
        // ugly but works for now: iterate over languages to check which code we have
        foreach ( const QString &code, KGlobal::locale()->allLanguagesList() ) {
            if ( firstLanguage == KGlobal::locale()->languageCodeToName(code) ) {
                firstLocale = code;
            }
            if ( secondLanguage == KGlobal::locale()->languageCodeToName(code) ) {
                secondLocale = code;
            }
        }

        m_doc->identifier(0).setLocale(firstLocale);
        m_doc->identifier(0).setName(firstLanguage);
        m_doc->identifier(1).setLocale(secondLocale);
        m_doc->identifier(1).setName(secondLanguage);
    }
}

#include <KMessageBox>

void TitlePage::fetchGrammar() {
    QString firstLanguage = firstLanguageComboBox->currentText();
    QString firstLocale;
    QString secondLanguage = secondLanguageComboBox->currentText();
    QString secondLocale;
    foreach ( const QString &code, KGlobal::locale()->allLanguagesList() ) {
        if ( firstLanguage == KGlobal::locale()->languageCodeToName(code) ) {
            firstLocale = code;
        }
        if ( secondLanguage == KGlobal::locale()->languageCodeToName(code) ) {
            secondLocale = code;
        }
    }

    KUrl location(QString("http://edu.kde.org/parley/locale/") + firstLocale + QString(".kvtml"));

    KEduVocDocument firstLanguageDoc;
    if (firstLanguageDoc.open(location) == KEduVocDocument::NoError) {


        m_doc->identifier(0).setArticle(firstLanguageDoc.identifier(0).article());
        m_doc->identifier(0).setPersonalPronouns(firstLanguageDoc.identifier(0).personalPronouns());
// @todo        m_doc->identifier(0).setDeclension(firstLanguageDoc.identifier(0).declension());
        m_doc->identifier(0).setTenseList(firstLanguageDoc.identifier(0).tenseList());

        KMessageBox::information(this, "Success");
    } else {
        KMessageBox::information(this, "Failure");
    
    }

}

#include "TitlePage.moc"
