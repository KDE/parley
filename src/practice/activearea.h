//
// C++ Interface: activearea
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


#ifndef ACTIVEAREA_H
#define ACTIVEAREA_H

#include <QGraphicsSvgItem>
#include <QSet>

class QPointF;
class QString;
class KSvgRenderer;

class ActiveArea : public QGraphicsSvgItem
{
    public:
        ActiveArea(KSvgRenderer * renderer, const QString& elementId, const QString& fallbackElementId = "");
        QPointF offset() const { return m_offset; };
        QString translateElementId(const QString& originalElementId);
        QRectF original() const { return m_original; };
        bool valid() const { return m_valid; };
        void setActive(bool active) { m_active = active; };
        bool active() const { return m_active; };
    private:
        KSvgRenderer * m_renderer;
        QPointF m_offset;
        QString m_mode_string; /// The active area name.
        QRectF m_original;
        bool m_valid;
        bool m_active;
};

#endif
