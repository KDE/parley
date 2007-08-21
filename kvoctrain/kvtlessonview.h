/***************************************************************************

                             kvtlessonview

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef KVTLESSONVIEW_H
#define KVTLESSONVIEW_H

#include <QTreeView>

class KVTLessonModel;

/**
 * View for the lesson list.
 * Actually a QTreeView because QListView cannot have checkboxes.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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
    // /** Append an action to the right click menu.
    // * @param appendAction */
    //void appendAction(QAction *appendAction);
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
    /**
     * Slot from model/view - a new item has been selected.
     * We only extract the newly selected lesson and call slotSelectLesson.
     * @param selected is the new selection
     * @param deselected was selected before
     */
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    /** Add all lessons to query. Activated from menu, passes to m_model. */
    void slotCheckAllLessons();
    /** Remove all lessons from query. Activated from menu, passes to m_model. */
    void slotCheckNoLessons();
    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();
signals:
    /** Emitted when a new lesson is selected. Indicates the selected lesson. */
    void signalCurrentLessonChanged(int currentIndex);
    /** Emitted when any of the checkboxes for the query change. */
    void lessonsInQueryChanged();
private:
    /**
     * Get the name of the current selection.
     * @returns index of the selection. */
    int indexOfCurrentLesson();
    void dropEvent(QDropEvent * event);
    /** The KVTLessonModel for this view. */
    KVTLessonModel *m_model;
    /** The context menu for the lesson list. */
    QMenu *m_lessonMenu;
};

#endif
