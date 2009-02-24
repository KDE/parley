//
// C++ Implementation: prompt
//
// Description: Implementation of the prompt (or question) displaying class
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

#include "textualprompt.h"
#include "../activearea.h"

#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <KDebug>

TextualPrompt::TextualPrompt ( KSvgRenderer * renderer, ActiveArea * area, const QString& elementId,  QGraphicsItem * parent ) :
        QGraphicsTextItem(parent), m_renderer ( renderer ), m_area(area)
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);

    m_backgroundRect = renderer->boundsOnElement ( tId );
    setPos (area->offset(tId).x() + m_backgroundRect.x(), area->offset(tId).y() + m_backgroundRect.y() + m_backgroundRect.height() / 4.0 );
    adjustSize();
    setZValue(5);

    setTextWidth(m_backgroundRect.width()*.8);
}

void TextualPrompt::slotSetText ( const QString& text )
{
    if (!m_area->active())
    {
        setHtml("");
        return;
    }

	QString escapedText = text;
	escapedText.replace("<", "&lt;").replace(">", "&gt;");
	setHtml("<center>" + escapedText + "</center>");
}

void TextualPrompt::slotClear()
{
    setHtml("");
}

