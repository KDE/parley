/***************************************************************************

    $Id$

                            input several descriptions

    -----------------------------------------------------------------------

    begin                : Thu Dec 9 1999
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
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

#ifndef LessonInputDlg_included
#define LessonInputDlg_included

#include "LessonInputDlgForm.h"

#include <qlineedit.h>

class LessonInputDlg : public LessonInputDlgForm
{
    Q_OBJECT

public:

    LessonInputDlg
    (
        QString     title,
        QString     descr,
        QString     label,
        QWidget    *parent = NULL,
        const char *name = NULL
    );

    QString getInput() { return input; }

protected slots:

    void slotInputChanged(const QString&);

private:
    QString input;
};
#endif // LessonInputDlg_included
