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

#ifndef WIZARDFINALPAGE_H
#define WIZARDFINALPAGE_H

#include "ui_wizardfinalpage.h"

#include <QWizardPage>

/**
 * QWizardPage giving additional information. Last page.
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class WizardFinalPage : public QWizardPage, public Ui::WizardFinalPage
{
Q_OBJECT
public:
    WizardFinalPage(QWizard *parent);
};

#endif
