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
#ifndef KVTNEWDOCUMENTWIZARDLANGUAGEPAGE_H
#define KVTNEWDOCUMENTWIZARDLANGUAGEPAGE_H

#include <QWizardPage>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>


/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardLanguagePage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardLanguagePage(bool isFirstLanguagePage, QWizard *parent = 0);
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
