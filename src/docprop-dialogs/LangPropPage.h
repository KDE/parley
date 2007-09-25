/***************************************************************************

                   language properties dialog page

    -----------------------------------------------------------------------

    begin         : Wed Oct 13 18:37:13 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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


#ifndef LangPropPage_included
#define LangPropPage_included

#include "ui_LangPropPageForm.h"

#include <keduvocgrammar.h>
#include <keduvocconjugation.h>

class KEduVocDocument;

class LangPropPage : public QWidget, public Ui::LangPropPageForm
{
    Q_OBJECT
public:
    LangPropPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent = 0);
    void accept();

private slots:
    void updateCheckBoxes();
private:
    KEduVocDocument    *m_doc;
    int m_identifierIndex;
};

#endif // LangPropPage_included
