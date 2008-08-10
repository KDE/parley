//
// C++ Implementation: multipletextualinput
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


#include "multipletextualinput.h"


#include "../activearea.h"
#include "../practiceentry.h"


#include <QObject>
#include <QStringList>
#include <QString>
#include <KSvgRenderer>
#include <QMap>
#include <QList>
#include <QGraphicsScene>
#include <QLineEdit>
#include <KRandom>
MultipleTextualInput::MultipleTextualInput(KSvgRenderer * renderer, ActiveArea * area, QStringList& elementIds, QObject * parent)
    : QObject(parent), m_renderer(renderer),m_elementIds(elementIds)
{
    foreach(QString id, elementIds)
    {
        QString tId = area->translateElementId(id);
        if (tId.isEmpty()) continue; // we just move on if it doesn't exist.

        QLineEdit * t = new QLineEdit();
        m_kids.append(t);
        m_map[id] = t;
        area->scene()->addWidget(t);
        QRectF bounds = m_renderer->boundsOnElement(tId);
        bounds.translate(area->offset());
        t->setGeometry(bounds.toRect());
    }

}

MultipleTextualInput::~MultipleTextualInput()
{
    qDeleteAll(m_kids);
}


void MultipleTextualInput::slotSetText(const QStringList& texts)
{
    kDebug() << texts;
    if (texts.size() < 4)
    {
        kDebug() << "input data <" << texts << "> is of length" << texts.size() << "; we need length 3";
        return; // bad news
    }

    slotClear();

    // in this mode we only set one; the provide the other 2
    int r = KRandom::random() % 3;
    kDebug() << r;
    m_map[m_elementIds[r]]->setText(texts[r]);


}

void MultipleTextualInput::slotClear()
{
    foreach(QLineEdit* k, m_kids)
    {
        k->setText("");
    }
}

void MultipleTextualInput::slotEmitAnswer()
{
    QStringList qsl;
    for(int i = 0; i <= 4; ++i)
    {
        qsl << m_map[m_elementIds[i]]->text();
    }
    kDebug() << qsl;
    emit signalAnswer(qsl);
}