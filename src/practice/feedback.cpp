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

#include <QRectF>
#include <QString>
#include <KSvgRenderer>
#include <KDebug>

Feedback::Feedback ( KSvgRenderer * renderer, const QString& elementId ) :
        m_renderer ( renderer )
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    m_backgroundRect = renderer->boundsOnElement ( elementId );
};

void Feedback::slotSetText(const QString& htmltext)
{
    setHtml(htmltext);
}

void Feedback::slotClear()
{
    setHtml("");
}
