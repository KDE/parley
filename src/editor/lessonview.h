/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    explicit LessonView(EditorWindow *parent);

    /** Set the model for the view.
     * @param model the model */
    virtual void setModel(LessonModel *model);

Q_SIGNALS:
    /** A lesson was selected */
    void selectedLessonChanged(KEduVocLesson *lesson);

    /** Emitted when a new container is selected. Indicates the selected container. */
    void signalShowContainer(KEduVocContainer *selected);

    /** Emitted when any of the checkboxes for the query change. */
    void lessonsInPracticeChanged();

public Q_SLOTS:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New unit" into something meaningful.*/
    void slotCreateNewLesson();

    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLesson();

    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();

    /**
     * Moves the selected vocabs to a new lession
     */
    void moveToNewLesson();

    /** Remove the grades for the selected lesson.  */
    void slotRemoveGradesLesson();

    /** Remove the grades for the selected lesson and its children.  */
    void slotRemoveGradesLessonChildren();

    /**
     * get notified of new columns only to hide them (grades not shown by default).
     */
    void columnsInserted();

    void setTranslation(KEduVocExpression *entry, int translationId);

    /** Expand all the lessons and its corresponding sub-lessons **/
    void expandAllLesson();

    /** Collapse all the lessons and its corresponding sub-lessons **/
    void collapseAllLesson();

protected Q_SLOTS:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private:
    void setModel(ContainerModel *model) override
    {
        Q_UNUSED(model)
    }
    void setModel(QAbstractItemModel *model) override
    {
        Q_UNUSED(model)
    }

    LessonModel *m_model{nullptr};
    EditorWindow *m_editorWindow{nullptr};
};
}

#endif
