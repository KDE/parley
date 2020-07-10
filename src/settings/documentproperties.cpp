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

#include "parleymainwindow.h"
#include "parleydocument.h"

#include <KEduVocDocument>
#include <QLineEdit>

#include <QTextEdit>
#include <QLabel>

DocumentProperties::DocumentProperties(KEduVocDocument * doc, bool languageSetup, QWidget* parent)
    : QWidget(parent), m_doc(doc), m_showLanguages(languageSetup)
{
    setupUi(this);

    titleLineEdit->setText(doc->title());
    authorLineEdit->setText(doc->author());
    contactLineEdit->setText(doc->authorContact());
    licenseComboBox->setEditText(doc->license());
    commentTextEdit->setText(doc->documentComment());
    categoryComboBox->setEditText(doc->category());

    if (languageSetup) {
        prepareLanguageSelection();
    } else {
        languageGroupBox->setVisible(false);
    }
}

void DocumentProperties::prepareLanguageSelection()
{
    QStringList languageNames;

    //Create a key map, key: language-country pair, value: locale
    QMap<QString, QString> languageCodeMap = DocumentProperties::localeLangsMap();

    QMap<QString, QString>::const_iterator lcMapIter;

    for (lcMapIter = languageCodeMap.constBegin(); lcMapIter != languageCodeMap.constEnd(); ++lcMapIter) {
        firstLanguageComboBox->addItem(lcMapIter.key(), lcMapIter.value());
        secondLanguageComboBox->addItem(lcMapIter.key(), lcMapIter.value());
        languageNames.append(lcMapIter.key());
    }

    languageNames.sort();

    firstLanguageComboBox->completionObject()->insertItems(languageNames);
    firstLanguageComboBox->completionObject()->setIgnoreCase(true);
    secondLanguageComboBox->completionObject()->insertItems(languageNames);
    secondLanguageComboBox->completionObject()->setIgnoreCase(true);

    languageGroupBox->setVisible(true);
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
        acceptLanguageConfiguration();
    }
}

void DocumentProperties::acceptLanguageConfiguration()
{
    QString firstLanguage = firstLanguageComboBox->currentText();
    QString firstLocale = firstLanguageComboBox->currentData().toString();
    QString secondLanguage = secondLanguageComboBox->currentText();
    QString secondLocale = secondLanguageComboBox->currentData().toString();
    m_doc->identifier(0).setLocale(firstLocale);
    m_doc->identifier(0).setName(firstLanguage);
    m_doc->identifier(1).setLocale(secondLocale);
    m_doc->identifier(1).setName(secondLanguage);
}

QMap<QString, QString> DocumentProperties::localeLangsMap()
{
    //Get a list of locales
    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage,
        QLocale::AnyScript,
        QLocale::AnyCountry);

    QMap<QString, QString> languageCodeMap;

    foreach (const QLocale & myLocale, allLocales) {
        if(!myLocale.nativeLanguageName().isEmpty() && !myLocale.nativeCountryName().isEmpty()) {
            languageCodeMap[ myLocale.nativeLanguageName() +  " (" +  myLocale.nativeCountryName() + ')' ] = myLocale.name();
        }
    }
    return languageCodeMap;
}
