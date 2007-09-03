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

#ifndef IDENTIFIERLANGUAGEPAGE_H
#define IDENTIFIERLANGUAGEPAGE_H

#include "ui_identifierlanguagepage.h"
#include "common-dialogs/kvtlanguageview.h"
#include <QWizardPage>

/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class IdentifierLanguagePage : public QWizardPage, public Ui::IdentifierLanguagePage
{
Q_OBJECT
public:
    IdentifierLanguagePage(QWizard *parent);
    ~IdentifierLanguagePage();
//     void initializePage();


// public:
//     IdentifierLanguagePage(bool isFirstLanguagePage, QWizard *parent);
//     void initializePage();
//
// private:
//     bool m_isFirstLanguagePage;
//     QLabel *m_beforeSelectionLabel;
//     QLabel *m_currentLabel;
//     QCheckBox *m_moreLanguagesCheckbox;
//
//     QLineEdit *m_languageLabel;
//     QLineEdit *m_languageShortLabel;
//
// private slots:
//     void slotLanguageChanged(const QString &language, const QString &languageShort);



};

#endif
