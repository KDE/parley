/***************************************************************************

                      table for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Jan 4 19:04:53 PST 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

#include <qpainter.h>

#include <kapplication.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <dcopclient.h>
#include <klocale.h>
#include <kglobalsettings.h>

#include "kvoctraintable.h"
#include "kv_resource.h"

KVocTrainTable::KVocTrainTable(kvoctrainDoc *doc, const LangSet *ls, const GradeCols *gc, QWidget *parent, const char *name)
  : QTable(parent, name), langs(ls)
{
  m_doc = doc;
  defaultItem = 0;
  setNumCols(m_doc->numLangs());
  setNumRows(m_doc->numEntries());

  setLeftMargin(0);
  setSelectionMode(MultiRow);
  setColumnMovingEnabled(false);
  setRowMovingEnabled(false);
  setSorting(false);

  setDoc(m_doc, gc);

  triggerSect = -1;

  delayTimer = new QTimer(this);
  connect(delayTimer, SIGNAL(timeout()), this, SLOT(menuTriggerTimeout()));

  QHeader *header = horizontalHeader();
  connect(header, SIGNAL(pressed(int)), this, SLOT(headerPressEvent(int)));
  connect(header, SIGNAL(released(int)), this, SLOT(headerReleaseEvent(int)));
}


void KVocTrainTable::setCurrentItem(int row)
{
  setCurrentRow(row, currentColumn());
}

QWidget* KVocTrainTable::beginEdit(int row, int col, bool replace)
{
  if (KApplication::dcopClient()->isApplicationRegistered("kxkb")) {

    if (m_doc) {
      QString id = (col == KV_COL_ORG) ? m_doc->getOriginalIdent()
        : m_doc->getIdent(col - KV_EXTRA_COLS);

      if (langs) {
        QString kbLayout(langs->keyboardLayout(langs->indexShortId(id)));
        if (!kbLayout.isEmpty()) {
          QByteArray data, replyData;
          QCString replyType;
          QDataStream arg(data, IO_WriteOnly);
          arg << kbLayout;

          if (!KApplication::dcopClient()->call("kxkb", "kxkb",
               "setLayout(QString)",
               data, replyType, replyData)) {
                 kdDebug() << "kskb dcop error" << endl;
               }
        }
      }
    }
  }
  return QTable::beginEdit(row, col, replace);
}

void KVocTrainTable::endEdit(int row, int col, bool accept, bool replace)
{
//   if (KApplication::dcopClient()->isApplicationRegistered("kxkb")) {
//     QByteArray data, replyData;
//     QCString replyType;

//     if (!KApplication::dcopClient()->call("kxkb", "kxkb",
//            "setLayout(QString)",
//            data, replyType, replyData)) {
//   }
  QTable::endEdit(row, col, accept, replace);
}

void KVocTrainTable::sortByColumn(int header, bool alpha) {
  if (header == KV_COL_MARK)
    return;

  if (header >= numRows()) {
    kdError() << "header >= numRows()\n";
    return;
  }

  if (m_doc && !m_doc->isAllowedSorting()) {
    KMessageBox::information(this, i18n("Sorting is currently turned off for this document.\n"
      "\nUse the document properties dialog to turn sorting on."), kapp->makeStdCaption(""));
    return;
  }

  QApplication::setOverrideCursor(waitCursor);

  clearSelection();

  bool sortdir = false;
  if (m_doc) {
    if (header >= KV_COL_ORG)
      sortdir = m_doc->sort(header-KV_EXTRA_COLS);
    else
      if (alpha)
        sortdir = m_doc->sortByLesson_alpha();
    else
      sortdir = m_doc->sortByLesson_index();
  }
  horizontalHeader()->setSortIndicator(header, sortdir);
  repaintContents();
  m_doc->setModified();
  emit currentChanged(currentRow(), currentColumn());
  QApplication::restoreOverrideCursor();
}

void KVocTrainTable::sortByColumn_alpha(int header)
{
  sortByColumn(header, true);
}


void KVocTrainTable::slotSelectionChanged()
{
  emit currentChanged(currentRow(), currentColumn());
}


void KVocTrainTable::sortByColumn_index(int header)
{
  sortByColumn(header, false);
}

void KVocTrainTable::setCurrentRow(int row, int col)
{
  QTable::setCurrentCell(row, col);
}

void KVocTrainTable::updateContents(int row, int col)
{
  int current_row = row;
  if (current_row < 0)
    current_row = currentRow();
  int current_col = col;
  if (current_col < 0)
    current_col = currentColumn();
  int ncols, nrows;
  if (m_doc) {
    nrows = m_doc->numEntries();
    ncols = QMAX(1, m_doc->numLangs())+KV_EXTRA_COLS;
  }
  else {
    nrows = 0;
    ncols = 1+KV_EXTRA_COLS;
  }
  if (nrows != numRows())
    setNumRows(nrows);
  if (ncols != numCols())
    setNumCols(ncols);
  current_row = QMIN(current_row, QMAX(0, numRows()-1));
  current_col = QMIN(current_col, QMAX(0, numCols()-1));
  bool b = signalsBlocked();
  blockSignals(true);
  setCurrentRow(current_row, current_col);
  blockSignals(b);
  update();
  emit currentChanged(current_row, current_col);
}

kvoctrainExpr *KVocTrainTable::getRow(int row)
{
  if (m_doc)
    return m_doc->getEntry(row);
  else
    return 0;
}

void KVocTrainTable::setDoc(kvoctrainDoc * rows, const GradeCols * gc)
{
  if (defaultItem)
    endEdit(defaultItem->row(), defaultItem->col(), true, false);

  KVocTrainTableItem *d = defaultItem;
  defaultItem = 0;

  if (rows) {
    m_doc = rows;
    setNumRows(rows->numEntries());
    setNumCols(QMAX(1+KV_EXTRA_COLS, m_doc->numLangs()+KV_EXTRA_COLS));
    setCurrentRow(0, 0);
  }
  else {
    setNumRows(0);
    setNumCols(1+KV_EXTRA_COLS);
    m_doc = 0;
  }

  if (d == 0) {
    defaultItem = new KVocTrainTableItem(this, QTableItem::OnTyping, rows);
  }
  else {
    defaultItem = d;
    defaultItem->setDoc(rows);
  }
  gradecols = gc;
}

void KVocTrainTable::menuTriggerTimeout()
{
  delayTimer->stop();
  if (triggerSect == -1)
    return;

  int mt = triggerSect;
  triggerSect = -1;

  QHeader *header = horizontalHeader();
  int x = leftMargin();
  for (int i = 0; i < mt; ++i)
    x += header->sectionSize(i);
  QPoint mpos = mapToGlobal(QPoint(x, topMargin()));

  if (mt != KV_COL_MARK)
    emit rightButtonClicked(mt, mpos.x(), mpos.y());

  QMouseEvent me(QEvent::MouseButtonRelease, QPoint(0, 0), QMouseEvent::LeftButton, QMouseEvent::LeftButton);
  QApplication::sendEvent(header, &me);
}

void KVocTrainTable::headerReleaseEvent(int /*sec*/)
{
  delayTimer->stop();
  if(triggerSect == -1) {             // long enough pressed for popup menu
    return;
  }
  int mt = triggerSect;
  triggerSect = -1;
  emit selected(mt);
}

void KVocTrainTable::headerPressEvent(int sec)
{
  triggerSect = sec;
  delayTimer->stop();
  delayTimer->start(500, true);
}

void KVocTrainTable::setFont(const QFont & font)
{
  QTable::setFont(font);
  horizontalHeader()->setFont(KGlobalSettings::generalFont());
  for (unsigned i = 0; i < (unsigned) numRows(); ++i)
    setRowHeight(i, fontMetrics().lineSpacing());
}

void KVocTrainTable::paintCell(QPainter * p, int row, int col, const QRect & cr, bool selected)
{
  if (cr.width() == 0 || cr.height() == 0)
    return;

  QColorGroup cg = colorGroup();

  int w = cr.width();
  int h = cr.height();
  int x2 = w - 1;
  int y2 = h - 1;

  p->fillRect(0, 0, w, h, selected ? cg.brush(QColorGroup::Highlight) : cg.brush(QColorGroup::Base));

  kvoctrainExpr *cell = getRow(row);
  if(cell) {
    p->save();
    cell->paint(p, col, w, selected, m_doc, numCols() == KV_EXTRA_COLS+2 ? KV_COL_TRANS : currentColumn(), gradecols);
    p->restore();
  }

  // We are drawing our own grid lines
  QPen pen(p->pen());
  p->setPen(colorGroup().mid());
  p->drawLine(x2, 0, x2, y2);
  p->drawLine(0, y2, x2, y2);
  p->setPen(pen);
}

void KVocTrainTable::setItem(int row, int col, QTableItem * /*item*/)
{
  // ignore item!
  updateContents(row, col);
}

QTableItem * KVocTrainTable::item(int row, int col) const
{
 if (defaultItem)
  defaultItem->setPosition(row, col);
  return defaultItem;
}

void KVocTrainTable::keyPressEvent(QKeyEvent * e)
{
  delayTimer->stop();
  switch(e->key())
  {
    case Key_Right: {
      int topCell = rowAt(0);
      int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
      if (numCols() > 2)
        for (int i = topCell; i <= lastRowVisible; i++)
          updateCell(i, KV_COL_ORG);
    }  // fallthrough
    case Key_Up:
    case Key_Down:
    case Key_Next:
    case Key_Prior:
      QTable::keyPressEvent(e);
      break;

      case Key_Left: {
        QTable::keyPressEvent(e);
        int topCell = rowAt(0);
        int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
        if (numCols() > 2)
          for (int i = topCell; i <= lastRowVisible; i++)
            updateCell(i, KV_COL_ORG);
      }
      break;

    case Key_Shift:
    case Key_Alt:
      case Key_Control:  // fallthrough
        QTable::keyPressEvent(e);
        emit forwardKeyPressEvent(e);
        break;

    case Key_Minus:
    case Key_Plus:
    case Key_Tab:
      case Key_Backtab:  // fallthrough
        emit forwardKeyPressEvent(e);
        break;

    default:
      QTable::keyPressEvent(e);
      break;
  }
}

void KVocTrainTable::keyReleaseEvent(QKeyEvent * e)
{
  delayTimer->stop();
  switch(e->key())
  {
    case Key_Shift:
    case Key_Alt:
    case Key_Control:  // fallthrough
      QTable::keyPressEvent(e);
      emit forwardKeyReleaseEvent(e);
      break;
  }
}

void KVocTrainTable::contentsMousePressEvent(QMouseEvent * e)
{
  delayTimer->stop();
  int cc = columnAt(e->x());
  int cr = rowAt(e->y());
  int co = currentColumn();

  QTable::contentsMousePressEvent(e);

  if(cc >= KV_EXTRA_COLS) {
    // update color of original when column changes and more than 1 translation
    bool update_org = false;
    if (cc != co && numCols() > 2)
      update_org = true;

    int topCell = rowAt(0);
    int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
    if (update_org)
      for (int i = topCell; i <= lastRowVisible; i++)
        updateCell(i, KV_COL_ORG);
  }
  if(e->button() == LeftButton)
    setCurrentCell(cr, cc);
}

void KVocTrainTable::contentsMouseDoubleClickEvent(QMouseEvent * e)
{
  delayTimer->stop();
  QTable::contentsMouseDoubleClickEvent(e);
}

void KVocTrainTable::activateNextCell()
{
  int currRow = currentRow();
  int currCol = currentColumn();

  QTableSelection currentSel = selection(0);

  int tr = currentSel.topRow();
  int br = currentSel.bottomRow();
  int lc = currentSel.leftCol();
  int rc = currentSel.rightCol();

  if (currCol == rc)
  {
    if (currRow < br)
      setCurrentCell(currRow + 1, lc);
    else
      setCurrentCell(tr, lc);
  }
  else
    setCurrentCell(currRow, currCol + 1);

}

#include "kvoctraintable.moc"
