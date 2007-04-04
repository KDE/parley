//
// C++ Interface: kvtnewdocumentwizardlanguagepage
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWDOCUMENTWIZARDALLLANGUAGESPAGE_H
#define KVTNEWDOCUMENTWIZARDALLLANGUAGESPAGE_H

#include "../common-dialogs/languageoptions.h"


/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardAllLanguagesPage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardAllLanguagesPage(KVTLanguages &languages, QWidget *parent = 0);

    bool validatePage();
    KVTLanguages getLanguages();
private:
    QLabel *label;
    LanguageOptions *m_languageOptions;
};

#endif
