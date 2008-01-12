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

/**
 * View for the lesson list.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class LessonView : public ContainerView
{
    Q_OBJECT

public:
    LessonView(QWidget *parent = 0);


public slots:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewLesson();
    /** Create an editor to let the user type a new name for the lesson.*/
    void slotRenameLesson();
    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLesson();

    /** Creates many small lessons with the contents of the original lesson.
     * @todo Let the user select if entries are taken by random or order.
     */
    void slotSplitLesson();
};

#endif
