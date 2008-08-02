//
// C++ Interface: activearea
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ACTIVEAREA_H
#define ACTIVEAREA_H

#include <QGraphicsSvgItem>

class QPointF;
class QString;
class KSvgRenderer;

class ActiveArea : public QGraphicsSvgItem
{
    public:
        ActiveArea(KSvgRenderer * renderer, const QString& elementId, const QString& fallbackElementId = "");
        QPointF offset();

    private:
        KSvgRenderer * m_renderer;
        QPointF m_offset;
};

#endif
