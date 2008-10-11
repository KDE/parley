//
// C++ Implementation: mixedlettersprompt
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

#include "mixedlettersprompt.h"
#include "../activearea.h"


#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <KRandomSequence>
#include <KRandom>
#include <KDebug>
#include <QGraphicsTextItem>

MixedLettersPrompt::MixedLettersPrompt(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent)
: QWidget(parent), m_renderer(renderer), m_area(area), m_scene(area->scene())
{
    QString tId = area->translateElementId(elementId);
    m_enabled = tId.isEmpty();
    setVisible(m_enabled);

    m_backgroundRect = renderer->boundsOnElement ( tId );

     m_backgroundRect.translate(area->offset(tId));
     setGeometry(m_backgroundRect.toRect());

}

void MixedLettersPrompt::slotSetText (const QString& solution )
{
    m_solution = solution;

    // remove old items
    foreach ( QGraphicsItem* item, m_letters) {
        m_scene->removeItem(item);
        delete item;
    }
    m_letters.clear();

    if (!m_area->active() || !m_enabled)
        return;

    for ( int i = 0; i < solution.length(); i++ )
    {
        QGraphicsTextItem* letter = new QGraphicsTextItem( QString(solution[i]) );
        letter->setPos(randomValidLocation());
        m_letters.append(letter);
        m_scene->addItem(letter);
    }
}

void MixedLettersPrompt::slotAnswerChanged(const QString& answer)
{
    if (!m_area->active() || !m_enabled)
        return;

    int i = 0;
    for (; i < m_solution.length() && i < answer.length(); i++ ) {
        if ( answer[i] == m_solution[i] ) {
            m_letters[i]->setHtml("<b><font color=\"#188C18\">" + m_solution[i] + "</font></b>");
        } else {
            m_letters[i]->setHtml(QString(m_solution[i]));
        }
    }
    // unhighlight all remaining letters.
    for (; i < m_solution.length(); ++i)
    {
        m_letters[i]->setHtml(QString(m_solution[i]));
    }
}

QPointF MixedLettersPrompt::randomValidLocation()
{
    qreal minx = m_backgroundRect.left() + m_backgroundRect.width() / 8.0;
    qreal maxx = m_backgroundRect.right() - m_backgroundRect.width() / 8.0;
    qreal miny = m_backgroundRect.top() + m_backgroundRect.height() / 8.0;
    qreal maxy = m_backgroundRect.bottom() - m_backgroundRect.height() / 8.0;
    KRandomSequence seq;
    qreal x = seq.getLong(maxx-minx) + minx;
    qreal y = seq.getLong(maxy-miny) + miny;

    return QPointF(x,y);
}

