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

#include "documentproperties.h"

#include <KDebug>
#include <KLineEdit>

#include <QTextEdit>
#include <QLabel>

#include <keduvocdocument.h>

DocumentProperties::DocumentProperties(KEduVocDocument * doc, bool languageSetup, QWidget* parent) : QWidget(parent)
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
    } else {
        languageGroupBox->setVisible(false);
    }
}

void DocumentProperties::accept()
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
	fetchGrammar(m_doc, 0);
	fetchGrammar(m_doc, 1);
    }
}

void DocumentProperties::fetchGrammar(KEduVocDocument* doc, int index)
{
    QString locale = doc->identifier(index).locale();
    
    KUrl location(QString("http://edu.kde.org/parley/locale/") + locale + QString(".kvtml"));

    KEduVocDocument grammarDoc;
    if (grammarDoc.open(location) == KEduVocDocument::NoError) {
        m_doc->identifier(index).setArticle(grammarDoc.identifier(0).article());
        m_doc->identifier(index).setPersonalPronouns(grammarDoc.identifier(0).personalPronouns());
// @todo        m_doc->identifier(index).setDeclension(grammarDoc.identifier(0).declension());
        m_doc->identifier(index).setTenseList(grammarDoc.identifier(0).tenseList());
    } else {
        kDebug() << "Download of " << location.url() << " failed.";
    }
}

#include "documentproperties.moc"

