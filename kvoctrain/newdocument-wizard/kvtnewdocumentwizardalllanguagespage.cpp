/***************************************************************************

                             kvtnewdocumentwizardalllanguagepage

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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

#include "kvtnewdocumentwizardalllanguagespage.h"

#include <QWizardPage>
#include <QLabel>

#include <KMessageBox>

#include <kvtlanguages.h>

KVTNewDocumentWizardAllLanguagesPage::KVTNewDocumentWizardAllLanguagesPage(KVTLanguages &languages, QWidget *parent)
 : QWizardPage(parent)
{
    /// @todo so far adding a language has no effect.
    setTitle(i18n("Language"));
    setSubTitle(i18n("Please make sure that all languages you want to use are defined here."));
    label = new QLabel(i18n("Check the two letter list, if all languages you want to use are there. "));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    
    m_languageOptions = new LanguageOptions(languages, 0);
    layout->addWidget(m_languageOptions);
    
//    connect(m_languageOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

    registerField("languagesComboBox", m_languageOptions->d_shortName);

    setLayout(layout);
}

bool KVTNewDocumentWizardAllLanguagesPage::validatePage()
{
//    kDebug() << "Validate page." << endl;
//    m_languageOptions->updateSettings();
    if(m_languageOptions->d_shortName->count() >= 2)
        return true;
    KMessageBox::error(this, i18n("Please set at least two languages up."), i18n("Error:"));
    return false;
}

KVTLanguages KVTNewDocumentWizardAllLanguagesPage::getLanguages()
{
    return m_languageOptions->getLangSet();
}

#include "kvtnewdocumentwizardalllanguagespage.moc"
