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

namespace Editor {
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

public slots:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewLesson();

    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLesson();

    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();

    /**
     * get notified of new columns only to hide them (grades not shown by default).
     */
    void columnsInserted();

signals:
    void selectedLessonChanged(KEduVocLesson* lesson);

protected slots:
    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    void currentChanged( const QModelIndex & current, const QModelIndex & previous );

private:
    LessonModel *m_model;
};
}

#endif
