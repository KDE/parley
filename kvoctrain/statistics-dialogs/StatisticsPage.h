/***************************************************************************

                         statistics dialog page

    -----------------------------------------------------------------------

    begin         : Thu Sep 21 20:50:53 MET 1999

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

#ifndef StatisticsPage_included
#define StatisticsPage_included

#include <QTreeWidget>
#include <QPixmap>

#include "ui_StatisticsPageForm.h"
#include <keduvocexpression.h>

class KEduVocDocument;
class GradeCols;

class StatisticsPage : public QWidget, public Ui::StatisticsPageForm
{
  Q_OBJECT

public:
  StatisticsPage(int col, KEduVocDocument *doc, QWidget *parent = 0);

public slots:
  void slotPopupMenu(int row, int col);
  void slotRMB(QTreeWidgetItem * Item, const QPoint & point, int);

protected:
  void setupPixmaps();

  struct stat_counter
  {
    stat_counter() {
      for (int i = 0; i <= KV_MAX_GRADE; i++)
        grade[i] = 0;
      num = 0;
    }

    int grade [KV_MAX_GRADE + 1];
    int num;
  };

  int calc_width (struct StatisticsPage::stat_counter *gc, int grade, int max_width);

  QList<QPixmap> from_pix;
  QList<QPixmap> to_pix;
  KEduVocDocument *doc;

  QVector<stat_counter> fsc;
  QVector<stat_counter> tsc;
  //GradeCols            *gcol;
};

#endif // StatisticsPage_included
