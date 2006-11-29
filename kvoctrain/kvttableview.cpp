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

#include "kvttableview.h"

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
  //setCurrentIndex(model->index(0, 0));
  //scrollTo(currentIndex());
  connect(verticalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(verticalHeaderResized(int, int, int)));
  connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(horizontalHeaderResized(int, int, int)));
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

#include "kvttableview.moc"
