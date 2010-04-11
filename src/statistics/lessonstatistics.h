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
    void showGrades(int languageFrom, int languageTo);

private Q_SLOTS:
    void removeGrades();

private:
    virtual void setModel(QAbstractItemModel *model) { Q_UNUSED(model) }
};

#endif
