/***************************************************************************

                   internal doc options dialog class

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 12:00:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef DocOptionsPage_included
#define DocOptionsPage_included

#include "ui_DocOptionsPageForm.h"

class DocOptionsPage : public QWidget, public Ui::DocOptionsPageForm
{
    Q_OBJECT
public:
    DocOptionsPage(bool sort, QWidget *parent = 0);

    bool getSorting() const
    {
        return m_allowSorting;
    }

protected slots:
    void docSortToggled(bool);

private:
    bool m_allowSorting;
};

#endif // DocOptionsPage_included
