/***************************************************************************

                      view to kvoctrain parts

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004 Peter Hedlund <peter@peterandlinda.com>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <kvoctraincore.h>
#include "kvoctrain.h"
#include "kvoctrainview.h"
#include "prefs.h"
#include "langset.h"

#include <kstandarddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kdebug.h>
#include <dcopclient.h>
#include <kapplication.h>
#include <kprinter.h>

#include <qlayout.h>
#include <qbitmap.h>
#include <qfile.h>

#include <ctype.h>

#define HEADER_MINSIZE   24
#define KV_COLWIDTH_MARK 20

kvoctrainView::kvoctrainView(kvoctrainDoc* doc, const LangSet &ls, kvoctrainApp *_parent) : QWidget(_parent)
{
  m_doc = doc;
  parent = _parent;

  m_table = new KVocTrainTable(m_doc, &ls, this, "ListBox_1");
  m_table->setLineWidth( 2 );

  if (m_doc->numLangs() == 0)
    m_doc->appendLang(i18n("Original"));

  connect(m_table, SIGNAL(selected(int)), m_table, SLOT(sortByColumn_alpha(int)));
  connect(m_table, SIGNAL(rightButtonClicked(int, int, int)), parent, SLOT(slotHeaderMenu(int, int, int)));
  connect(m_table, SIGNAL(edited(int, int)), parent, SLOT(slotEditEntry(int, int)));
  connect(m_table, SIGNAL(currentChanged(int, int)), parent, SLOT(slotCurrentCellChanged(int, int)));
  connect(m_table, SIGNAL(selectionChanged()), m_table, SLOT(slotSelectionChanged()));
  connect(m_table, SIGNAL(forwardKeyPressEvent(QKeyEvent*)), parent, SLOT(keyPressEvent(QKeyEvent *)));
  connect(m_table, SIGNAL(forwardKeyReleaseEvent(QKeyEvent*)), parent, SLOT(keyReleaseEvent(QKeyEvent *)));

  list_layout = new QGridLayout( this, 2, 1/*, 4 */);
  list_layout->addWidget(m_table, 1, 0);
  list_layout->setRowStretch(1, 1);
  list_layout->activate();

  setView(m_doc, ls);
}


void kvoctrainView::showEvent (QShowEvent * s_ev)
{
  QWidget::showEvent(s_ev);
  QResizeEvent r_ev(size(), size());
  resizeEvent(&r_ev);
}


void kvoctrainView::adjustContent()
{
  QResizeEvent r_ev(size(), size());
  resizeEvent (&r_ev);
}


void kvoctrainView::resizeEvent (QResizeEvent *r_ev)
{
  QWidget::resizeEvent(r_ev);

  if (r_ev == 0)
    return;

  QHeader *header = m_table->horizontalHeader();
  unsigned oldwidth = 0;
  for (int i = 0; i < m_table->numCols(); ++i)
    oldwidth += header->sectionSize(i);

  unsigned newwidth = m_table->clipper()->width();
  int remain = newwidth;

  switch (Prefs::headerResizeMode())
  {
    case Prefs::EnumHeaderResizeMode::Automatic:
    {
    // lesson is only half as wide as a original/translation
    // exclude fixed size of "mark"-column
      int x = (remain -KV_COLWIDTH_MARK) / ((m_table->numCols() - 1) * 2 - 1);
      header->resizeSection(KV_COL_LESS, x);
      m_table->setColumnWidth (KV_COL_LESS, x);
      remain -= x;
      header->resizeSection(KV_COL_MARK, KV_COLWIDTH_MARK);
      m_table->setColumnWidth (KV_COL_MARK, KV_COLWIDTH_MARK);
      remain -= KV_COLWIDTH_MARK;
      for (int i = KV_COL_ORG; i < m_table->numCols()-1; i++)
      {
        remain -= 2 * x;
        header->resizeSection(i, 2 * x);
        m_table->setColumnWidth (i, 2 * x);
      }
      header->resizeSection(m_table->numCols() - 1, remain);
      m_table->setColumnWidth(m_table->numCols() - 1, remain);
    }
    break;

    case Prefs::EnumHeaderResizeMode::Percent:
    {
      float grow = float(newwidth) / float(oldwidth);
      header->resizeSection(KV_COL_MARK, KV_COLWIDTH_MARK);
      m_table->setColumnWidth(KV_COL_MARK, KV_COLWIDTH_MARK);
      int remain = newwidth - KV_COLWIDTH_MARK;
      int x = QMAX(20, (int)((header->sectionSize(KV_COL_LESS) * grow) + 0.5));
      header->resizeSection(KV_COL_LESS, x);
      m_table->setColumnWidth(KV_COL_LESS, x);
      remain -= x;

      for (int i = KV_COL_ORG; i < m_table->numCols() - 1; i++)
      {
        x = QMAX(20, (int)((header->sectionSize(i) * grow) + 0.5));
        remain -= x;
        header->resizeSection(i, x);
        m_table->setColumnWidth(i, x);
      }
      header->resizeSection(m_table->numCols() - 1, QMAX(20, remain));
      m_table->setColumnWidth(m_table->numCols() - 1, QMAX(20, remain));
    }
    break;

    case Prefs::EnumHeaderResizeMode::Fixed:
      // nix
    break;
  }

  if (m_doc != 0) {
    for (int i = KV_COL_ORG; i < m_table->numCols(); i++)
    {
      m_doc->setSizeHint (i - KV_EXTRA_COLS, header->sectionSize(i));
    }
    m_doc->setSizeHint (-1, header->sectionSize(KV_COL_LESS));
  }
}


void kvoctrainView::setView(kvoctrainDoc *doc, const LangSet& ls)
{
  // set header
  m_doc = doc;
  m_table->setDoc(m_doc);
  if (m_doc) {
    int id = ls.indexShortId (m_doc->getOriginalIdent());

    setHeaderProp(KV_COL_LESS, i18n("Lesson"),  QString::null);
    setHeaderProp(KV_COL_MARK, "",  QString::null);
    m_table->setColumnWidth(KV_COL_MARK, KV_COLWIDTH_MARK);

    if (id < 0)
      setHeaderProp(KV_COL_ORG, m_doc->getOriginalIdent());
    else
      setHeaderProp(KV_COL_ORG, ls.longId(id), ls.PixMapFile(id));

    for (int i = KV_COL_TRANS; i < m_table->numCols(); i++)
    {
      int id = ls.indexShortId(m_doc->getIdent(i - KV_EXTRA_COLS));

      if (id < 0)
        setHeaderProp(i, m_doc->getIdent(i-KV_EXTRA_COLS));
      else
        setHeaderProp(i, ls.longId(id), ls.PixMapFile(id));
    }

    if (m_table->currentColumn() < KV_COL_ORG)
      m_table->setCurrentRow(m_table->currentRow(), KV_COL_ORG);
  }
  m_table->updateContents();
}


kvoctrainView::~kvoctrainView()
{
}


void kvoctrainView::setHeaderProp (int id, const QString &name, const QString &pixfile)
{
  QHeader *header = m_table->horizontalHeader();

  if (pixfile.isEmpty())
  {
    header->setLabel(id, name);
  }
  else
  {
    QPixmap pix;
    if (QFile::exists(pixfile))
      pix.load(pixfile);
    else
    {
      //this is the code used by kxkb when a flag image can't be found
      //see kdebase/kxkb/pixmap.cpp/LayoutIcon::findPixmap()
      pix.resize(21, 14);
      pix.fill(Qt::white);

      QPainter p(&pix);

      QFont font("sans");
      font.setPixelSize(10);
      font.setWeight(QFont::Bold);
      p.setFont(font);
      p.setPen(Qt::red);
      p.drawText(2, 1, pix.width(), pix.height()-2, Qt::AlignCenter, "err");
      p.setPen(Qt::blue);
      p.drawText(1, 0, pix.width(), pix.height()-2, Qt::AlignCenter, "err");
    }

    int w = pix.width();
    int h = pix.height();

    QPixmap arrow(w + 14, h);
    QBitmap mask(arrow.width(), arrow.height());
    arrow.fill(Qt::white);
    mask.fill(Qt::black);

    bitBlt(&arrow, 0, 0, &pix, 0, 0);
    if (pix.mask() != 0)
      bitBlt(&mask, 0, 0, pix.mask(), 0, 0);
    else {
      QBitmap bmp(pix.createHeuristicMask());
      bitBlt(&mask, 0, 0, &bmp, 0, 0);
    }

    QPainter p, pm;
    p.begin(&arrow);
    pm.begin(&mask);

    p.setPen(QColor(0, 0, 0));
    p.drawLine(w + 5, h - 2, w + 5 + 1, h - 2);
    p.drawLine(w + 4, h - 3, w + 6 + 1, h - 3);
    p.drawLine(w + 3, h - 4, w + 7 + 1, h - 4);
    p.drawLine(w + 2, h - 5, w + 8 + 1, h - 5);
    p.drawLine(w + 2, h - 6, w + 8 + 1, h - 6);

    pm.setPen(QColor(255, 255, 255));
    pm.drawLine(w + 5, h - 2, w + 5 + 1, h - 2);
    pm.drawLine(w + 4, h - 3, w + 6 + 1, h - 3);
    pm.drawLine(w + 3, h - 4, w + 7 + 1, h - 4);
    pm.drawLine(w + 2, h - 5, w + 8 + 1, h - 5);
    pm.drawLine(w + 2, h - 6, w + 8 + 1, h - 6);

    pm.end();
    p.end();
    arrow.setMask(mask);

    QIconSet set(arrow, QIconSet::Small);
    header->setLabel(id, set, name);
  }
}

void kvoctrainView::print(KPrinter * pPrinter)
{
  QPainter painter;

  //type 0 Vocabulary list
  int type = 0;
  //I think working with screen resolution is enough for our purposes
  int res = pPrinter->resolution();
  int pad = 2;
  int marg = res;

  painter.begin(pPrinter);

  int pageNum = 1;

  int cw0 = m_table->columnWidth(KV_COL_LESS);
  int cw1 = m_table->columnWidth(KV_COL_MARK);
  int cw2 = m_table->columnWidth(KV_COL_ORG);
  int cw3 = m_table->columnWidth(KV_COL_TRANS);

  int gridWidth = cw0 + cw1 + cw2 + cw3;
  int lPos = marg;
  int tPos = marg + m_table->horizontalHeader()->height();

  QRect w = painter.window();
  QRect cr;
  newPage(painter, res, type);

  for (int rc = 0; rc < m_table->numRows(); ++rc)
  {
    int rh = m_table->rowHeight(rc);

    painter.drawLine(lPos, tPos, lPos + gridWidth, tPos);

    painter.setFont(Prefs::tableFont());

    kvoctrainExpr *expr = m_table->getRow(rc);

    cr.setRect(lPos + pad, tPos, cw0 - pad, rh);

    QString less_str;
    if (m_doc != 0 && expr->getLesson() != 0)
      less_str = m_doc->getLessonDescr(expr->getLesson());
    painter.drawText(cr, AlignAuto | AlignVCenter, less_str);

    cr.setRect(lPos + cw0 + pad, tPos, cw1, rh);
    if (!expr->isActive())
      painter.drawPixmap(cr.left() + pad, cr.top() + pad, m_table->m_pixInactive);
    else if (expr->isInQuery())
      painter.drawPixmap(cr.left() + pad, cr.top() + pad, m_table->m_pixInQuery);

    cr.setRect(lPos + cw0 + cw1 + pad, tPos, cw2 - pad, rh);
    QString s = expr->getOriginal();
    painter.drawText(cr, AlignAuto | AlignVCenter, s);

    cr.setRect(lPos + cw0 + cw1 + cw2 + pad, tPos, cw3 - pad, rh);
    s = expr->getTranslation(1);
    painter.drawText(cr, AlignAuto | AlignVCenter, s);

    tPos = tPos + m_table->rowHeight(rc);

    if (tPos + m_table->rowHeight(rc + 1) > w.height() - marg)
    {
      endOfPage(painter, tPos, pageNum++, res, type);
      tPos = marg + m_table->horizontalHeader()->height();
      pPrinter->newPage();
      newPage(painter, res, type);
    }
  }

  endOfPage(painter, tPos, pageNum++,  res, type);
  painter.end();
}

void kvoctrainView::newPage(QPainter & painter, int res, int type)
{
  int cw0 = m_table->columnWidth(KV_COL_LESS);
  int cw1 = m_table->columnWidth(KV_COL_MARK);
  int cw2 = m_table->columnWidth(KV_COL_ORG);
  int cw3 = m_table->columnWidth(KV_COL_TRANS);
  int marg = res;
  int pad = 2;

  QRect w = painter.window();

  painter.setFont(KGlobalSettings::generalFont());

  painter.drawLine(marg, marg, marg + cw0 + cw1 + cw2 + cw3, marg);
  painter.drawText(marg, marg - 20, i18n("KVocTrain - %1").arg(m_doc->getTitle()));

  int hh = m_table->horizontalHeader()->height();
  painter.drawText(marg + pad, marg, cw0 - pad, hh, AlignAuto | AlignVCenter, m_table->horizontalHeader()->label(0));
  painter.drawText(marg + cw0 + pad, marg, cw1 - pad, hh, AlignAuto | AlignVCenter, m_table->horizontalHeader()->label(1));
  painter.drawText(marg + cw0 + cw1 + pad, marg, cw2 - pad, hh, AlignAuto | AlignVCenter, m_table->horizontalHeader()->label(2));
  painter.drawText(marg + cw0 + cw1 + cw2 + pad, marg, cw3 - pad, hh, AlignAuto | AlignVCenter, m_table->horizontalHeader()->label(3));
}

void kvoctrainView::endOfPage(QPainter & painter, int vPos, int pageNum, int res, int type)
{
  int marg = res;
  painter.setFont(KGlobalSettings::generalFont());
  QRect w = painter.window();
  QRect r = painter.boundingRect(0, 0, 0, 0, AlignAuto, QString::number(pageNum));
  painter.drawText((w.width()/2) - (r.width()/2), w.height() - marg + 20, QString::number(pageNum));

  int cw0 = m_table->columnWidth(KV_COL_LESS);
  int cw1 = m_table->columnWidth(KV_COL_MARK);
  int cw2 = m_table->columnWidth(KV_COL_ORG);
  int cw3 = m_table->columnWidth(KV_COL_TRANS);

  //Last horizontal line
  painter.drawLine(marg, vPos, marg + cw0 + cw1 + cw2 + cw3, vPos);
  //Five vertical lines
  painter.drawLine(marg, marg, marg, vPos);
  painter.drawLine(marg + cw0, marg, marg + cw0, vPos);
  painter.drawLine(marg + cw0 + cw1, marg, marg + cw0 + cw1, vPos);
  painter.drawLine(marg + cw0 + cw1 + cw2, marg, marg + cw0 + cw1 + cw2, vPos);
  painter.drawLine(marg + cw0 + cw1 + cw2 + cw3, marg, marg + cw0 + cw1 + cw2 + cw3, vPos);
}


#include "kvoctrainview.moc"
