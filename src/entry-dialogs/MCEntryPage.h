/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef MCEntryPage_included
#define MCEntryPage_included

#include "ui_MCEntryPageForm.h"

class KEduVocExpression;

class MCEntryPage : public QWidget, public Ui::MCEntryPageForm
{
    Q_OBJECT

public:
    explicit MCEntryPage(QWidget *parent = 0);

    void clear();

public slots:
    void setTranslation(KEduVocExpression* entry,int translation);

private slots:
    void slotDataChanged(const QString&);

private:
//     KEduVocTranslation* m_translation;
};

#endif // MCEntryPage_included
