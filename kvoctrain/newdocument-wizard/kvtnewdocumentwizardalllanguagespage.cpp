//
// C++ Implementation: kvtnewdocumentwizardlanguagepage
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

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
