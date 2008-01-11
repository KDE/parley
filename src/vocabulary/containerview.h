/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include <QTreeView>

class ContainerModel;
class KEduVocContainer;
class KEduVocLesson;
class KEduVocWordType;
class KEduVocExpression;


/**
 * View for containers (KEduVocContainer).
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class ContainerView : public QTreeView
{
    Q_OBJECT
public:
    /** Set up the lesson list widget.
     * Also creates the menu to manipulate the lessons.
     * @param parent parent widget */
    ContainerView(QWidget *parent = 0);
    /** Set the model for the view.
     * @param model the model */
    void setModel(ContainerModel *model);


public slots:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewLesson();
    /** Create an editor to let the user type a new name for the lesson.*/
    void slotRenameLesson();
    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLesson();

    void setTranslation(KEduVocExpression*, int);

    void currentChanged( const QModelIndex & current, const QModelIndex & previous );

    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();

signals:
    /** Emitted when a new container is selected. Indicates the selected container. */
    void signalShowContainer(KEduVocContainer* selected);

    /** Emitted when any of the checkboxes for the query change. */
    void lessonsInPracticeChanged();

    void selectedLessonChanged(KEduVocLesson* lesson);

    void selectedWordTypeChanged(KEduVocWordType* wordType);

protected slots:
    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );

private:
    /** The KVTContainerModel for this view. */
    ContainerModel *m_model;
    /** The context menu for the lesson list. */
    QMenu *m_lessonMenu;
};

#endif
