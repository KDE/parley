/***************************************************************************

    $Id$

                         statistics dialog page

    -----------------------------------------------------------------------

    begin                : Thu Sep 21 20:50:53 MET 1999
                                           
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


#ifndef Statistik_included
#define Statistik_included

#include <vector.h>

#include "StatistikPageForm.h"

#include <kvoctrainexpr.h>

#include <qlistview.h>

class kvoctrainDoc;
class GradeCols;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class StatistikPage : public StatistikPageForm
{
    Q_OBJECT

public:

    StatistikPage
    (
        int           col,
        kvoctrainDoc *doc,
        GradeCols    *gc,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    ~StatistikPage();

 public slots:
    void slotPopupMenu(int row, int col);
    void slotRMB( QListViewItem* Item, const QPoint & point, int );

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
   
   int calc_width (struct StatistikPage::stat_counter *gc,
                   int grade, int max_width);

   vector<QPixmap>  from_pix, to_pix;
   kvoctrainDoc    *doc;

   vector<stat_counter>  fsc;
   vector<stat_counter>  tsc;
   GradeCols            *gcol;
   QListView            *lessonbox;
   QGridLayout          *layout;

};
#endif // StatistikPage_included
