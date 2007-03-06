//
// C++ Interface: kvtlessonview
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef KVTLESSONVIEW_H
#define KVTLESSONVIEW_H


#include <QTreeView>

class KVTLessonView : public QTreeView
{
Q_OBJECT
public:
  KVTLessonView(QWidget *parent = 0);
  void setModel(KVTLessonModel *model);
  void initializeSelection();
public slots:
  void slotCreateNewLesson();
private:
  KVTLessonModel *m_model;
};

#endif
