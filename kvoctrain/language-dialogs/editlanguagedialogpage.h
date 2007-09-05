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

#ifndef EDITEDITLANGUAGEDIALOGPAGEPAGE_H
#define EDITEDITLANGUAGEDIALOGPAGEPAGE_H

#include "ui_languagepage.h"
#include <QWidget>


class KEduVocDocument;


/**
 *
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class EditLanguageDialogPage
    : public QWidget, public Ui::LanguagePage
{
Q_OBJECT
public:
    EditLanguageDialogPage(KEduVocDocument* doc, int identifierIndex, QWidget *parent);
    ~EditLanguageDialogPage();

public slots:
    void commitData();

signals:
    void iconSelected( const QString& );

private slots:
    void iconChanged(int iconIndex);

private:
    void initialize();

    KEduVocDocument* m_doc;
    int m_identifierIndex;
};

#endif
