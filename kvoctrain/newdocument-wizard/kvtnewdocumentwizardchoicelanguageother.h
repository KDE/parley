//
// C++ Interface: kvtnewdocumentwizardchoicelanguageother
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWDOCUMENTWIZARDCHOICELANGUAGEOTHER_H
#define KVTNEWDOCUMENTWIZARDCHOICELANGUAGEOTHER_H

#include <QRadioButton>
#include <QLabel>
#include <QWizardPage>

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/

class KVTNewDocumentWizardChoiceLanguageOther : public QWizardPage
{
Q_OBJECT
public:
    KVTNewDocumentWizardChoiceLanguageOther(bool showFileOpen, QWidget *parent = 0);

private:
    QLabel *m_label;
    QRadioButton *m_languageRadioButton;
    QRadioButton *m_otherRadioButton;
    QRadioButton *m_fileOpenRadioButton;
};

#endif
