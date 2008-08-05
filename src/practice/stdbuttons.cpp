//
// C++ Implementation: stdbuttons
//
// Description: Implements a set of standard buttons to control the the practice session
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

#include <KDebug>
#include <KLocalizedString>

#include "stdbuttons.h"
#include "statistics/statistics.h"

StdButton::StdButton(const QString& text, KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget* parent)
        : KPushButton(text, parent),
        m_renderer(renderer)
{
    QString tId = area->translateElementId(elementId);

    if (tId.isEmpty()) setVisible(false);

    connect(this, SIGNAL(clicked()), this, SLOT(slotActivated()));

     QRectF bounds = m_renderer->boundsOnElement(tId);
     bounds.translate(area->offset());
     setGeometry(bounds.toRect());

     m_state = ParleyPracticeMainWindow::CheckAnswer;
}


PracticeActionButton::PracticeActionButton(const QString& text, KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget* parent)
        : KPushButton(text, parent),
        m_renderer(renderer)
{
    QString tId = area->translateElementId(elementId);

    if (tId.isEmpty()) setVisible(false);

     QRectF bounds = m_renderer->boundsOnElement(tId);
     bounds.translate(area->offset());
     setGeometry(bounds.toRect());
}


void StdButton::slotActivated()
{
    if (m_state == ParleyPracticeMainWindow::CheckAnswer)
    {
        emit signalCheckAnswer();
    }
    else
    {
        emit signalContinue();
    }
}

void StdButton::slotToggleText(int state)
{
    m_state = state;
    if (state == ParleyPracticeMainWindow::CheckAnswer)
    {
        setText(i18n("Check Answer"));
    }
    else
    {
       setText(i18n("Continue"));
    }
}
