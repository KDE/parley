//
// C++ Implementation: svgbarstatistics
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "svgbarstatistics.h"
#include "statistics.h"
#include "../activearea.h"

#include <QSvgRenderer>
#include <QString>
#include <QRectF>
#include <KDebug>

SvgBarStatistics::SvgBarStatistics(QSvgRenderer* renderer, ActiveArea * area, const QString& foregroundElementId, const QString& backgroundElementId, QGraphicsItem * parent)
    : QGraphicsSvgItem(parent)
{
    QString tId_foreground = area->translateElementId(foregroundElementId);
    QString tId_background = area->translateElementId(backgroundElementId);
    if (tId_foreground.isEmpty() || tId_background.isEmpty()) setVisible(false);

    setSharedRenderer(renderer);
    setElementId(tId_foreground);
    m_backgroundRect = renderer->boundsOnElement(tId_background);
    setPos(m_backgroundRect.x() + area->offset().x(), m_backgroundRect.y() + area->offset().y());
    scale((m_backgroundRect.width())/boundingRect().width()*.0001, 1.0);
    setZValue(10); // higher than the rest
}

SvgBarStatistics::~SvgBarStatistics()
{}

void SvgBarStatistics::slotUpdateDisplay(Statistics*stats)
{
    kDebug() << "update to" << stats->correct() << "/" << stats->manager()->totalEntryCount() << ((double)stats->correct() / stats->manager()->totalEntryCount());
   scale((m_backgroundRect.width() * ((double)stats->correct() / stats->manager()->totalEntryCount()) )/mapToScene(boundingRect()).boundingRect().width(), 1.0);
}



