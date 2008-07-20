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
#include "statistics.h"


StdButton::StdButton(KSvgRenderer * renderer, PracticeView * view, const QString& elementId, QWidget* parent)
        : KPushButton(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    connect(this, SIGNAL(clicked()), this, SLOT(slotActivated()));
       QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());
}

StdButton::StdButton(const QString& text, KSvgRenderer * renderer, PracticeView * view, const QString& elementId, QWidget* parent)
        : KPushButton(text, parent),
        m_renderer(renderer)
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    connect(this, SIGNAL(clicked()), this, SLOT(slotActivated()));
       QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());
}


PracticeActionButton::PracticeActionButton(const QString& text, KSvgRenderer * renderer, const QString& elementId, QWidget* parent)
        : KPushButton(text, parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }
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
