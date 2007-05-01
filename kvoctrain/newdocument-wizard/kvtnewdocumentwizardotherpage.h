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
#ifndef KVTNEWDOCUMENTWIZARDOTHERPAGE_H
#define KVTNEWDOCUMENTWIZARDOTHERPAGE_H

#include <QWizardPage>
#include <QLabel>

#include <KLineEdit>

/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardOtherPage : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardOtherPage(QWidget *parent = 0);

private:
    QLabel *m_labelFirst;
    QLabel *m_labelSecond;
    KLineEdit *m_firstIdentifierLineEdit;
    KLineEdit *m_secondIdentifierLineEdit;
    //QStringList m_languageStrings;

private slots:
};

#endif
