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


#include "practiceview.h"
#include "activearea.h"

#include <QResizeEvent>

PracticeView::PracticeView(QGraphicsScene * scene)
{
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_area = 0;
}

void PracticeView::resizeEvent(QResizeEvent* event) {
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    event->accept();
}

void PracticeView::setActiveArea(ActiveArea * area)
{
    m_area = area;
    if (m_area->valid())
        scene()->addItem(m_area);
}

ActiveArea * PracticeView::activeArea()
{
    return m_area;
}