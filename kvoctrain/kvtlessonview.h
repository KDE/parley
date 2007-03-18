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

class KVTLessonView : public QTreeView
{
Q_OBJECT
public:
  KVTLessonView(QWidget *parent = 0);
  void setModel(KVTLessonModel *model);
  void initializeSelection();
public slots:
  void slotModelReset();
  void slotCreateNewLesson();
  void slotRenameLesson();
  void slotDeleteLesson();
  void slotSelectLesson(int currentIndex);
  void contextMenuEvent(QContextMenuEvent * ev);
  void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
  void slotCheckAllLessons ();
  void slotCheckNoLessons ();
signals:
  void signalCurrentLessonChanged(int currentIndex);
  void lessonsInQueryChanged();
private:
  KVTLessonModel *m_model;
  QMenu *m_lessonPopupMenu;
};

#endif
