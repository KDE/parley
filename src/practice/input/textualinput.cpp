//
// C++ Implementation: textualinput
//
// Description:
//
//
// Copyright 2008 David Capel <wot.narg@gmail.com>
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
        : KLineEdit(parent),
        m_renderer(renderer), m_area(area), m_size_hint_shown(0)
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);


    QRectF bounds = m_renderer->boundsOnElement(tId);
    bounds.translate(area->offset(tId));
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
    if (!m_area->active()) return;

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    setPalette(pal);
    setText(solution);
}

void TextualInput::slotClear()
{
    if (!m_area->active()) return;

    setText("");
}

void TextualInput::slotShowHint(const QString& hint)
{
    if (!m_area->active()) return;

    QString temp;
    ++m_size_hint_shown;
    for( int i = 0; i < m_size_hint_shown ; ++i)
    {
        temp.append(hint[i]);
    }
    slotShowSolution(temp);
}

