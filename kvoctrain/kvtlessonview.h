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

/** View for the lesson list.
 * Actually a QTreeView because QListView cannot have checkboxes. Also includes the right click menu for the lessons.
 * @author Frederik Gladhorn <frederik.gladhorn@gmx.de>
 * @since 4.0
 */
class KVTLessonView : public QTreeView
{
Q_OBJECT
public:
  /** Set up the lesson list widget.
   * Also creates the menu to manipulate the lessons.
   * @param parent parent widget */
  KVTLessonView(QWidget *parent = 0);
  /** Set the model for the view.
   * @param model the model */
  void setModel(KVTLessonModel *model);
  /** Select the current lesson from the document.*/
  void initializeSelection();
  /** Get the QMenu to add it somewhere else.
   * @returns the right click menu */
  QMenu *lessonMenu();
public slots:
  /** The model was reset, we need to update the selection using initializeSelection. */
  void slotModelReset();
  /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
  void slotCreateNewLesson();
  /** Create an editor to let the user type a new name for the lesson.*/
  void slotRenameLesson();
  /** Remove a lesson. Ask if it's not empty. */
  void slotDeleteLesson();
  /** Set the selection to a lesson.
   * @param lesson the index of the new selection. */
  void slotSelectLesson(int lesson);
  /** Display the context menu.
  * @param ev the event source*/
  void contextMenuEvent(QContextMenuEvent * ev);
  
  void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
  void slotCheckAllLessons ();
  void slotCheckNoLessons ();
  void slotSplitLesson();
signals:
  void signalCurrentLessonChanged(int currentIndex);
  void lessonsInQueryChanged();
private:
  /** Get the name of the current selection.
  @returns index of the selection. */
  int indexOfCurrentLesson();
  void dropEvent ( QDropEvent * event);
  KVTLessonModel *m_model;
  QMenu *m_lessonMenu;
};

#endif
