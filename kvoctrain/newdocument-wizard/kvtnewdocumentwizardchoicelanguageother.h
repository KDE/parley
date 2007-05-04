/***************************************************************************

                             kvtnewdocumentwizardchoicelanguageother

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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
    KVTNewDocumentWizardChoiceLanguageOther(bool showFileOpen, QWidget *parent);

private:
    QLabel *m_label;
    QRadioButton *m_languageRadioButton;
    QRadioButton *m_otherRadioButton;
    QRadioButton *m_fileOpenRadioButton;
};

#endif
