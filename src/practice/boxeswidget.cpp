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
    if(!m_renderer->getSizeForId("box").isEmpty()) {
        m_box = m_renderer->getPixmapForId("box");
    }
    if(!m_renderer->getSizeForId("box-active").isEmpty()) {
        m_activeBox = m_renderer->getPixmapForId("box-active");
    }
    if(!m_renderer->getSizeForId("arrow-begin").isEmpty()) {
        m_arrowBegin = m_renderer->getPixmapForId("arrow-begin");
    }
    if(!m_renderer->getSizeForId("arrow-center").isEmpty()) {
        m_arrowCenter = m_renderer->getPixmapForId("arrow-center");
    }
    if(!m_renderer->getSizeForId("arrow-end").isEmpty()) {
        m_arrowEnd = m_renderer->getPixmapForId("arrow-end");
    }

    m_arrowHint = qRound(m_renderer->getSizeForId("arrow-hint").width());
    m_spacingHint = qRound(m_renderer->getSizeForId("box-spacing-hint").width());
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
    kDebug() << currentBox << lastBox;
    m_currentBox = currentBox;
    m_lastBox = lastBox;
    updatePixmap();
}

QSize BoxesWidget::minimumSizeHint() const
{
    return QSize(m_boxCount*m_box.width() + (m_boxCount-1)*m_spacingHint,
                 m_box.height()+qMax(m_arrowBegin.height(), qMax(m_arrowCenter.height(), m_arrowEnd.height())));
}

void BoxesWidget::updatePixmap()
{
    QImage image(minimumSizeHint(), QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    for(int i = 0; i < m_boxCount; i++) {
        int x = i*(m_box.width() + m_spacingHint);
        int y = image.height() - m_box.height();
        p.drawPixmap(x, y, i+1 == m_currentBox ? m_activeBox : m_box);
    }
    if (m_lastBox != -1 && m_currentBox != -1 && m_lastBox != m_currentBox) {

    }
    this->setPixmap(QPixmap::fromImage(image));
}
