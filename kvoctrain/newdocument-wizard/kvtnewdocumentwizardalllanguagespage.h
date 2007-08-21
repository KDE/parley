/***************************************************************************

                             kvtnewdocumentwizardalllanguagepage

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

#ifndef KVTNEWDOCUMENTWIZARDALLLANGUAGESPAGE_H
#define KVTNEWDOCUMENTWIZARDALLLANGUAGESPAGE_H

#include "../common-dialogs/languageoptions.h"


/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KVTNewDocumentWizardAllLanguagesPage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardAllLanguagesPage(KVTLanguages &languages, QWidget *parent);

    bool validatePage();
    KVTLanguages getLanguages();
private:
    QLabel *label;
    LanguageOptions *m_languageOptions;
};

#endif
