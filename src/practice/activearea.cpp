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
    QString id = elementId;
    setSharedRenderer(renderer);
    if (!renderer->elementExists(elementId))
    {
        if (fallbackElementId.isEmpty())
        {
            kDebug() << "Current theme doesn't support mode " << elementId << "and no fallback was provided. Panic!";
        }
        else if (renderer->elementExists(fallbackElementId))
        {
            id = fallbackElementId;
            kDebug() << "Current theme doesn't support mode " << elementId << ". Falling back to" << fallbackElementId;
        }
        else
        {
            kDebug() << "Current theme doesn't support modes " << elementId << " or " << fallbackElementId <<
            " (fallback). Panic!";
        }
    }
    setElementId(id);
    m_mode_string = id;

    setZValue(-5);

    QRect bounds = m_renderer->boundsOnElement("active_area").toRect();
    setPos(bounds.x(), bounds.y());
    scale(bounds.width()/boundingRect().width(), bounds.height()/boundingRect().height());
    kDebug() << scenePos() << pos();

    m_offset = QPointF(m_renderer->boundsOnElement("active_area").x() - m_renderer->boundsOnElement(id).x(),
    m_renderer->boundsOnElement("active_area").y() - m_renderer->boundsOnElement(id).y());
    kDebug() << m_offset;
}

QPointF ActiveArea::offset()
{
    return m_offset;
}


QString ActiveArea::translateElementId(const QString& originalElementId)
{
    // active area specific elements take precedence.
    if (m_renderer->elementExists(m_mode_string + "_" + originalElementId))
        return m_mode_string + "_" + originalElementId;
    else if (m_renderer->elementExists("main_" + originalElementId))
        return "main_" + originalElementId;
    else
    {
        kDebug() << "No translation for elementid " << originalElementId << " found. (main_ +" << originalElementId << " and " << m_mode_string << "_" << originalElementId << " don't exist)";
        return "";
    }
}