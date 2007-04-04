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

/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardLanguagePage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardLanguagePage(QWidget *parent = 0);
    void setLanguages(const QStringList &languageStrings);
    void initializePage();
    int firstLanguageIndex();
    int secondLanguageIndex();

private:
    QLabel *labelFirst;
    QLabel *labelSecond;
    KComboBox *m_firstLanguageCombo;
    KComboBox *m_secondLanguageCombo;
    QStringList m_languageStrings;

private slots:
    /**
     * To update the second ComboBox when the first one changed.
     * The selected language from the first ComboBox is removed from the second.
     * @param newSelection is the current selection in the first ComboBox
     */
    void slotFirstLanguageComboChanged(int newSelection);
};

#endif
