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

#ifndef KVTNEWDOCUMENTWIZARDLANGUAGEPAGE_H
#define KVTNEWDOCUMENTWIZARDLANGUAGEPAGE_H

#include <QWizardPage>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>


/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KVTNewDocumentWizardLanguagePage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardLanguagePage(bool isFirstLanguagePage, QWizard *parent);
    void initializePage();

private:
    bool m_isFirstLanguagePage;
    QLabel *m_beforeSelectionLabel;
    QLabel *m_currentLabel;
    QCheckBox *m_moreLanguagesCheckbox;

    QLineEdit *m_languageLabel;
    QLineEdit *m_languageShortLabel;

private slots:
    void slotLanguageChanged(const QString &language, const QString &languageShort);
};

#endif
