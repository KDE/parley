/***************************************************************************

    $Id$

                   slightly modified QProgressBar

    -----------------------------------------------------------------------

    begin                : Sun Apr 22 18:55:34 2000

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MyProgress_H
#define MyProgress_H

#include <qframe.h>

class MyProgressBar : public QFrame
{
public:
 MyProgressBar(QWidget* parent = 0, const char* name = 0);
 void setData (int _all, int _current, bool x_from_y);

protected:
 void paintEvent (QPaintEvent *pev);

 int  all,
      current;
 bool from;
};

#endif // MyProgress_H
