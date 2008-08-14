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
    m_valid = true;
    m_active = false;
    QString id = elementId;
    setSharedRenderer(renderer);
    if (!renderer->elementExists(elementId))
    {
        if (fallbackElementId.isEmpty())
        {
            kDebug() << "Current theme doesn't support mode " << elementId << "and no fallback was provided. Panic!";
            m_valid = false;
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
            m_valid = false;
        }
    }
    setElementId(id);
    m_mode_string = id;


    QString bid = id + "_box";
    if (!m_renderer->elementExists(bid))
        kDebug() << bid << "doesn't exist";

    setZValue(-5);

//     kDebug() << "eq:" << (m_renderer->boundsOnElement(id) == m_renderer->boundsOnElement(bid));
//     kDebug() << m_renderer->boundsOnElement(id) << m_renderer->boundsOnElement(bid) << boundingRect();
//
//     //kDebug() << m_renderer->boundsOnElement("active_area") << "-" << m_renderer->boundsOnElement(bid) << m_renderer->boundsOnElement("active_area").topLeft() - m_renderer->boundsOnElement(id).topLeft();

    QRectF bounds = m_renderer->boundsOnElement("active_area");
     m_original = m_renderer->boundsOnElement(bid);
//
    setPos(bounds.x(), bounds.y());
    scale(bounds.width()/boundingRect().width(), bounds.height()/boundingRect().height());

    m_offset = QPointF(m_renderer->boundsOnElement("active_area").x() - m_renderer->boundsOnElement(bid).x(),
    (m_renderer->boundsOnElement("active_area").y() - m_renderer->boundsOnElement(bid).y()));
    kDebug() << m_offset;
}

QPointF ActiveArea::offset()
{
    return m_offset;
}

QRectF ActiveArea::original()
{
    return m_original;
}


QString ActiveArea::translateElementId(const QString& originalElementId)
{
    // active area specific elements take precedence.
    if (m_renderer->elementExists(m_mode_string + "_" + originalElementId))
        return m_mode_string + "_" + originalElementId;
    else if (m_renderer->elementExists("main_" + originalElementId))
    {
            return "main_" + originalElementId;
    }
    else
    {
        kDebug() << "No translation for elementid " << originalElementId << " found. Area:" << m_mode_string;
        return "";
    }
}

bool ActiveArea::valid()
{
    return m_valid;
}

void ActiveArea::setActive(bool active)
{
    kDebug() << (long int) this << m_mode_string  << active;
    m_active = active;
}

bool ActiveArea::active()
{
    return m_active;
}
