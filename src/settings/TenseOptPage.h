/***************************************************************************

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin         : Sun May 28 12:14:31 2000

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

#ifndef TenseOptPage_included
#define TenseOptPage_included

#include "ui_optionlistform.h"

class KEduVocDocument;

class TenseOptPage : public QWidget, public Ui::OptionListForm
{
    Q_OBJECT

public:
    TenseOptPage(KEduVocDocument * doc, QWidget *parent);
    void accept();

private slots:
    void slotDeleteTense();
    void slotNewTense();
    void slotTenseChosen(int);
    void slotModifyTense();

private:
    void updateListBox(int start);

    KEduVocDocument  *m_doc;
    int               m_currentTense;
    QList<int>        tenseIndex; // contains indices of tenses on exec()
    // negative values are new tenses
};

#endif // TenseOptPage_included
