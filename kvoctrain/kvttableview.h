//
// C++ Interface: kvttableview
//
// Description: 
//
//
// Author:  (C) 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTTABLEVIEW_H
#define KVTTABLEVIEW_H

#include <QTableView>

#include <KPrinter>

#include <kvtsortfiltermodel.h>
#include <kvttabledelegate.h>

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableView : public QTableView
{
Q_OBJECT
public:
  KVTTableView(QWidget *parent = 0);

  void setModel(KVTSortFilterModel * model);
  void print(KPrinter *pPrinter);
  void adjustContent();

protected slots:
  void verticalHeaderResized(int, int, int);
  void horizontalHeaderResized(int, int, int);
  void slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous);

public slots:
  void slotModelReset();

protected:
  /** resizes table when frame is resized */
  void resizeEvent(QResizeEvent *);
  void showEvent(QShowEvent *);
  void keyPressEvent(QKeyEvent*);

private:
  void newPage(QPainter &, int, int, int);
  void endOfPage(QPainter &, int, int);

  KVTTableDelegate * m_delegate;
};

#endif
