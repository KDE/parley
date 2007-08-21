/***************************************************************************

                             kvtnewdocumentwizardotherpage

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

#ifndef KVTNEWDOCUMENTWIZARDOTHERPAGE_H
#define KVTNEWDOCUMENTWIZARDOTHERPAGE_H

#include <QWizardPage>
#include <QLabel>

#include <KLineEdit>

/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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
