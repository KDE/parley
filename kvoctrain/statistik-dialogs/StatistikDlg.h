/***************************************************************************

    $Id$

                        show document statistics

    -----------------------------------------------------------------------

    begin                : Sun Sep 19 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:47:12  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef StatistikDlg_included
#define StatistikDlg_included

#include <qtabdialog.h>

#include <vector>
using namespace std;

class kvoctrainDoc;
class LangSet;
class GradeCols;

class StatistikDlg : public QTabDialog
{
    Q_OBJECT

public:

    StatistikDlg
    (
        LangSet         &langset,
        kvoctrainDoc    *doc,
        GradeCols       *gc,
        QWidget         *parent = NULL,
        const char      *name = NULL
    );

protected slots:

    void okButton();

protected:

};

#endif // StatistikDlg_included

