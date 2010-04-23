/***************************************************************************
    Copyright 2009-2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "boxeswidget.h"

#include "themedbackgroundrenderer.h"

#include <keduvoctext.h>
#include <kdebug.h>

#include <QtGui/QPainter>
#include <qbrush.h>

using namespace Practice;

BoxesWidget::BoxesWidget(QWidget* parent)
    : ImageWidget(parent), m_boxCount(1), m_currentBox(-1), m_lastBox(-1), m_renderer(0), m_arrowHint(0), m_spacingHint(0)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setBoxCount(KV_MAX_GRADE);
    setScalingEnabled(false);
}

void BoxesWidget::setRenderer(ThemedBackgroundRenderer *renderer)
{
    m_renderer = renderer;
    m_size = m_renderer->getSizeForId("boxes-noscale").toSize(); //TODO: implement scaling mode
    setMinimumSize(minimumSizeHint());
    updateGeometry();
    updatePixmap();
}

void BoxesWidget::setBoxCount(int boxCount)
{
    m_boxCount = boxCount;
    setMinimumSize(minimumSizeHint());
    updateGeometry();
    updatePixmap();
}

void BoxesWidget::setBoxes(int currentBox, int lastBox)
{
    if(currentBox == m_currentBox && lastBox == m_lastBox)
        return;
    m_currentBox = currentBox;
    m_lastBox = lastBox;
    updatePixmap();
}

QSize BoxesWidget::minimumSizeHint() const
{
    return m_size;
}

void BoxesWidget::updatePixmap()
{
    if(!m_renderer)
        return;
    QImage image(minimumSizeHint(), QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    for(int i = 0; i < m_boxCount; i++) {
        QString id = "box-"+QString::number(i+1);
        if (i+1 == m_currentBox) {
            id += "-active";
        }
        drawElement(&p, id);
    }
    if (m_lastBox != -1 && m_currentBox != -1 && m_lastBox != m_currentBox) {
        drawElement(&p, "arrow-"+QString::number(m_lastBox)+"-"+QString::number(m_currentBox));
    }
    setPixmap(QPixmap::fromImage(image));
}

void BoxesWidget::drawElement(QPainter *p, const QString& id)
{
    QRect container = m_renderer->getRectForId("boxes-noscale").toRect();
    QRect rect = m_renderer->getRectForId(id).toRect();
    QPoint pos = rect.topLeft() - container.topLeft();
    p->drawPixmap(pos, m_renderer->getPixmapForId(id));
}
