/***************************************************************************

                   types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#ifndef WORDTYPEOPTIONPAGE_H
#define WORDTYPEOPTIONPAGE_H

#include "ui_typeeditform.h"

#include "keduvocwordtype.h"

#include <QStandardItemModel>

class KEduVocDocument;

class WordTypeOptionPage : public QWidget, public Ui::WordTypeEditForm
{
    Q_OBJECT

public:
    WordTypeOptionPage(KEduVocDocument *doc, QWidget *parent);

private slots:
    void itemChanged( QStandardItem * item );
    void slotActivated( QModelIndex * modelIndex );

    void slotNewWordType();
    void slotNewSubWordType();
    void slotRename();
    void slotDelete();

private:
    KEduVocDocument    *m_doc;
    KEduVocWordType     m_wordTypes;
    QStandardItemModel *m_wordTypeModel;
};

#endif // WordTypeOptionPage_included
