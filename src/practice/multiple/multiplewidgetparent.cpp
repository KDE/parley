//
// C++ Implementation: multiplewidgetparent
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "multiplewidgetparent.h"


#include "mwplogic.h"
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

MultipleWidgetParent::MultipleWidgetParent(KSvgRenderer * renderer, ActiveArea * area, QGraphicsScene * scene, QStringList& elementIds, MWPLogic* logic, QObject * parent)
    : QObject(parent), m_renderer(renderer),m_elementIds(elementIds),  m_logic(logic)
{
    foreach(QString id, elementIds)
    {
        QString tId = area->translateElementId(id);
        if (tId.isEmpty()) continue; // we just move on if it doesn't exist.

        QLineEdit * t = new QLineEdit();
        m_kids.append(t);
        m_map[id] = t;
        scene->addWidget(t);
        QRectF bounds = m_renderer->boundsOnElement(tId);
        bounds.translate(area->offset());
        t->setGeometry(bounds.toRect());
    }


    m_logic->setup(m_elementIds, m_kids, m_map);
}

void MultipleWidgetParent::slotSetTexts(const QStringList& texts)
{
    m_logic->setTexts(texts);
}

void MultipleWidgetParent::slotSetEntry(const PracticeEntry* entry)
{
    m_logic->setEntry(entry);
}

void MultipleWidgetParent::slotClear()
{
    m_logic->clear();
}

void MultipleWidgetParent::slotEmitAnswer()
{
    emit signalAnswer(m_logic->answer());
}

MultipleWidgetParent::~MultipleWidgetParent()
{
    qDeleteAll(m_kids);
    delete m_logic;
}

