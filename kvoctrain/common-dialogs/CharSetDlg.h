/***************************************************************************

    $Id$

                        substitute charset for document

    -----------------------------------------------------------------------

    begin                : Mon Jun 12 09:51:32 2000
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/12 19:13:50  arnold
    switched dialog files to qt-designer

    Revision 1.1  2001/10/05 15:37:45  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef CharSetDlg_included
#define CharSetDlg_included

#include "CharSetDlgForm.h"

#include <qglobal.h>

#if QT_VERSION < 300

#include <qstrlist.h>

#include <vector.h>

class CharSetDlg : public CharSetDlgForm
{
    Q_OBJECT

public:

    CharSetDlg
    (
        bool          flag,
        QString       exact_name,
        QStrList     &rawnames,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    QFont getFont () const;

protected slots:
    void slotItemSelected (int item);
    void slotSelected(int dummy);

private:

    int           sel;
    vector<QFont> fonts;

};
#endif

#endif // CharSetDlg_included
