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

#ifndef LESSONSTATISTICSVIEW_H
#define LESSONSTATISTICSVIEW_H

#include "vocabulary/containerview.h"

namespace Editor {
    class ContainerModel;
}

/**
 * View for the lesson list.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class LessonStatisticsView : public Editor::ContainerView
{
    Q_OBJECT

public:
    LessonStatisticsView(QWidget *parent);
    void setModel(Editor::ContainerModel *model);

private Q_SLOTS:
    void removeGrades();
};

#endif
