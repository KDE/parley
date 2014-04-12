/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LESSONVIEW_H
#define LESSONVIEW_H

#include "containerview.h"
#include "lessonmodel.h"

namespace Editor
{
class EditorWindow;

/**
 * View for the lesson list.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class LessonView : public ContainerView
{
    Q_OBJECT

public:
    LessonView(EditorWindow *parent);

    /** Set the model for the view.
     * @param model the model */
    virtual void setModel(LessonModel *model);

signals:
    /** A lesson was selected */
    void selectedLessonChanged(KEduVocLesson* lesson);

    /** Emitted when a new container is selected. Indicates the selected container. */
    void signalShowContainer(KEduVocContainer* selected);

    /** Emitted when any of the checkboxes for the query change. */
    void lessonsInPracticeChanged();

public slots:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewLesson();

    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLesson();

    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();

    /** Remove the grades for the selected lesson.  */
    void slotRemoveGradesLesson();

    /** Remove the grades for the selected lesson and its children.  */
    void slotRemoveGradesLessonChildren();

    /**
     * get notified of new columns only to hide them (grades not shown by default).
     */
    void columnsInserted();

    void setTranslation(KEduVocExpression* entry, int translationId);

    /** Expand all the lessons and its corresponding sub-lessons **/
    void expandAllLesson();

    /** Collapse all the lessons and its corresponding sub-lessons **/
    void collapseAllLesson();

protected slots:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    virtual void setModel(ContainerModel *model) {
        Q_UNUSED(model)
    }
    virtual void setModel(QAbstractItemModel *model) {
        Q_UNUSED(model)
    }

    LessonModel *m_model;
};
}

#endif
