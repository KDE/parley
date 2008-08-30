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
    : QObject(parent), m_renderer(renderer),m_elementIds(elementIds), m_area(area)
{
    foreach(const QString &id, elementIds)
    {
        QString tId = area->translateElementId(id);
        if (tId.isEmpty()) continue; // we just move on if it doesn't exist.

        QLineEdit * t = new QLineEdit();
        m_kids.append(t);
        m_map[id] = t;
        area->scene()->addWidget(t);
        QRectF bounds = m_renderer->boundsOnElement(tId);
        bounds.translate(area->offset(tId));
        t->setGeometry(bounds.toRect());
    }

}

MultipleTextualInput::~MultipleTextualInput()
{
    qDeleteAll(m_kids);
}


void MultipleTextualInput::slotSetChoices(const QStringList& texts)
{
    if (!m_area->active()) return;

    if (m_elementIds.size() != texts.size())
    {
        kDebug() << "Size mismatch; we got " << texts.size() << "and need" << m_elementIds.size();
        return;
    }

    if (texts.size() == 0)
    {
        kDebug() << "Text list of size 0 recieved, aborting";
        return;
    }


    slotClear();

    // in this mode we only set one; the provide the other 2
    int r = KRandom::random() % m_elementIds.size();
    kDebug() << r;
    m_map[m_elementIds[r]]->setText(texts[r]);

    // entry zero is the absolute/infintive/basic form, so it makes a good hint.
    // if r happens to equal 0, too bad for the user ;)
    m_hint = texts[0];
}

void MultipleTextualInput::slotClear()
{
    if (!m_area->active()) return;

    foreach(QLineEdit* k, m_kids)
    {
        k->setText("");
    }
}

void MultipleTextualInput::slotShowHint()
{
    if (!m_area->active()) return;

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    m_map[m_elementIds[0]]->setPalette(pal);
    // in this mode hints are the absolute/infinitive/basic form
    m_map[m_elementIds[0]]->setText(m_hint);
}

void MultipleTextualInput::slotEmitAnswer()
{
    if (!m_area->active()) return;

    QStringList qsl;
    for(int i = 0; i < m_elementIds.size(); ++i)
    {
        qsl << m_map[m_elementIds[i]]->text();
    }
    kDebug() << qsl;
    emit signalAnswer(qsl);
}

void MultipleTextualInput::slotShowSolution(const QStringList& texts)
{
    if (!m_area->active()) return;

    for(int i = 0; i < texts.size() && i < m_elementIds.size(); ++i)
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::green);
        m_map[m_elementIds[i]]->setPalette(pal);
        m_map[m_elementIds[i]]->setText(texts[i]);
    }
}
