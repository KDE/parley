/***************************************************************************

                         statistics dialog page

    -----------------------------------------------------------------------

    begin          : Thu Sep 21 20:50:53 MET 1999

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

#ifndef StatistikPage_included
#define StatistikPage_included

#include <Q3ListView>
#include <QPixmap>

#include "StatistikPageForm.h"
#include <keduvocexpression.h>

class KEduVocDocument;
class GradeCols;

class StatistikPage : public QWidget, public Ui::StatistikPageForm
{
  Q_OBJECT

public:
  StatistikPage(int col, KEduVocDocument *doc, QWidget *parent = 0);

public slots:
  void slotPopupMenu(int row, int col);
  void slotRMB(Q3ListViewItem* Item, const QPoint & point, int);

protected:
  void setupPixmaps();

  struct stat_counter
  {
    stat_counter() {
      for (int i = 0; i <= KV_MAX_GRADE; i++)
        grade[i] = 0;
      num = 0;
    }

    int grade [KV_MAX_GRADE+1];
    int num;
  };

  int calc_width (struct StatistikPage::stat_counter *gc, int grade, int max_width);

  vector<QPixmap>  from_pix, to_pix;
  KEduVocDocument    *doc;

  vector<stat_counter>  fsc;
  vector<stat_counter>  tsc;
  //GradeCols            *gcol;
};

#endif // StatistikPage_included
