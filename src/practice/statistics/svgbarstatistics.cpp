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
#include "../practiceentrymanager.h"

#include <QSvgRenderer>
#include <QString>
#include <QRectF>
#include <KDebug>

SvgBarStatistics::SvgBarStatistics(QSvgRenderer* renderer, const QString& foregroundElementId, const QString& backgroundElementId, QGraphicsItem * parent)
    : QGraphicsSvgItem(parent)
{
    if (!(renderer->elementExists(foregroundElementId) && renderer->elementExists(backgroundElementId)))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << foregroundElementId << ":" << renderer->elementExists(foregroundElementId);
        kDebug() << "or" << backgroundElementId << ":" << renderer->elementExists(backgroundElementId);
    }

    setSharedRenderer(renderer);
    setElementId(foregroundElementId);
    m_backgroundRect = renderer->boundsOnElement(backgroundElementId);
    setPos(m_backgroundRect.x(), m_backgroundRect.y());
    scale((m_backgroundRect.width())/boundingRect().width()*.0001, 1.0);
    setZValue(10); // higher than the rest
}

SvgBarStatistics::~SvgBarStatistics()
{}

void SvgBarStatistics::slotUpdateDisplay(Statistics*stats)
{
    if (isVisible())
    {
        setVisible(true);
    }
    scale((m_backgroundRect.width() * ((double)stats->correct() / stats->manager()->totalEntryCount()) )/mapToScene(boundingRect()).boundingRect().width(), 1.0);
}



