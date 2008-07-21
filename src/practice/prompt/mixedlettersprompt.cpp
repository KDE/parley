//
// C++ Implementation: mixedlettersprompt
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

#include "mixedlettersprompt.h"

#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <KRandomSequence>
#include <KRandom>
#include <KDebug>
#include <QGraphicsTextItem>

MixedLettersPrompt::MixedLettersPrompt(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent)
: QWidget(parent), m_renderer(renderer), m_scene(view->scene()), m_view(view)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    m_backgroundRect = renderer->boundsOnElement ( elementId );
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


    QList<int> positions;
    for ( int i = 0; i < solution.length(); i++ )
    {
        positions.append((m_backgroundRect.width()-20) * i/solution.length()  + 10 );
    }

    KRandomSequence random;
    for ( int i = 0; i < solution.length(); i++ )
    {
        QGraphicsTextItem* letter = new QGraphicsTextItem( QString(solution[i]) );
        letter->translate(positions.takeAt( random.getLong(positions.size())),
        m_backgroundRect.height()/4 - 5 + random.getLong( m_backgroundRect.height()/2 ) );
        m_letters.append(letter);
        m_scene->addItem(letter);
//         letter->setParentItem(backgroundItem);
        letter->setPos(m_view->mapToScene(letter->x(), letter->y()) + m_backgroundRect.topLeft());
    }
}

void MixedLettersPrompt::slotAnswerChanged(const QString& answer)
{
    for ( int i = 0; i < m_solution.length() && i < answer.length(); i++ ) {
        if ( answer[i] == m_solution[i] ) {
            m_letters[i]->setHtml("<b><font color=\"#188C18\">" + m_solution[i] + "</font></b>");
        } else {
//             if ( i >= answerLineEdit->text().length() ) {
                // no input yet
            m_letters[i]->setHtml(QString(m_solution[i]));
//             } else {
                // wrong
//                 m_answerTextItems[i]->setHtml("<b><font color=\"#FF0000\">" + solution[i] + "</font></b>");
//             }
        }
    }
}

