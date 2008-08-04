//
// C++ Implementation: feedback
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

#include "feedback.h"
#include "activearea.h"

#include <QRectF>
#include <QString>
#include <KSvgRenderer>
#include <KDebug>

Feedback::Feedback ( KSvgRenderer * renderer, ActiveArea * area, const QString& elementId ) :
        m_renderer ( renderer )
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);

    m_backgroundRect = renderer->boundsOnElement ( tId );
    setPos (area->offset().x() + m_backgroundRect.x(), area->offset().y() + m_backgroundRect.y() );
    adjustSize();
    setZValue(5);

}

void Feedback::slotSetText(const QString& htmltext)
{
    setHtml(htmltext);
}

void Feedback::slotClear()
{
    setHtml("");
}
