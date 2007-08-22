/***************************************************************************

                   dialog page for adjectives (comparison)

    -----------------------------------------------------------------------

    begin         : Sat Dec 4 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#ifndef AdjEntryPage_included
#define AdjEntryPage_included

#include "ui_AdjEntryPageForm.h"
#include <keduvocdocument.h>

class AdjEntryPage : public QWidget, public Ui::AdjEntryPageForm
{
    Q_OBJECT

public:
    AdjEntryPage(KEduVocDocument *doc, QWidget *parent = 0);

    void setData(int row, int col);
    void commitData();
    void clear();

    bool isModified();

signals:
    void sigModified();

private slots:
    void slotDataChanged(const QString&);

private:
    KEduVocDocument     *m_doc;
    int m_currentRow;
    int m_currentTranslation;
};

#endif // AdjEntryPage_included
