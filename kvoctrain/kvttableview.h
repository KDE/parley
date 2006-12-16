//
// C++ Interface: kvttableview
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTTABLEVIEW_H
#define KVTTABLEVIEW_H

#include <QTableView>

#include <kvttablemodel.h>
#include <kvttabledelegate.h>

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableView : public QTableView
{
Q_OBJECT
public:
  KVTTableView(QWidget *parent = 0);

  void setModel(KVTTableModel * model);

protected slots:
  void verticalHeaderResized(int, int, int);
  void horizontalHeaderResized(int, int, int);
  void slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous);

public slots:
  void slotModelReset();

private:
  KVTTableDelegate * m_delegate;
};

#endif
