/*
    SPDX-FileCopyrightText: 2009-2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "boxeswidget.h"

#include "themedbackgroundrenderer.h"

#include <KEduVocText>
#include <QDebug>

#include <QPainter>
#include <QBrush>
#include <QEvent>

using namespace Practice;

BoxesWidget::BoxesWidget(QWidget* parent)
    : ImageWidget(parent), m_boxCount(1), m_currentBox(-1), m_lastBox(-1), m_renderer(0), m_fixedSize(true), m_arrowHint(0), m_spacingHint(0)
{
    setBoxCount(KV_MAX_GRADE);
    setScalingEnabled(false);
    setAlignment(Qt::AlignLeft | Qt::AlignBottom);
}

void BoxesWidget::setRenderer(ThemedBackgroundRenderer *renderer)
{
    m_renderer = renderer;
    m_rect = m_renderer->getRectForId(QStringLiteral("boxes-noscale")).toRect();
    m_fixedSize = true;
    if (!m_rect.isValid()) {
        m_rect = m_renderer->getRectForId(QStringLiteral("boxes")).toRect();
        m_fixedSize = false;
    }
    updatePixmap();
}

void BoxesWidget::setBoxCount(int boxCount)
{
    m_boxCount = boxCount;
    updatePixmap();
}

void BoxesWidget::setBoxes(int currentBox, int lastBox)
{
    if (currentBox == m_currentBox && lastBox == m_lastBox)
        return;
    m_currentBox = currentBox;
    m_lastBox = lastBox;
    updatePixmap();
}

QSize BoxesWidget::minimumSizeHint() const
{
    if (m_fixedSize) {
        return m_rect.size();
    }
    return ImageWidget::minimumSizeHint();
}

void BoxesWidget::updatePixmap()
{
    if (!m_renderer)
        return;

    QSize imageSize = m_rect.size();
    if (!m_fixedSize) {
        imageSize.scale(size(), Qt::KeepAspectRatio);
    }
    QImage image(imageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    for (int i = 0; i < m_boxCount; i++) {
        QString id = "box-" + QString::number(i + 1);
        if (i + 1 == m_currentBox) {
            id += QLatin1String("-active");
        }
        drawElement(&p, id);
    }
    if (m_lastBox != -1 && m_currentBox != -1 && m_lastBox != m_currentBox) {
        drawElement(&p, "arrow-" + QString::number(m_lastBox) + '-' + QString::number(m_currentBox));
    }
    setPixmap(QPixmap::fromImage(image));
}

bool BoxesWidget::event(QEvent *e)
{
    if (e->type() == QEvent::Resize)
        updatePixmap();
    return ImageWidget::event(e);
}

void BoxesWidget::drawElement(QPainter *p, const QString& id)
{
    QRect rect = m_renderer->getRectForId(id).toRect();
    QPoint pos = rect.topLeft() - m_rect.topLeft();
    if (!m_fixedSize) {
        QSize scaledSize = m_rect.size();
        scaledSize.scale(size(), Qt::KeepAspectRatio);
        qreal scale = qreal(scaledSize.width()) / m_rect.width();
        rect.setWidth(rect.width()*scale);
        rect.setHeight(rect.height()*scale);
        pos.setX(pos.x()*scale);
        pos.setY(pos.y()*scale);
    }

    p->drawPixmap(pos, m_renderer->getPixmapForId(id, rect.size()));
}
