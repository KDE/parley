/***************************************************************************

                       statistics dialog page

    -----------------------------------------------------------------------

    begin          : Thu Sep 21 20:50:53 MET 1999

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

#include <QCursor>
#include <QPainter>
#include <QPixmap>

#include <kmenu.h>
#include <klocale.h>
#include <kdebug.h>

#include <kvoctraindoc.h>
#include <prefs.h>
#include "StatistikPage.h"


#define MIN_COL_WIDTH      2
#define PIX_SHIFT          2
#define SIZE_GRADE         100+PIX_SHIFT
#define SIZE_COUNT         70
#define SIZE_LESSON        300

#define TB_FGRADE          0
#define TB_TGRADE          1
#define TB_COUNT           2
#define TB_LESSON          3


class GradeListItem : public Q3ListViewItem
{
public:
  inline GradeListItem (Q3ListView* parent, int _lesson): Q3ListViewItem(parent), lesson(_lesson) {}
  inline int getLesson() const { return lesson; }

private:
  int lesson;
};


StatistikPage::StatistikPage(int col, kvoctrainDoc  *_doc, QWidget *parent) : QWidget(parent), doc(_doc)
{
  setupUi(this);
  StatListView->setColumnWidth(0, SIZE_GRADE + 10);
  StatListView->setColumnWidth(1, SIZE_GRADE + 10);
  StatListView->setColumnWidth(2, SIZE_COUNT);
  StatListView->setColumnWidth(3, SIZE_LESSON);

  vector<QString> lesson = doc->getLessonDescr();

  fsc.resize(lesson.size()+1);
  tsc.resize(lesson.size()+1);

  // accumulate numbers of grades per lesson
  for (int i = 0; i < (int) doc->numEntries(); i++) {
    kvoctrainExpr *expr = doc->getEntry(i);
    int fg = QMIN(KV_MAX_GRADE, (int)expr->getGrade(col, false));
    int tg = QMIN(KV_MAX_GRADE, (int)expr->getGrade(col, true));
    int l = expr->getLesson();
    if (l >= 0 && l <= (int) lesson.size() ) {
      fsc[l].grade[fg]++;
      fsc[l].num++;
      tsc[l].grade[tg]++;
      tsc[l].num++;
    }
  }
  setupPixmaps();
  connect(StatListView, SIGNAL( rightButtonPressed( Q3ListViewItem *, const QPoint& , int ) ),
          this, SLOT( slotRMB( Q3ListViewItem *, const QPoint &, int ) ) );
}


void StatistikPage::setupPixmaps()
{
  // create pixmaps with bar charts of numbers of grades
  int height;
  GradeListItem lvi (StatListView, 0);
  height = lvi.height();
  for (int entry = 0; entry < (int) fsc.size(); entry++) {
    QPainter p;
    QColor color;
    QPixmap fpix (SIZE_GRADE, height);
    p.begin( &fpix);
    p.eraseRect (0, 0, fpix.width(), fpix.height());
    p.setPen( Qt::black );
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
          case KV_NORM_GRADE: color = Prefs::gradeCol(0);    break;
          case KV_LEV1_GRADE: color = Prefs::gradeCol(1);    break;
          case KV_LEV2_GRADE: color = Prefs::gradeCol(2);    break;
          case KV_LEV3_GRADE: color = Prefs::gradeCol(3);    break;
          case KV_LEV4_GRADE: color = Prefs::gradeCol(4);    break;
          case KV_LEV5_GRADE: color = Prefs::gradeCol(5);    break;
          case KV_LEV6_GRADE: color = Prefs::gradeCol(6);    break;
          case KV_LEV7_GRADE: color = Prefs::gradeCol(7);    break;
          default           : color = Prefs::gradeCol(1);
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
      p.fillRect(PIX_SHIFT, 1, fpix.width()-PIX_SHIFT, height-1, Prefs::gradeCol(0));
      p.drawRect(PIX_SHIFT, 1, fpix.width()-PIX_SHIFT, height-1);
    }
    p.end();
    from_pix.push_back(fpix);

    QPixmap tpix (SIZE_GRADE, height);
    p.begin( &tpix );
    p.eraseRect (0, 0, tpix.width(), tpix.height());
    p.setPen( Qt::black );
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
          case KV_NORM_GRADE: color = Prefs::gradeCol(0);    break;
          case KV_LEV1_GRADE: color = Prefs::gradeCol(1);    break;
          case KV_LEV2_GRADE: color = Prefs::gradeCol(2);    break;
          case KV_LEV3_GRADE: color = Prefs::gradeCol(3);    break;
          case KV_LEV4_GRADE: color = Prefs::gradeCol(4);    break;
          case KV_LEV5_GRADE: color = Prefs::gradeCol(5);    break;
          case KV_LEV6_GRADE: color = Prefs::gradeCol(6);    break;
          case KV_LEV7_GRADE: color = Prefs::gradeCol(7);    break;
          default           : color = Prefs::gradeCol(1);
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
      p.fillRect(PIX_SHIFT, 1, tpix.width()-PIX_SHIFT, height-1, Prefs::gradeCol(0));
      p.drawRect(PIX_SHIFT, 1, tpix.width()-PIX_SHIFT, height-1);
    }
    p.end();
    to_pix.push_back(tpix);
  }

  // setup rows with pixmaps and strings
  vector<QString> lesson = doc->getLessonDescr();

  QString s;

  GradeListItem *plvi = new GradeListItem(StatListView, 0);
  plvi->setPixmap (TB_FGRADE, from_pix[0]);
  plvi->setPixmap (TB_TGRADE, to_pix[0]);
  s.setNum (tsc[0].num);
  plvi->setText (TB_COUNT, s);
  plvi->setText (TB_LESSON, i18n("<no lesson>"));
  StatListView->insertItem (plvi);

  for (int i = 0; i < (int) lesson.size(); i++) {
    plvi = new GradeListItem(StatListView, i+1);
    plvi->setPixmap (TB_FGRADE, from_pix[i+1]);
    plvi->setPixmap (TB_TGRADE, to_pix[i+1]);
    s.setNum (tsc[i+1].num);
    plvi->setText (TB_COUNT, s);
    plvi->setText (TB_LESSON, lesson[i]);
    StatListView->insertItem (plvi);
  }
}


void StatistikPage::slotRMB( Q3ListViewItem* Item, const QPoint & /*point*/, int col)
{
  if( Item != 0)
    slotPopupMenu(((GradeListItem*)Item)->getLesson(), col);
}


void StatistikPage::slotPopupMenu(int row, int col)
{
  struct stat_counter *sc;

  if (col == TB_FGRADE) {
    if (row >= (int) fsc.size() ) {
      kdError() << "row >= fsc.size()" << endl;
      return;
    }
    else
      sc = &fsc[row];
  }
  else if (col == TB_TGRADE) {
    if (row >= (int) tsc.size() ) {
      kdError() << "row >= tsc.size()" << endl;
      return;
    }
    else
      sc = &tsc[row];
  }
  else
    return;

  KMenu *header_m = new KMenu(i18n("Number of Entries per Grade"));

  header_m->insertItem (i18n(KV_NORM_TEXT) + "\t" + QString::number(sc->grade[KV_NORM_GRADE]) );
  header_m->insertItem (i18n(KV_LEV1_TEXT) + "\t" + QString::number(sc->grade[KV_LEV1_GRADE]) );
  header_m->insertItem (i18n(KV_LEV2_TEXT) + "\t" + QString::number(sc->grade[KV_LEV2_GRADE]) );
  header_m->insertItem (i18n(KV_LEV3_TEXT) + "\t" + QString::number(sc->grade[KV_LEV3_GRADE]) );
  header_m->insertItem (i18n(KV_LEV4_TEXT) + "\t" + QString::number(sc->grade[KV_LEV4_GRADE]) );
  header_m->insertItem (i18n(KV_LEV5_TEXT) + "\t" + QString::number(sc->grade[KV_LEV5_GRADE]) );
  header_m->insertItem (i18n(KV_LEV6_TEXT) + "\t" + QString::number(sc->grade[KV_LEV6_GRADE]) );
  header_m->insertItem (i18n(KV_LEV7_TEXT) + "\t" + QString::number(sc->grade[KV_LEV7_GRADE]) );

  header_m->exec(QCursor::pos()+QPoint(10, 0));
}

#include "StatistikPage.moc"
