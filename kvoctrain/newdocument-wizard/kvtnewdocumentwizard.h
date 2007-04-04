//
// C++ Interface: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWDOCUMENTWIZARD_H
#define KVTNEWDOCUMENTWIZARD_H


#include "kvtnewdocumentwizardintropage.h"
#include "kvtnewdocumentwizardlanguagepage.h"
#include "kvtnewdocumentwizardalllanguagespage.h"
#include "kvtnewdocumentwizardfinalpage.h"

/**
A wizard to assist in the creation of kvtml documents with KVocTrain. Helps to set up the languages of the document.

	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizard : public QWizard
{
    /// The pages of this wizard
    enum NewDocumentWizardPages{
        WizardIntroPage,
        WizardLanguagesPage,
        WizardLanguagePage,
        WizardFinalPage
    };
    Q_OBJECT

public:
    KVTNewDocumentWizard(const KVTLanguages &languages, QWidget *parent = 0);

    void initializePage(int id);
    void accept();
    /**
     * When the wizard is finished, get the new languages
     * @return the chosen KVTLanguages languages */
    KVTLanguages getLanguages();
    /** get users choice for the first language
     * @return index
     */
    int getFirstLanguageIndex();
    /** get users choice for the second language
     * @return index
     */
    int getSecondLanguageIndex();
private:
    KVTLanguages m_languages;
    /// @todo rename allLanguages and language to something more distinct
    KVTNewDocumentWizardAllLanguagesPage *m_allLanguagesPage;
    KVTNewDocumentWizardLanguagePage *m_languagePage;
signals:
    void setLanguage(int column, int languageIndex);
};

#endif
