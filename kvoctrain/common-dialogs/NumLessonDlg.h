/***************************************************************************

    $Id$

                    input number of entries per lesson

    -----------------------------------------------------------------------

    begin                : Sat May 27 19:37:59 2000
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NumLessonDlg_included
#define NumLessonDlg_included

#include "NumLessonDlgData.h"

class NumLessonDlg : public NumLessonDlgData
{
    Q_OBJECT

public:

    NumLessonDlg
    (
        int         num,
        QWidget    *parent = NULL,
        const char *name = NULL
    );

    int getInput() { return input; }

protected slots:

    void slotInputChanged(const QString&);

private:
    int  input;
//  virtual ~NumLessonDlg();

};
#endif // NumLessonDlg_included
