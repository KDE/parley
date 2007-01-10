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

#include <QCursor>
#include <QPainter>
#include <QPixmap>

#include <klocale.h>

#include <keduvocdocument.h>

#include <prefs.h>
#include "StatisticsPage.h"

#define MIN_COL_WIDTH      2
#define PIX_SHIFT          2
#define SIZE_GRADE         100+PIX_SHIFT
#define SIZE_COUNT         70
#define SIZE_LESSON        300

#define TB_FGRADE          0
#define TB_TGRADE          1
#define TB_COUNT           2
#define TB_LESSON          3

///@todo get this beast properly localized
const QString toolTipTemplate = "<html><p style='white-space:pre'><b>Number of Entries per Grade</b></p>"
                                "<table><tr><td>%1</td><td>%2</td></tr>"
                                       "<tr><td>%3</td><td>%4</td></tr>"
                                       "<tr><td>%5</td><td>%6</td></tr>"
                                       "<tr><td>%7</td><td>%8</td></tr>"
                                       "<tr><td>%9</td><td>%10</td></tr>"
                                       "<tr><td>%11</td><td>%12</td></tr>"
                                       "<tr><td>%13</td><td>%14</td></tr>"
                                       "<tr><td>%15</td><td>%16</td></tr>"
                                "</table></html>";

class GradeListItem : public QTreeWidgetItem
{
public:
  inline GradeListItem (QTreeWidget * parent, int _lesson): QTreeWidgetItem(parent), lesson(_lesson) {}
  inline int getLesson() const { return lesson; }

private:
  int lesson;
};


StatisticsPage::StatisticsPage(int col, KEduVocDocument  *_doc, QWidget *parent) : QWidget(parent), doc(_doc)
{
  setupUi(this);
  StatListView->setColumnWidth(0, SIZE_GRADE + 10);
  StatListView->setColumnWidth(1, SIZE_GRADE + 10);
  StatListView->setColumnWidth(2, SIZE_COUNT);
  StatListView->setColumnWidth(3, SIZE_LESSON);

  QStringList lesson = doc->lessonDescriptions();

  fsc.resize(lesson.size() + 1);
  tsc.resize(lesson.size() + 1);

  // accumulate numbers of grades per lesson
  for (int i = 0; i < (int) doc->numEntries(); i++) {
    KEduVocExpression *expr = doc->entry(i);
    int fg = qMin(KV_MAX_GRADE, (int) expr->grade(col, false));
    int tg = qMin(KV_MAX_GRADE, (int) expr->grade(col, true));
    int l = expr->lesson();
    if (l >= 0 && l <= (int) lesson.size() ) {
      fsc[l].grade[fg]++;
      fsc[l].num++;
      tsc[l].grade[tg]++;
      tsc[l].num++;
    }
  }
  setupPixmaps();
}


void StatisticsPage::setupPixmaps()
{
  // create pixmaps with bar charts of numbers of grades
  int height;
  //GradeListItem lvi(StatListView, 0);
  height = 22; //lvi.sizeHint(0).height();
  for (int entry = 0; entry < (int) fsc.size(); entry++) {
    QPainter p;
    QColor color;
    QPixmap fpix (SIZE_GRADE, height);
    p.begin(&fpix);
    p.eraseRect(0, 0, fpix.width(), fpix.height());
    p.setPen(Qt::black);
    if (fsc[entry].num != 0) {

      int num = 0;
      for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++)
        if (fsc[entry].grade[j] != 0)
          num++;

      int maxw = fpix.width() - PIX_SHIFT - PIX_SHIFT - 1;
      int w = maxw;
      int widths[KV_MAX_GRADE + 1];
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
            widths[j] = qMax(MIN_COL_WIDTH, fsc[entry].grade[j] * maxw / fsc[entry].num);
            w -= widths[j];
          }
        }
      }

      int x = 0;
      int x2 = 1 + PIX_SHIFT;
      for (int j = KV_MIN_GRADE; j <= KV_MAX_GRADE; j++) {
        switch (j) {
          case KV_NORM_GRADE: color = Prefs::gradeCol(0); break;
          case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
          case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
          case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
          case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
          case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
          case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
          case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
          default           : color = Prefs::gradeCol(1); break;
        }
        if (widths[j] != 0) {
          x2 += widths[j];
          p.fillRect(x + PIX_SHIFT, 1, x2 - x, height - 1, color);
          p.drawRect(x + PIX_SHIFT, 1, x2 - x, height - 1);
          x = x2 - 1;
        }
      }
    }
    else {
      p.fillRect(PIX_SHIFT, 1, fpix.width() - PIX_SHIFT, height - 1, Prefs::gradeCol(0));
      p.drawRect(PIX_SHIFT, 1, fpix.width() - PIX_SHIFT, height - 1);
    }
    p.end();
    from_pix.append(fpix);

    QPixmap tpix(SIZE_GRADE, height);
    p.begin(&tpix);
    p.eraseRect(0, 0, tpix.width(), tpix.height());
    p.setPen(Qt::black);
    if (tsc[entry].num != 0) {
      int num = 0;
      for (int j = KV_NORM_GRADE; j <= KV_MAX_GRADE; j++)
        if (tsc[entry].grade[j] != 0)
          num++;

      int maxw = tpix.width() - PIX_SHIFT - PIX_SHIFT - 1;
      int w = maxw;
      int widths[KV_MAX_GRADE+1];
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
            widths[j] = qMax(MIN_COL_WIDTH, tsc[entry].grade[j] * maxw / tsc[entry].num);
            w -= widths[j];
          }
        }
      }

      int x = 0;
      int x2 = 1+PIX_SHIFT;
      for (int j = KV_MIN_GRADE; j <= KV_MAX_GRADE; j++) {
        switch (j) {
          case KV_NORM_GRADE: color = Prefs::gradeCol(0); break;
          case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
          case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
          case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
          case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
          case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
          case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
          case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
          default           : color = Prefs::gradeCol(1); break;
        }
        if (widths[j] != 0) {
          x2 += widths[j];
          p.fillRect(x + PIX_SHIFT, 1, x2 - x, height - 1, color);
          p.drawRect(x + PIX_SHIFT, 1, x2 - x, height - 1);
          x = x2-1;
        }
      }
    }
    else {
      p.fillRect(PIX_SHIFT, 1, tpix.width() - PIX_SHIFT, height - 1, Prefs::gradeCol(0));
      p.drawRect(PIX_SHIFT, 1, tpix.width() - PIX_SHIFT, height - 1);
    }
    p.end();
    to_pix.push_back(tpix);
  }

  // setup rows with pixmaps and strings
  QString s;
  GradeListItem *plvi = 0;

  for (int i = 0; i <= (int) doc->lessonDescriptions().count(); i++) {
    plvi = new GradeListItem(StatListView, i);
    plvi->setData(TB_FGRADE, Qt::DecorationRole, QVariant(from_pix[i]));
    plvi->setData(TB_TGRADE, Qt::DecorationRole, QVariant(to_pix[i]));

    s = toolTipTemplate.arg(i18n(KV_NORM_TEXT)).arg(QString::number(fsc[i].grade[KV_NORM_GRADE]))
                       .arg(i18n(KV_LEV1_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV1_GRADE]))
                       .arg(i18n(KV_LEV2_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV2_GRADE]))
                       .arg(i18n(KV_LEV3_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV3_GRADE]))
                       .arg(i18n(KV_LEV4_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV4_GRADE]))
                       .arg(i18n(KV_LEV5_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV5_GRADE]))
                       .arg(i18n(KV_LEV6_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV6_GRADE]))
                       .arg(i18n(KV_LEV7_TEXT)).arg(QString::number(fsc[i].grade[KV_LEV7_GRADE]));
    plvi->setToolTip(TB_FGRADE, s);

    s = toolTipTemplate.arg(i18n(KV_NORM_TEXT)).arg(QString::number(tsc[i].grade[KV_NORM_GRADE]))
                       .arg(i18n(KV_LEV1_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV1_GRADE]))
                       .arg(i18n(KV_LEV2_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV2_GRADE]))
                       .arg(i18n(KV_LEV3_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV3_GRADE]))
                       .arg(i18n(KV_LEV4_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV4_GRADE]))
                       .arg(i18n(KV_LEV5_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV5_GRADE]))
                       .arg(i18n(KV_LEV6_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV6_GRADE]))
                       .arg(i18n(KV_LEV7_TEXT)).arg(QString::number(tsc[i].grade[KV_LEV7_GRADE]));
    plvi->setToolTip(TB_TGRADE, s);

    s.setNum (tsc[i].num);
    plvi->setText(TB_COUNT, s);
    plvi->setText(TB_LESSON, doc->lessonDescription(i));

    StatListView->addTopLevelItem(plvi);
  }
}

#include "StatisticsPage.moc"
