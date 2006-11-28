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

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableView : public QTableView
{
Q_OBJECT
public:
  KVTTableView(QWidget *parent = 0);

  void setModel(KVTTableModel * model);


};

#endif
