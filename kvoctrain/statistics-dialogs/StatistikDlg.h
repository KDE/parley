/***************************************************************************

                        show document statistics

    -----------------------------------------------------------------------

    begin          : Sun Sep 19 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <kpagedialog.h>

class KEduVocDocument;
class KVTLanguages;
class GradeCols;

class StatistikDlg : public KPageDialog
{
  Q_OBJECT
public:
  StatistikDlg(KVTLanguages &langset, KEduVocDocument *doc, QWidget *parent = NULL, const char *name = NULL, bool modal = true);

};

#endif // StatistikDlg_included

