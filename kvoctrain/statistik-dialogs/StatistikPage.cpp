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
    Revision 1.3  2001/10/19 17:52:18  arnold
    replaced dialog files from qtarch by qtdesigner

    Revision 1.2  2001/10/13 11:45:30  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

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

#include "StatistikPage.h"

#include <kpopupmenu.h>
#include <kapp.h>
#include <klocale.h>

#include <kvoctraindoc.h>

#include <qcursor.h>
#include <qlayout.h>
#include <qpainter.h>

#define MIN_COL_WIDTH      2
#define PIX_SHIFT          2
#define SIZE_GRADE         100+PIX_SHIFT
#define SIZE_COUNT         70
#define SIZE_LESSON        300

#define TB_FGRADE          0
#define TB_TGRADE          1
#define TB_COUNT           2
#define TB_LESSON          3


class GradeListItem : public QListViewItem
{
 public:
   inline GradeListItem (QListView* parent, int _lesson)
      : QListViewItem(parent), lesson(_lesson) {}

   inline int getLesson() const { return lesson; }

 private:
   int lesson;
};


StatistikPage::StatistikPage
(
        int            col,
        kvoctrainDoc  *_doc,
        GradeCols     *_gcol,
	QWidget       *parent,
	const char    *name
)
	:
	StatistikPageForm( parent, name ),
        doc(_doc),
        gcol(_gcol)
{

   lessonbox = new QListView( this, "QListView_1" );

   layout = new QGridLayout( this );
   layout->setSpacing( 6 );
   layout->setMargin( 11 );
   layout->addWidget( lessonbox, 0, 0 );

   lessonbox->addColumn (i18n("Grade FROM"), SIZE_GRADE+10);
   lessonbox->addColumn (i18n("Grade TO"), SIZE_GRADE+10);
   lessonbox->addColumn (i18n("Entries"), SIZE_COUNT);
   lessonbox->addColumn (i18n("Lesson"), SIZE_LESSON);

   vector<QString> lesson = doc->getLessonDescr();

   fsc.resize(lesson.size()+1);
   tsc.resize(lesson.size()+1);

   // accumulate numbers of grades per lesson
   for (int i = 0; i < (int) doc->numEntries(); i++) {
     kvoctrainExpr *expr = doc->getEntry(i);
     int fg = QMIN(KV_MAX_GRADE, expr->getGrade(col, false));
     int tg = QMIN(KV_MAX_GRADE, expr->getGrade(col, true));
     int l = expr->getLesson();
     if (l >= 0 && l <= (int) lesson.size() ) {
       fsc[l].grade[fg]++;
       fsc[l].num++;
       tsc[l].grade[tg]++;
       tsc[l].num++;
     }
   }
   setupPixmaps();
   connect(lessonbox, SIGNAL( rightButtonPressed( QListViewItem *, const QPoint& , int ) ),
	    this, SLOT( slotRMB( QListViewItem *, const QPoint &, int ) ) );
   lessonbox->setSelectionMode( QListView::NoSelection );
}


void StatistikPage::setupPixmaps()
{
   // create pixmaps with bar charts of numbers of grades
   int height;
   GradeListItem lvi (lessonbox, 0);
   height = lvi.height();
   for (int entry = 0; entry < (int) fsc.size(); entry++) {
     QPainter p;
     QColor color;
     QPixmap fpix (SIZE_GRADE, height);
     p.begin( &fpix);
     p.eraseRect (0, 0, fpix.width(), fpix.height());
     p.setPen( black );
     if (fsc[entry].num != 0) {

       int num = 0;
       for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++)
         if (fsc[entry].grade[j] != 0)
           num++;

       int maxw = fpix.width() -PIX_SHIFT -PIX_SHIFT -1;
       int w = maxw;
       int widths [KV_MAX_GRADE+1];
       for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++) {
         if (fsc[entry].grade[j] == 0)
            widths[j] = 0;
         else {
           if (num <= 1) {
              widths[j] = w;
              w = 0;
           }
           else {
             --num;
             widths[j] = QMAX(MIN_COL_WIDTH, fsc[entry].grade[j] * maxw / fsc[entry].num);
             w -= widths[j];
           }
         }
       }

       int x = 0;
       int x2 = 1+PIX_SHIFT;
       for (int j = KV_MIN_GRADE; j <= KV_MAX_GRADE; j++) {
         switch (j) {
           case KV_NORM_GRADE: color = gcol->col0;    break;
           case KV_LEV1_GRADE: color = gcol->col1;    break;
           case KV_LEV2_GRADE: color = gcol->col2;    break;
           case KV_LEV3_GRADE: color = gcol->col3;    break;
           case KV_LEV4_GRADE: color = gcol->col4;    break;
           case KV_LEV5_GRADE: color = gcol->col5;    break;
           case KV_LEV6_GRADE: color = gcol->col6;    break;
           case KV_LEV7_GRADE: color = gcol->col7;    break;
           default           : color = gcol->col1;
         }
         if (widths[j] != 0) {
           x2 += widths[j];
           p.fillRect(x+PIX_SHIFT, 1, x2-x, height-1, color);
           p.drawRect(x+PIX_SHIFT, 1, x2-x, height-1);
           x = x2-1;
         }
       }
     }
     else {
       p.fillRect(PIX_SHIFT, 1, fpix.width()-PIX_SHIFT, height-1, gcol->col0);
       p.drawRect(PIX_SHIFT, 1, fpix.width()-PIX_SHIFT, height-1);
     }
     p.end();
     from_pix.push_back(fpix);

     QPixmap tpix (SIZE_GRADE, height);
     p.begin( &tpix );
     p.eraseRect (0, 0, tpix.width(), tpix.height());
     p.setPen( black );
     if (tsc[entry].num != 0) {
       int num = 0;
       for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++)
         if (tsc[entry].grade[j] != 0)
           num++;

       int maxw = tpix.width() -PIX_SHIFT -PIX_SHIFT -1;
       int w = maxw;
       int widths [KV_MAX_GRADE+1];
       for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++) {
         if (tsc[entry].grade[j] == 0)
            widths[j] = 0;
         else {
           if (num <= 1) {
              widths[j] = w;
              w = 0;
           }
           else {
             --num;
             widths[j] = QMAX(MIN_COL_WIDTH, tsc[entry].grade[j] * maxw / tsc[entry].num);
             w -= widths[j];
           }
         }
       }

       int x = 0;
       int x2 = 1+PIX_SHIFT;
       for (int j = KV_MIN_GRADE; j <= KV_MAX_GRADE; j++) {
         switch (j) {
           case KV_NORM_GRADE: color = gcol->col0;    break;
           case KV_LEV1_GRADE: color = gcol->col1;    break;
           case KV_LEV2_GRADE: color = gcol->col2;    break;
           case KV_LEV3_GRADE: color = gcol->col3;    break;
           case KV_LEV4_GRADE: color = gcol->col4;    break;
           case KV_LEV5_GRADE: color = gcol->col5;    break;
           case KV_LEV6_GRADE: color = gcol->col6;    break;
           case KV_LEV7_GRADE: color = gcol->col7;    break;
           default           : color = gcol->col1;
         }
         if (widths[j] != 0) {
           x2 += widths[j];
           p.fillRect(x+PIX_SHIFT, 1, x2-x, height-1, color);
           p.drawRect(x+PIX_SHIFT, 1, x2-x, height-1);
           x = x2-1;
         }
       }
     }
     else {
       p.fillRect(PIX_SHIFT, 1, tpix.width()-PIX_SHIFT, height-1, gcol->col0);
       p.drawRect(PIX_SHIFT, 1, tpix.width()-PIX_SHIFT, height-1);
     }
     p.end();
     to_pix.push_back(tpix);
   }

   // setup rows with pixmaps and strings
   vector<QString> lesson = doc->getLessonDescr();

   QString s;

   GradeListItem *plvi = new GradeListItem(lessonbox, 0);
   plvi->setPixmap (TB_FGRADE, from_pix[0]);
   plvi->setPixmap (TB_TGRADE, to_pix[0]);
   s.setNum (tsc[0].num);
   plvi->setText (TB_COUNT, s);
   plvi->setText (TB_LESSON, i18n("<no lesson>"));
   lessonbox->insertItem (plvi);

   for (int i = 0; i < (int) lesson.size(); i++) {
     plvi = new GradeListItem(lessonbox, i+1);
     plvi->setPixmap (TB_FGRADE, from_pix[i+1]);
     plvi->setPixmap (TB_TGRADE, to_pix[i+1]);
     s.setNum (tsc[i+1].num);
     plvi->setText (TB_COUNT, s);
     plvi->setText (TB_LESSON, lesson[i]);
     lessonbox->insertItem (plvi);
   }
}


void StatistikPage::slotRMB( QListViewItem* Item, const QPoint & point, int col)
{
    if( Item != 0)
      slotPopupMenu(((GradeListItem*)Item)->getLesson(), col);
}


void StatistikPage::slotPopupMenu(int row, int col)
{
  struct stat_counter *sc;

  if (col == TB_FGRADE) {
    if (row >= (int) fsc.size() ) {
      cerr << "row >= fsc.size()" << endl;
      return;
    }
    else
      sc = &fsc[row];
  }
  else if (col == TB_TGRADE) {
    if (row >= (int) tsc.size() ) {
      cerr << "row >= tsc.size()" << endl;
      return;
    }
    else
      sc = &tsc[row];
  }
  else
    return;

  KPopupMenu *header_m = new KPopupMenu(i18n("Number of entries per grade"));
  QString s;

  s.setNum (sc->grade[KV_NORM_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_NORM_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV1_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV1_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV2_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV2_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV3_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV3_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV4_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV4_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV5_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV5_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV6_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV6_TEXT);
  header_m->insertItem (s);

  s.setNum (sc->grade[KV_LEV7_GRADE]);
  s.insert(0, "\t");
  s.insert(0, KV_LEV7_TEXT);
  header_m->insertItem (s);

  header_m->exec(QCursor::pos()+QPoint(10, 0));
}


StatistikPage::~StatistikPage()
{
}
#include "StatistikPage.moc"
