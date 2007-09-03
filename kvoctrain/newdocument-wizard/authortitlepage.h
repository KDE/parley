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

#ifndef AUTHORTITLEPAGE_H
#define AUTHORTITLEPAGE_H

#include "ui_TitlePageForm.h"

#include <QWizardPage>

/**
 * QWizardPage to let the user select the first two languages that he wants in his document.
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class AuthorTitlePage : public QWizardPage, public Ui::TitlePageForm
{
Q_OBJECT
public:
    AuthorTitlePage(QWizard *parent);
    ~AuthorTitlePage();

};

#endif
