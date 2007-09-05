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
#include <keduvocmultiplechoice.h>
#include <keduvocdocument.h>

class MCEntryPage : public QWidget, public Ui::MCEntryPageForm
{
    Q_OBJECT

public:
    explicit MCEntryPage(KEduVocDocument *doc, QWidget *parent = 0);

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

#endif // MCEntryPage_included
