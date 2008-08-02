//
// C++ Implementation: activearea
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "activearea.h"
#include <QString>
#include <KSvgRenderer>
#include <QRect>
#include <KDebug>

ActiveArea::ActiveArea(KSvgRenderer * renderer, const QString& elementId, const QString& fallbackElementId)
: m_renderer(renderer)
{
    setSharedRenderer(renderer);
    if (!renderer->elementExists(elementId))
    {
        if (fallbackElementId.isEmpty())
        {
            kDebug() << "Current theme doesn't support mode " << elementId << "and no fallback was provided. Panic!";
        }
        else if (renderer->elementExists(fallbackElementId))
        {
            setElementId(fallbackElementId);
            kDebug() << "Current theme doesn't support mode " << elementId << ". Falling back to" << fallbackElementId;
        }
        else
        {
            kDebug() << "Current theme doesn't support modes " << elementId << " or " << fallbackElementId <<
            " (fallback). Panic!";
        }
    }

    setElementId(elementId);
    setZValue(-1);

    QRect bounds = m_renderer->boundsOnElement("active_area").toRect();
    setPos(bounds.x(), bounds.y());
    scale(bounds.width()/boundingRect().width(), bounds.height()/boundingRect().height());
    kDebug() << scenePos() << pos();

    m_offset = QPointF(m_renderer->boundsOnElement("active_area").x() - m_renderer->boundsOnElement(elementId).x(),
    m_renderer->boundsOnElement("active_area").y() - m_renderer->boundsOnElement(elementId).y());
}

QPointF ActiveArea::offset()
{
    return m_offset;
}
