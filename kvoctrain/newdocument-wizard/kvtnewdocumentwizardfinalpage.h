/***************************************************************************

                             kvtnewdocumentwizardfinalpage

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

#ifndef KVTNEWDOCUMENTWIZARDFINALPAGE_H
#define KVTNEWDOCUMENTWIZARDFINALPAGE_H

#include <QWizardPage>
#include <QLabel>

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardFinalPage : public QWizardPage
{
    Q_OBJECT

public:
    KVTNewDocumentWizardFinalPage(QWidget *parent = 0);

private:
    QLabel *label;
};

#endif
