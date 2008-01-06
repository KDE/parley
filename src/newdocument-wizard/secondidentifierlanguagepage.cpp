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

#include "secondidentifierlanguagepage.h"

#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QCheckBox>
#include <QVBoxLayout>

#include <KComboBox>
#include <KLocale>


#include "kvtnewdocumentwizard.h"


/**
 *
 * @param parent
 */
SecondIdentifierLanguagePage::SecondIdentifierLanguagePage(QWizard * parent)
    : QWizardPage(parent)
{
    setTitle(i18n("Identifier and language selection"));
    setSubTitle(i18n("Please select the second column data:"));

    setupUi(this);
    registerField("secondIdentifierName*", identifierNameLineEdit);
    registerField("secondLocale", languageComboBox, "currentText", "currentTextChanged()");

    connect(languageComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentLanguageChanged(const QString&)));

    QStringList codes = KGlobal::locale()->allLanguagesList();

    QStringList languageNames;
    foreach (const QString &code, codes){
        languageNames.append( KGlobal::locale()->languageCodeToName(code) );
    }
    languageNames.sort();

    languageComboBox->addItems(languageNames);
}

SecondIdentifierLanguagePage::~SecondIdentifierLanguagePage()
{
}

void SecondIdentifierLanguagePage::currentLanguageChanged(const QString & language)
{
    identifierNameLineEdit->setText(language);
}

void SecondIdentifierLanguagePage::initializePage()
{
    firstLanguageName->setText( field("firstIdentifierName").toString() );

    // maybe stupid, but just assume everyone wants to learn english :)
    languageComboBox->setCurrentIndex(
        languageComboBox->findText(
            KGlobal::locale()->languageCodeToName("en") ) );
}


#include "secondidentifierlanguagepage.moc"
