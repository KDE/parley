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

#include <KDebug>
#include <KSvgRenderer>
#include <QString>
#include <KLocalizedString>

#include <QGraphicsView>

TextualInput::TextualInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
        : QLineEdit(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());

     connect(this, SIGNAL(textChanged(const QString&)), this, SIGNAL(signalAnswerChanged(const QString&)));
}

void TextualInput::slotEmitAnswer()
{
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
