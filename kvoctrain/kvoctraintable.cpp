/***************************************************************************

                      table for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Jan 4 19:04:53 PST 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <QPainter>
#include <QStyle>
#include <QPixmap>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <kapplication.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <dcopclient.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <kstandarddirs.h>
#include <kiconloader.h>

#include "kvoctraintable.h"
#include "kv_resource.h"
#include "prefs.h"

KVocTrainTable::KVocTrainTable(kvoctrainDoc *doc, const LangSet *ls, QWidget *parent, const char *name)
  : Q3Table(parent, name), langs(ls)
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

  setDoc(m_doc);

  triggerSect = -1;

  m_pixInQuery = QPixmap(KGlobal::iconLoader()->loadIcon("ok", KIcon::Small));
  m_pixInactive = QPixmap(KGlobal::iconLoader()->loadIcon("no", KIcon::Small));

  delayTimer = new QTimer(this);
  connect(delayTimer, SIGNAL(timeout()), this, SLOT(menuTriggerTimeout()));

  Q3Header *header = horizontalHeader();
  connect(header, SIGNAL(pressed(int)), this, SLOT(headerPressEvent(int)));
  connect(header, SIGNAL(released(int)), this, SLOT(headerReleaseEvent(int)));
}


void KVocTrainTable::setCurrentItem(int row)
{
  setCurrentRow(row, currentColumn());
}

QWidget* KVocTrainTable::beginEdit(int row, int col, bool replace)
{
  ///@todo
  ///port
  /*
  if (KApplication::dcopClient()->isApplicationRegistered("kxkb")) {

    if (m_doc) {
      QString id = (col == KV_COL_ORG) ? m_doc->getOriginalIdent()
        : m_doc->getIdent(col - KV_EXTRA_COLS);

      if (langs) {
        QString kbLayout(langs->keyboardLayout(langs->indexShortId(id)));
        if (!kbLayout.isEmpty()) {
          QByteArray data, replyData;
          Q3CString replyType;
          QDataStream arg(data, QIODevice::WriteOnly);
          arg << kbLayout;

          if (!KApplication::dcopClient()->call("kxkb", "kxkb",
               "setLayout(QString)",
               data, replyType, replyData)) {
                 kdDebug() << "kskb dcop error" << endl;
               }
        }
      }
    }
  }*/
  return Q3Table::beginEdit(row, col, replace);
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
  Q3Table::endEdit(row, col, accept, replace);
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

  QApplication::setOverrideCursor(Qt::WaitCursor);

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
  Q3Table::setCurrentCell(row, col);
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
  viewport()->update();
  emit currentChanged(current_row, current_col);
}

kvoctrainExpr *KVocTrainTable::getRow(int row)
{
  if (m_doc)
    return m_doc->getEntry(row);
  else
    return 0;
}

void KVocTrainTable::setDoc(kvoctrainDoc * rows)
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
    defaultItem = new KVocTrainTableItem(this, Q3TableItem::OnTyping, rows);
  }
  else {
    defaultItem = d;
    defaultItem->setDoc(rows);
  }
  //gradecols = gc;
}

void KVocTrainTable::menuTriggerTimeout()
{
  delayTimer->stop();
  if (triggerSect == -1)
    return;

  int mt = triggerSect;
  triggerSect = -1;

  Q3Header *header = horizontalHeader();
  int x = leftMargin();
  for (int i = 0; i < mt; ++i)
    x += header->sectionSize(i);
  QPoint mpos = mapToGlobal(QPoint(x, topMargin()));

  if (mt != KV_COL_MARK)
    emit rightButtonClicked(mt, mpos.x(), mpos.y());

  QMouseEvent me(QEvent::MouseButtonRelease, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton);
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
  Q3Table::setFont(font);
  horizontalHeader()->setFont(KGlobalSettings::generalFont());
  for (unsigned i = 0; i < (unsigned) numRows(); ++i)
    setRowHeight(i, fontMetrics().lineSpacing());
}

int cellAlignment(const QString & text)
{
  bool num;
  bool ok1 = false;
  bool ok2 = false;
  text.toInt(&ok1);
  if (!ok1)
    text.toDouble(&ok2);
  num = ok1 || ok2;

  return ( num ? Qt::AlignRight : Qt::AlignAuto ) | Qt::AlignVCenter;
}


void KVocTrainTable::paintCell(QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup &cg)
{
  if (cr.width() == 0 || cr.height() == 0)
    return;

  if (selected && row == currentRow() && col == currentColumn() && (hasFocus() || viewport()->hasFocus()))
    selected = false;

  int w = cr.width();
  int h = cr.height();
  int x2 = w - 1;
  int y2 = h - 1;

  p->fillRect( 0, 0, w, h, selected ? cg.brush( QColorGroup::Highlight ) : cg.brush( QColorGroup::Base ) );

  kvoctrainExpr *expr = getRow(row);
  if (expr)
  {
    p->save();
    //cell->paint(p, col, w, selected, m_doc, numCols() == KV_EXTRA_COLS+2 ? KV_COL_TRANS : currentColumn(), gradecols);

    QColor color = KV_NORM_COLOR;
    int current_col = numCols() == KV_EXTRA_COLS+2 ? KV_COL_TRANS : currentColumn();

    if (Prefs::useGradeCol())
    {
      if (col > KV_COL_ORG)
      {
        color = Prefs::gradeCol(0);
        if (expr->getQueryCount(col - KV_EXTRA_COLS, false) != 0)
        {
          switch (expr->getGrade(col-KV_EXTRA_COLS, false))
          {
            case KV_NORM_GRADE: color = Prefs::gradeCol(0); break;
            case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
            case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
            case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
            case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
            case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
            case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
            case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
            default           : color = Prefs::gradeCol(1);
          }
        }
      }
      else if ( col == KV_COL_ORG )
      {
        color = Prefs::gradeCol(0);
        if (expr->numTranslations() != 0 && current_col > KV_COL_ORG )
        {
          if (expr->getQueryCount(current_col - KV_EXTRA_COLS, true) != 0 )
          {
            switch (expr->getGrade(current_col - KV_EXTRA_COLS, true))
            {
              case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
              case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
              case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
              case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
              case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
              case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
              case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
              default           : color = Prefs::gradeCol(1);
            }
          }
        }
      }
    }

    if (selected)
      p->setPen (cg.highlightedText());
    else
      p->setPen (color);

    int fontpos = (p->fontMetrics().lineSpacing() - p->fontMetrics().lineSpacing()) / 2;

    switch (col)
    {
      case KV_COL_LESS: // lesson
      {
        QString less_str;
        if (m_doc != 0 && expr->getLesson() != 0)
          less_str = m_doc->getLessonDescr(expr->getLesson());
        p->drawText( 3, fontpos, w, p->fontMetrics().lineSpacing(), Qt::AlignLeft, less_str);
      }
      break;

      case KV_COL_MARK: // mark
      {
        if (!expr->isActive())
        {
          p->drawPixmap((w - m_pixInactive.width()) / 2,
                         (p->fontMetrics().lineSpacing() - m_pixInactive.height())/2, m_pixInactive);
        }
        else if (expr->isInQuery() )
        {
          p->drawPixmap((w - m_pixInQuery.width()) / 2,
                         (p->fontMetrics().lineSpacing() - m_pixInQuery.height())/2, m_pixInQuery);
        }
      }
      break;

      case KV_COL_ORG: // original
      {
        QString s = expr->getOriginal();
        p->drawText(3, fontpos, w - 6, p->fontMetrics().lineSpacing(), cellAlignment(s), s);
      }
      break;

      default: // translation x
        QString s = expr->getTranslation(col - KV_COL_ORG);
        p->drawText(3, fontpos, w - 6, p->fontMetrics().lineSpacing(), cellAlignment(s), s);
        break;
    }
    p->restore();
  }

  QPen pen( p->pen() );
  int gridColor = 0; ///@todo port style().styleHint( QStyle::SH_Table_GridLineColor, this );
  if (gridColor != -1) {
    const QPalette &pal = palette();
    if (cg != colorGroup()
        && cg != pal.disabled()
        && cg != pal.inactive())
      p->setPen(cg.mid());
    else
      p->setPen((QRgb)gridColor);
  } else {
    p->setPen(cg.mid());
  }
  p->drawLine( x2, 0, x2, y2 );
  p->drawLine( 0, y2, x2, y2 );
  p->setPen( pen );
}

void KVocTrainTable::setItem(int row, int col, Q3TableItem * /*item*/)
{
  // ignore item!
  updateContents(row, col);
}

Q3TableItem * KVocTrainTable::item(int row, int col) const
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
    case Qt::Key_Right: {
      int topCell = rowAt(0);
      int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
      if (numCols() > 2)
        for (int i = topCell; i <= lastRowVisible; i++)
          updateCell(i, KV_COL_ORG);
    }  // fallthrough
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
    case Qt::Key_PageUp:
      Q3Table::keyPressEvent(e);
      break;

      case Qt::Key_Left: {
        Q3Table::keyPressEvent(e);
        int topCell = rowAt(0);
        int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
        if (numCols() > 2)
          for (int i = topCell; i <= lastRowVisible; i++)
            updateCell(i, KV_COL_ORG);
      }
      break;

    case Qt::Key_Shift:
    case Qt::Key_Alt:
      case Qt::Key_Control:  // fallthrough
        Q3Table::keyPressEvent(e);
        emit forwardKeyPressEvent(e);
        break;

    case Qt::Key_Minus:
    case Qt::Key_Plus:
    case Qt::Key_Tab:
      case Qt::Key_Backtab:  // fallthrough
        emit forwardKeyPressEvent(e);
        break;

    default:
      Q3Table::keyPressEvent(e);
      break;
  }
}

void KVocTrainTable::keyReleaseEvent(QKeyEvent * e)
{
  delayTimer->stop();
  switch(e->key())
  {
    case Qt::Key_Shift:
    case Qt::Key_Alt:
    case Qt::Key_Control:  // fallthrough
      Q3Table::keyPressEvent(e);
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

  Q3Table::contentsMousePressEvent(e);

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
  if(e->button() == Qt::LeftButton)
    setCurrentCell(cr, cc);
}

void KVocTrainTable::contentsMouseDoubleClickEvent(QMouseEvent * e)
{
  delayTimer->stop();
  Q3Table::contentsMouseDoubleClickEvent(e);
}

void KVocTrainTable::activateNextCell()
{
  int currRow = currentRow();
  int currCol = currentColumn();

  Q3TableSelection currentSel = selection(0);

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
