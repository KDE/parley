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

#include "firstidentifierlanguagepage.h"

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
FirstIdentifierLanguagePage::FirstIdentifierLanguagePage(QWizard * parent)
    : QWizardPage(parent)
{
    setTitle(i18n("Identifier and language selection"));
    setSubTitle(i18n("Please select the first column data:"));

    setupUi(this);
    registerField("firstIdentifierName*", identifierNameLineEdit);
    registerField("firstLocale", languageComboBox);

    connect(languageComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentLanguageChanged(const QString&)));

    QStringList codes = KGlobal::locale()->allLanguagesList();

    QStringList languageNames;
    foreach (QString code, codes){
        languageNames.append( KGlobal::locale()->languageCodeToName(code) );
    }
    languageNames.sort();

    languageComboBox->addItems(languageNames);
}

FirstIdentifierLanguagePage::~FirstIdentifierLanguagePage()
{
}

void FirstIdentifierLanguagePage::currentLanguageChanged(const QString & language)
{
    identifierNameLineEdit->setText(language);
}

void FirstIdentifierLanguagePage::initializePage()
{
    QString currentCode = KGlobal::locale()->language();
    languageComboBox->setCurrentIndex(
        languageComboBox->findText(
            KGlobal::locale()->languageCodeToName(currentCode) ) );
}


#include "firstidentifierlanguagepage.moc"
