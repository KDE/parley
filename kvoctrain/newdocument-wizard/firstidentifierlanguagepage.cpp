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
    QStringList codes = KGlobal::locale()->allLanguagesList();

    int current = codes.indexOf(KGlobal::locale()->language());

    QStringList languageNames;
    foreach (QString code, codes){
        languageNames.append( KGlobal::locale()->languageCodeToName(code) );
    }

    languageComboBox->addItems(languageNames);
    languageComboBox->setCurrentIndex(current);

    identifierNameLineEdit->setText( languageComboBox->currentText() );

    connect(languageComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentLanguageChanged(const QString&)));

    registerField("firstIdentifierName*", identifierNameLineEdit);
    registerField("firstLocale", languageComboBox);
}

FirstIdentifierLanguagePage::~FirstIdentifierLanguagePage()
{
}

void FirstIdentifierLanguagePage::currentLanguageChanged(const QString & language)
{
    //if ( identifierNameLineEdit->text().isEmpty() ) {
    identifierNameLineEdit->setText(language);
    //}
}


#include "firstidentifierlanguagepage.moc"
