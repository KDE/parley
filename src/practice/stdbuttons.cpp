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
#include <QPushButton>
#include <QList>

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


void StdButton::slotActivated()
{
    if (text() == "Check Answer")
    {
        emit signalCheckAnswer();
    }
    else if (text() == "Continue")
    {
        emit signalContinue();
    }
    else
    {
        kDebug() << "slotButtonClicked recieved unhandled button " << text();
    }
}

void StdButton::slotToggleText()
{
    if (text() == "Check Answer")
        setText("Continue");
    else
        setText("Check Answer");
}

void StdButton::slotSolutionShown()
{
    if (text() == "Check Answer")
    {
        // showing the solution removes their ability to provide an answer (duh!)
        setText("Continue");
        signalToggleContinueShowAnswerActions();
    }
}
