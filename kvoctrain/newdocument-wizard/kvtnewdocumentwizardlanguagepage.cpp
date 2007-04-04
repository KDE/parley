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


#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>

#include <KComboBox>

#include <kvtlanguages.h>
#include "../common-dialogs/languageoptions.h"

#include "kvtnewdocumentwizardlanguagepage.h"

KVTNewDocumentWizardLanguagePage::KVTNewDocumentWizardLanguagePage(QWidget *parent)
 : QWizardPage(parent)
{
    setTitle(i18n("Your language"));
    setSubTitle(i18n("Now select the languages that will be used."));
    
    labelFirst = new QLabel(i18n("First language: "));
    labelFirst->setWordWrap(true);
    labelFirst->setBuddy(m_firstLanguageCombo);
    m_firstLanguageCombo = new KComboBox();

    labelSecond = new QLabel(i18n("Second language: "));
    labelSecond->setWordWrap(true);
    labelSecond->setBuddy(m_secondLanguageCombo);
    m_secondLanguageCombo = new KComboBox();

    QLabel *labelHint = new QLabel(i18n("Please note that you can add more languages later. Use Vocabulary->AppendLanguage to do that."));
    labelHint->setWordWrap(true);
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(labelFirst);
    layout->addWidget(m_firstLanguageCombo);
    layout->addWidget(labelSecond);
    layout->addWidget(m_secondLanguageCombo);
    layout->addWidget(labelHint);
    
    setLayout(layout);

    connect(m_firstLanguageCombo, SIGNAL(activated(int)), this, SLOT(slotFirstLanguageComboChanged(int)));
}


void KVTNewDocumentWizardLanguagePage::setLanguages(const QStringList &languageStrings)
{
    m_languageStrings = languageStrings;
}

void KVTNewDocumentWizardLanguagePage::initializePage()
{
    // initialize the ComboBoxes
    m_firstLanguageCombo->clear();
    m_firstLanguageCombo->addItems(m_languageStrings);
    //m_firstLanguageCombo->select(0);
    slotFirstLanguageComboChanged(0);

}


void KVTNewDocumentWizardLanguagePage::slotFirstLanguageComboChanged(int newSelection)
{
    kDebug() << "slotFirstLanguageComboChanged" << endl;
    // update the second ComboBox removing the selection from the first
    m_secondLanguageCombo->clear();
    for (int i = 0; i<m_firstLanguageCombo->count(); i++){
        if (i != newSelection){
            /// @todo change this to the not deprecated functions
            m_secondLanguageCombo->addItem(m_languageStrings[i]);
        }
    }
}

int KVTNewDocumentWizardLanguagePage::firstLanguageIndex()
{
    return m_firstLanguageCombo->currentIndex();
}


int KVTNewDocumentWizardLanguagePage::secondLanguageIndex()
{
    int index = m_secondLanguageCombo->currentIndex();
    if ( index >= firstLanguageIndex() ){
        index++; // before the original was removed from the list.
    }
    return index;
}

#include "kvtnewdocumentwizardlanguagepage.moc"
