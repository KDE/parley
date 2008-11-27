/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TITLEPAGE_H
#define TITLEPAGE_H

#include "ui_TitlePageForm.h"

class KEduVocDocument;

class TitlePage : public QWidget, public Ui::TitlePageForm
{
    Q_OBJECT

public:
    TitlePage(KEduVocDocument * doc, bool languageSetup, QWidget *parent);

public slots:
    // this will apply the settings, but be aware, this is not a kdialog, only a qwidget - thus you need to connect to the accept!
    virtual void accept();

private:
    void fetchGrammar(KEduVocDocument* doc, int index);
    KEduVocDocument* m_doc;

    // when this is used to set up a new document, show languages is true and the user can input two languages.
    bool m_showLanguages;
};

#endif // TitlePage_included
