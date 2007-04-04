//
// C++ Implementation: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <KLocale>
#include <KComboBox>
#include <kvtlanguages.h>



#include "kvtnewdocumentwizard.h"

KVTNewDocumentWizard::KVTNewDocumentWizard(const KVTLanguages &languages, QWidget *parent)
 : QWizard(parent)
{
    m_languages=languages;
    m_allLanguagesPage = new KVTNewDocumentWizardAllLanguagesPage(m_languages);
    m_languagePage = new KVTNewDocumentWizardLanguagePage();
    
    setPage(WizardIntroPage, new KVTNewDocumentWizardIntroPage);
    setPage(WizardLanguagesPage, m_allLanguagesPage);
    setPage(WizardLanguagePage, m_languagePage);
    setPage(WizardFinalPage, new KVTNewDocumentWizardFinalPage);

    //setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(i18n("Create a new KVocTrain document"));
}


void KVTNewDocumentWizard::initializePage(int id)
{
    if (id == WizardLanguagePage){
        m_languages = m_allLanguagesPage->getLanguages();
        kDebug() << "About to initialize LanguagePage!" << endl;


        QStringList languages;
        for (int i = 0; i < m_languages.count(); i++){
            if(!m_languages.longId(i).isEmpty()){
                languages.append(m_languages.longId(i));
            }
            else {
                languages.append(m_languages.shortId(i));
            }
        }
        m_languagePage->setLanguages(languages);
    }
    QWizard::initializePage(id);

}


void KVTNewDocumentWizard::accept()
{
    //m_languages = m_allLanguagesPage->getLanguages();
    /*
    int originalLanguageIndex = m_languagePage->firstLanguageIndex();
    int translationLanguageIndex = m_languagePage->secondLanguageIndex();
    
    kDebug() << "Setting languages to: o: " << originalLanguageIndex << " t: " << translationLanguageIndex << endl;*/
    QDialog::accept();
    /*
    emit setLanguage(0, originalLanguageIndex);
    emit setLanguage(1 , translationLanguageIndex);
    */
}


int KVTNewDocumentWizard::getFirstLanguageIndex()
{
    return m_languagePage->firstLanguageIndex();
}

int KVTNewDocumentWizard::getSecondLanguageIndex()
{
    return m_languagePage->secondLanguageIndex();
}

KVTLanguages KVTNewDocumentWizard::getLanguages()
{
    return m_languages;
}

#include "kvtnewdocumentwizard.moc"
