/***************************************************************************

                      view to kvoctrain parts

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <ctype.h>

#include <QLayout>
#include <QBitmap>
#include <QFile>
#include <QPixmap>
#include <QGridLayout>
#include <QKeyEvent>
#include <QShowEvent>
#include <QResizeEvent>

#include <kstandarddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kprinter.h>

#include "kvoctrain.h"
#include "kvoctrainview.h"
#include "prefs.h"
#include "langset.h"

#define HEADER_MINSIZE   24
#define KV_COLWIDTH_MARK 20

kvoctrainView::kvoctrainView(KEduVocDocument* doc, const LangSet &ls, KVocTrainApp *_parent) : QWidget(_parent)
{
  m_doc = doc;
  parent = _parent;

  m_table = new KVocTrainTable(m_doc, &ls, this, "ListBox_1");
  m_table->setFont(Prefs::tableFont());
  m_table->setLineWidth( 2 );

  if (m_doc->numIdentifiers() == 0)
    m_doc->appendIdentifier(i18n("Original"));

  connect(m_table, SIGNAL(selected(int)), m_table, SLOT(sortByColumn_alpha(int)));
  connect(m_table, SIGNAL(rightButtonClicked(int, int, int)), parent, SLOT(slotHeaderMenu(int, int, int)));
  connect(m_table, SIGNAL(edited(int, int)), parent, SLOT(slotEditEntry(int, int)));
  connect(m_table, SIGNAL(currentChanged(int, int)), parent, SLOT(slotCurrentCellChanged(int, int)));
  connect(m_table, SIGNAL(selectionChanged()), m_table, SLOT(slotSelectionChanged()));
  connect(m_table, SIGNAL(forwardKeyPressEvent(QKeyEvent*)), parent, SLOT(keyPressEvent(QKeyEvent *)));
  connect(m_table, SIGNAL(forwardKeyReleaseEvent(QKeyEvent*)), parent, SLOT(keyReleaseEvent(QKeyEvent *)));

  list_layout = new QGridLayout( this );
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

  Q3Header *header = m_table->horizontalHeader();
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
      int x = qMax(20, (int)((header->sectionSize(KV_COL_LESS) * grow) + 0.5));
      header->resizeSection(KV_COL_LESS, x);
      m_table->setColumnWidth(KV_COL_LESS, x);
      remain -= x;

      for (int i = KV_COL_ORG; i < m_table->numCols() - 1; i++)
      {
        x = qMax(20, (int)((header->sectionSize(i) * grow) + 0.5));
        remain -= x;
        header->resizeSection(i, x);
        m_table->setColumnWidth(i, x);
      }
      header->resizeSection(m_table->numCols() - 1, qMax(20, remain));
      m_table->setColumnWidth(m_table->numCols() - 1, qMax(20, remain));
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


void kvoctrainView::setView(KEduVocDocument *doc, const LangSet& ls)
{
  // set header
  m_doc = doc;
  m_table->setDoc(m_doc);
  if (m_doc) {
    int id = ls.indexShortId (m_doc->originalIdentifier());

    setHeaderProp(KV_COL_LESS, i18n("Lesson"),  QString());
    setHeaderProp(KV_COL_MARK, "",  QString());
    m_table->setColumnWidth(KV_COL_MARK, KV_COLWIDTH_MARK);

    if (id < 0)
      setHeaderProp(KV_COL_ORG, m_doc->originalIdentifier());
    else
      setHeaderProp(KV_COL_ORG, ls.longId(id), ls.PixMapFile(id));

    for (int i = KV_COL_TRANS; i < m_table->numCols(); i++)
    {
      int id = ls.indexShortId(m_doc->identifier(i - KV_EXTRA_COLS));

      if (id < 0)
        setHeaderProp(i, m_doc->identifier(i-KV_EXTRA_COLS));
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
  Q3Header *header = m_table->horizontalHeader();

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
      pix = QPixmap(21, 14);
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

    QBitmap pixMask = pix.mask();

    if (!pixMask.isNull())
      bitBlt(&mask, 0, 0, &pixMask, 0, 0);
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

    QIcon set(arrow/*, QIcon::Small*/);
    header->setLabel(id, set, name);
  }
}

#include "kvoctrainview.moc"
