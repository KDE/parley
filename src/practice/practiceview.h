/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_VIEW_H
#define PRACTICE_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class ActiveArea;

class PracticeView
    :public QGraphicsView
{
    public:
        PracticeView(QGraphicsScene*scene, QWidget* parent = 0);
        void resizeEvent(QResizeEvent* event);
        void setActiveArea(ActiveArea * area);
        ActiveArea * activeArea();
    private:
        class ActiveArea * m_area;
};

#endif
