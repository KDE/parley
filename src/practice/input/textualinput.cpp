//
// C++ Implementation: textualinput
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

#include "textualinput.h"

#include "../practiceentry.h"
#include "prefs.h"
#include "../activearea.h"

#include <KDebug>
#include <KSvgRenderer>
#include <QString>
#include <KLocalizedString>
#include <QRectF>

#include <QGraphicsView>

TextualInput::TextualInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget* parent)
        : QLineEdit(parent),
        m_renderer(renderer), m_area(area)
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);


    QRectF bounds = m_renderer->boundsOnElement(tId);
    bounds.translate(area->offset());
    setGeometry(bounds.toRect());


     connect(this, SIGNAL(textChanged(const QString&)), this, SIGNAL(signalAnswerChanged(const QString&)));
}

void TextualInput::slotEmitAnswer()
{
    if (m_area->active())
        emit signalAnswer(text());
}

void TextualInput::slotShowSolution(const QString& solution)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    setPalette(pal);
    setText(solution);
}

void TextualInput::slotClear()
{
    setText("");
}
