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

#include "identifierlanguagepage.h"

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
IdentifierLanguagePage::IdentifierLanguagePage(QWizard * parent)
    : QWizardPage(parent)
{

    setTitle(i18n("Identifier and language selection"));

    setupUi(this);
    QStringList codes = KGlobal::locale()->allLanguagesList();

    int current = codes.indexOf(KGlobal::locale()->language());

    QStringList languageNames;
    foreach (QString code, codes){
        languageNames.append( KGlobal::locale()->languageCodeToName(code) );
    }

    languageComboBox->addItems(languageNames);
    languageComboBox->setCurrentIndex(current);

    connect(languageComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentLanguageChanged(const QString&)));

    registerField("firstIdentifierName*", identifierNameLineEdit);
    registerField("firstLocale", languageComboBox);
}

IdentifierLanguagePage::~IdentifierLanguagePage()
{
}

void IdentifierLanguagePage::currentLanguageChanged(const QString & language)
{
kDebug() << "currentLanguageChanged";
    if ( identifierNameLineEdit->text().isEmpty() ) {
        identifierNameLineEdit->setText(language);
    }
}


#include "identifierlanguagepage.moc"
