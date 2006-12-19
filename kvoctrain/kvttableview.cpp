//
// C++ Implementation: kvttableview
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QHeaderView>
#include <QPainter>

#include <KInstance>

#include "kvttableview.h"
#include "prefs.h"

KVTTableView::KVTTableView(QWidget *parent) : QTableView(parent)
{
  setSelectionMode(QAbstractItemView::ContiguousSelection);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
  setTabKeyNavigation(true);
  connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(horizontalHeaderResized(int, int, int)));
  m_delegate = new KVTTableDelegate(this);
  setItemDelegate(m_delegate);
}


/*!
    \fn KVTTableView::setModel(KVTTableModel * model)
 */
void KVTTableView::setModel(KVTTableModel * model)
{
  QTableView::setModel(model);
  setCurrentIndex(model->index(0, 0));
  scrollTo(currentIndex());
  connect(verticalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(verticalHeaderResized(int, int, int)));
  connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(horizontalHeaderResized(int, int, int)));
  connect(selectionModel(), SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentColumnChanged(const QModelIndex&, const QModelIndex&)));
}

void KVTTableView::verticalHeaderResized(int , int , int)
{
  //kDebug() << "Row resized\n";
}

void KVTTableView::horizontalHeaderResized(int logicalIndex, int oldSize, int newSize)
{
  //kDebug() << "Column resized\n";
  model()->setHeaderData(logicalIndex, Qt::Horizontal, QSize(newSize, 25), Qt::SizeHintRole);
}

void KVTTableView::slotModelReset()
{
  for (int i = 0; i < horizontalHeader()->count(); ++i)
    setColumnWidth(i, qvariant_cast<QSize>(model()->headerData(i, Qt::Horizontal, Qt::SizeHintRole)).width());
}

void KVTTableView::slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous)
{
  m_delegate->setCurrentIndex(current);
  reset();
}

void KVTTableView::print(KPrinter * pPrinter)
{
  ///@todo revisit and check before release
  QPainter painter;
  painter.setBrush(Qt::NoBrush);
  int res = pPrinter->resolution();
  int marg = res;

  int pw;
  int c = 0;
  int currentWidth;
  int startCol;
  int pageNum = 1;

  int colCount = model()->columnCount(QModelIndex());
  int hh = horizontalHeader()->height();
  int tPos = marg + hh;

  QColorGroup cg;
  QStyleOptionViewItem option;
  option.initFrom(this);

  painter.begin(pPrinter);
  QRect w = painter.window();
  QRect cr;

  pw = w.width() - (2 * marg);

  while (c < colCount)
  {
    startCol = c;
    currentWidth = marg;

    while ((currentWidth < pw) && (c < colCount))
    {
      currentWidth = currentWidth + columnWidth(c);
      c++;
    }
    if (c < colCount)
      c--;

    newPage(painter, res, startCol, c);

    for (int rc = 0; rc < model()->rowCount(QModelIndex()); ++rc)
    {
      int rh = rowHeight(rc);

      painter.resetXForm();
      painter.setFont(Prefs::tableFont());
      painter.translate(marg, tPos);

      for (int i = startCol; i <= c && i < colCount; ++i)
      {
        cr.setRect(0, 0, columnWidth(i), rh);
        option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        option.rect = cr;
        painter.drawRect(cr);
        painter.save();

        itemDelegate()->paint(&painter, option, model()->index(rc, i));
        painter.restore();

        painter.translate(columnWidth(i), 0);
      }

      tPos = tPos + rowHeight(rc);

      if (tPos + rowHeight(rc + 1) > w.height() - marg)
      {
        endOfPage(painter, pageNum++, res);
        tPos = marg + hh;
        pPrinter->newPage();
        newPage(painter, res, startCol, c);
      }
    }
    endOfPage(painter, pageNum++, res);

    if (c < colCount)
    {
      pPrinter->newPage();
      tPos = marg + hh;
      c++;
    }
  }
  painter.end();
}

void KVTTableView::newPage(QPainter & painter, int res, int startCol, int endCol)
{
  int marg = res;
  int hh = horizontalHeader()->height();
  int cw;
  QRect cr;
  QRect w = painter.window();
  painter.resetXForm();
  painter.setFont(KGlobalSettings::generalFont());
  /// @todo improve i18n("KVocTrain - %1", m_doc->title())
  painter.drawText(marg, marg - 20, KInstance::caption() );
  painter.translate(marg, marg);
  painter.drawLine(0 , 0, 0, hh);
  for (int i = startCol; i <= endCol && i < model()->columnCount(QModelIndex()); ++i)
  {
    cw = columnWidth(i);
    painter.drawLine(0, 0, cw, 0);
    painter.drawLine(cw, 0, cw, hh);
    cr.setRect(3, 0, columnWidth(i)- 6, hh);
    painter.drawText(cr, Qt::AlignLeft | Qt::AlignVCenter, model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    painter.translate(cw, 0);
  }
}

void KVTTableView::endOfPage(QPainter & painter, int pageNum, int res)
{
  painter.resetXForm();
  painter.setFont(KGlobalSettings::generalFont());
  QRect w = painter.window();
  QRect r = painter.boundingRect(0, 0, 0, 0, Qt::AlignLeft, QString::number(pageNum));
  painter.drawText((w.width()/2) - (r.width()/2), w.height() - res + 20, QString::number(pageNum));
}

#include "kvttableview.moc"
