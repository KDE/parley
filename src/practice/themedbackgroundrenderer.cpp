/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "themedbackgroundrenderer.h"

#include <kdebug.h>
#include <unistd.h>

#include <QtConcurrentRun>
#include <QPainter>

using namespace Practice;

ThemedBackgroundRenderer::ThemedBackgroundRenderer(QObject* parent)
    : QObject(parent), m_queuedRequest(false)
{
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(renderingFinished()));
}

void ThemedBackgroundRenderer::setSvgFilename(const QString& filename)
{
    m_renderer.load(filename);  //TODO: error handling
}

void ThemedBackgroundRenderer::setSize(const QSize& size)
{
    m_size = size;
}

void ThemedBackgroundRenderer::clearRects()
{
    m_rects.clear();
}

void ThemedBackgroundRenderer::addRect(const QString& name, const QRect& rect)
{
    m_rects.append(qMakePair<QString, QRect>(name, rect));
}

void ThemedBackgroundRenderer::updateBackground()
{
    if (m_future.isRunning()) {
        m_queuedRequest = true;
        return;
    }
    m_future = QtConcurrent::run(this, &ThemedBackgroundRenderer::renderBackground);
    m_watcher.setFuture(m_future);
}

void ThemedBackgroundRenderer::renderingFinished()
{
    emit backgroundChanged(QPixmap::fromImage(m_future.result()));
    m_future = QFuture<QImage>();
    if (m_queuedRequest) {
        m_queuedRequest = false;
        updateBackground();
    }
}

QPixmap ThemedBackgroundRenderer::getPixmapForId(const QString& id)
{
    //TODO
    return QPixmap();
}

QImage ThemedBackgroundRenderer::renderBackground()
{
    QImage image(m_size, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    kDebug() << "foobar bounds" << m_renderer.boundsOnElement("foobar") << "rect-bg bounds" << m_renderer.boundsOnElement("rect-bg");

    renderRect("rect", QRect(QPoint(0,0), m_size), &p);
    QPair<QString, QRect> rect;
    Q_FOREACH(rect, m_rects) {
        renderRect("rect", rect.second, &p);
    }

    return image;
}

void ThemedBackgroundRenderer::renderRect(const QString& name, const QRect& rect, QPainter *p)
{
    renderItem(name+"-center", rect, p, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name+"-border-topleft", rect, p, NoScale, Qt::IgnoreAspectRatio, Top, Corner, false);
    renderItem(name+"-border-topright", rect, p, NoScale, Qt::IgnoreAspectRatio, Right, Corner, false);
    renderItem(name+"-border-bottomleft", rect, p, NoScale, Qt::IgnoreAspectRatio, Left, Corner, false);
    renderItem(name+"-border-bottomright", rect, p, NoScale, Qt::IgnoreAspectRatio, Bottom, Corner, false);
    renderItem(name+"-border-top", rect, p, Horizontal, Qt::IgnoreAspectRatio, Top, Centered, false);
    renderItem(name+"-border-bottom", rect, p, Horizontal, Qt::IgnoreAspectRatio, Bottom, Centered, false);
    renderItem(name+"-border-left", rect, p, Vertical, Qt::IgnoreAspectRatio, Left, Centered, false);
    renderItem(name+"-border-right", rect, p, Vertical, Qt::IgnoreAspectRatio, Right, Centered, false);
}

void ThemedBackgroundRenderer::renderItem(const QString& id, const QRect& rect, QPainter *p, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio, Edge edge, Align align, bool inside)
{
    kDebug() << "render item" << id << "based on rect" << rect;
    if (!m_renderer.elementExists(id))
        return;
    QRect itemRect = m_renderer.boundsOnElement(id).toAlignedRect();
    if (itemRect.isNull() || rect.isNull())
        return;

    kDebug() << "original item rect:" << itemRect;
    itemRect = scaleRect(itemRect, rect, scaleBase, aspectRatio);
    kDebug() << "scaled" << itemRect;
    itemRect = alignRect(itemRect, rect, edge, align, inside);
    kDebug() << "aligned" << itemRect;
    if (aspectRatio == Qt::KeepAspectRatioByExpanding) {
        //TODO: clip painter
    }
    m_renderer.render(p, id, itemRect);
    if (aspectRatio == Qt::KeepAspectRatioByExpanding) {
        //TODO: unclip painter
    }
}

QRect ThemedBackgroundRenderer::scaleRect(QRect itemRect, const QRect& baseRect, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio)
{
    qreal verticalFactor = 0;
    qreal horizontalFactor = 0;
    switch (scaleBase) {
    case NoScale:
        return itemRect;
    case Horizontal:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setWidth(baseRect.width());
            return itemRect;
        case Qt::KeepAspectRatio:
            horizontalFactor = qreal(baseRect.width())/itemRect.width();
            itemRect.setWidth(baseRect.width());
            itemRect.setHeight(qRound(itemRect.height()*horizontalFactor));
            return itemRect;
        case Qt::KeepAspectRatioByExpanding:
            kWarning() << "KeepAspectRatioByExpanding only works for the center";
            return itemRect;
        }
        break;
    case Vertical:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setHeight(baseRect.height());
            return itemRect;
        case Qt::KeepAspectRatio:
            verticalFactor = qreal(baseRect.height())/itemRect.height();
            itemRect.setHeight(baseRect.height());
            itemRect.setWidth(qRound(itemRect.width()*verticalFactor));
            return itemRect;
        case Qt::KeepAspectRatioByExpanding:
            kWarning() << "KeepAspectRatioByExpanding only works for the center";
            return itemRect;
        }
        break;
    case Rect:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setWidth(baseRect.width());
            itemRect.setHeight(baseRect.height());
            return itemRect;
        case Qt::KeepAspectRatio:
            horizontalFactor = qreal(baseRect.width())/itemRect.width();
            verticalFactor = qreal(baseRect.height())/itemRect.height();
            if (verticalFactor < horizontalFactor) {
                itemRect.setHeight(baseRect.height());
                itemRect.setWidth(qRound(itemRect.width()*verticalFactor));
            } else {
                itemRect.setWidth(baseRect.width());
                itemRect.setHeight(qRound(itemRect.height()*horizontalFactor));
            }
            return itemRect;
        case Qt::KeepAspectRatioByExpanding:
            horizontalFactor = qreal(baseRect.width())/itemRect.width();
            verticalFactor = qreal(baseRect.height())/itemRect.height();
            if (verticalFactor > horizontalFactor) {
                itemRect.setHeight(baseRect.height());
                itemRect.setWidth(qRound(itemRect.width()*verticalFactor));
            } else {
                itemRect.setWidth(baseRect.width());
                itemRect.setHeight(qRound(itemRect.height()*horizontalFactor));
            }
            return itemRect;
        }
        break;
    }
    kDebug() << "unhandled scaling option";
    return itemRect;
}

QRect ThemedBackgroundRenderer::alignRect(QRect itemRect, const QRect &baseRect, Edge edge, Align align, bool inside)
{
    if (edge == Center) {
        int x = baseRect.x() + (baseRect.width()-itemRect.width())/2;
        int y = baseRect.y() + (baseRect.height()-itemRect.height())/2;
        itemRect.moveTo(x, y);
        return itemRect;
    }

    if (edge == Top || edge == Bottom) {
        // set x coordinate
        int x = 0;
        switch(align) {
        case Corner:
            if (edge == Top) {
                x = baseRect.x() - itemRect.width();
            } else {
                x = baseRect.x() + baseRect.width();
            }
            break;
        case LeftTop:
            x = baseRect.x();
            break;
        case Centered:
            x = baseRect.x() + (baseRect.width()-itemRect.width())/2;
            break;
        case RightBottom:
            x = baseRect.x() + baseRect.width() - itemRect.width();
            break;
        }
        // set y coordinate
        int y = baseRect.y();
        if (edge == Bottom) {
            y += baseRect.height()-itemRect.height();
        }
        if ((!inside) && edge == Top) {
            y -= itemRect.height();
        } else {
            y += itemRect.height();
        }
        itemRect.moveTo(x, y);
        return itemRect;
    } else if (edge == Left || edge == Right) {
        // set y coordinate
        int y = 0;
        switch(align) {
        case Corner:
            if (edge == Right) {
                y = baseRect.y() - itemRect.height();
            } else {
                y = baseRect.y() + baseRect.height();
            }
            break;
        case LeftTop:
            y = baseRect.y();
            break;
        case Centered:
            y = baseRect.y() + (baseRect.height()-itemRect.height())/2;
            break;
        case RightBottom:
            y = baseRect.y() + baseRect.height() - itemRect.height();
            break;
        }
        // set x coordinate
        int x = baseRect.x();
        if (edge == Right) {
            x += baseRect.width()-itemRect.width();
        }
        if ((!inside) && edge == Left) {
            x -= itemRect.width();
        } else {
            x += itemRect.width();
        }
        itemRect.moveTo(x, y);
        return itemRect;
    }
    kDebug() << "unhandled alignment option";
    return itemRect;
}

#include "themedbackgroundrenderer.moc"
