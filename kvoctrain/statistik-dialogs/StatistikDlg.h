/***************************************************************************

                        show document statistics

    -----------------------------------------------------------------------

    begin          : Sun Sep 19 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#ifndef StatistikDlg_included
#define StatistikDlg_included

#include <vector>

#include <kdialogbase.h>

using namespace std;

class kvoctrainDoc;
class LangSet;
class GradeCols;

class StatistikDlg : public KDialogBase
{
  Q_OBJECT
public:
  StatistikDlg(LangSet &langset, kvoctrainDoc *doc, QWidget *parent = NULL, const char *name = NULL, bool modal = true);

};

#endif // StatistikDlg_included

