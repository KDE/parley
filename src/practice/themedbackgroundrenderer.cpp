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
#include <kstandarddirs.h>
#include <unistd.h>

#include <QtConcurrentRun>
#include <QPainter>
#include <QMargins>

using namespace Practice;

ThemedBackgroundRenderer::ThemedBackgroundRenderer(QObject* parent)
    : QObject(parent), m_queuedRequest(false)
{
    m_cache.setSaveFilename(KStandardDirs::locateLocal("appdata", "practicethemecache.bin"));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(renderingFinished()));
}

ThemedBackgroundRenderer::~ThemedBackgroundRenderer()
{
    m_cache.saveCache();
}

void ThemedBackgroundRenderer::setSvgFilename(const QString& filename)
{
    m_renderer.load(filename);  //TODO: error handling
    m_cache.setFilename(filename);
    kDebug() << m_cache;
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
    if (m_size.isEmpty()) {
        qWarning() << "trying to render with an invalid size";
    }
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

QMargins ThemedBackgroundRenderer::contentMargins()
{
    QMargins margins;
    if (m_renderer.elementExists("background-border-top"))
        margins.setTop(m_renderer.boundsOnElement("background-border-top").toAlignedRect().height());
    if (m_renderer.elementExists("background-border-bottom"))
        margins.setBottom(m_renderer.boundsOnElement("background-border-bottom").toAlignedRect().height());
    if (m_renderer.elementExists("background-border-left"))
        margins.setLeft(m_renderer.boundsOnElement("background-border-left").toAlignedRect().width());
    if (m_renderer.elementExists("background-border-right"))
        margins.setRight(m_renderer.boundsOnElement("background-border-right").toAlignedRect().width());
    return margins;
}

QImage ThemedBackgroundRenderer::renderBackground()
{
    QImage image(m_size, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    QMargins margins = contentMargins();
    QRect backgroundRect(QPoint(margins.left(),margins.top()), m_size-QSize(margins.right()+margins.left(), margins.bottom()+margins.top()));

    renderRect("background", backgroundRect, &p);
    QPair<QString, QRect> rect;
    Q_FOREACH(rect, m_rects) {
        renderRect(rect.first, rect.second, &p);
    }

    kDebug() << "image renderd, cache:" << m_cache;
    return image;
}

void ThemedBackgroundRenderer::renderRect(const QString& name, const QRect& rect, QPainter *p)
{
    renderItem(name+"-center", rect, p, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name+"-center-ratio", rect, p, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name+"-center-crop", rect, p, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);

    renderItem(name+"-border-topleft", rect, p, NoScale, Qt::IgnoreAspectRatio, Top, Corner, false);
    renderItem(name+"-border-topright", rect, p, NoScale, Qt::IgnoreAspectRatio, Right, Corner, false);
    renderItem(name+"-border-bottomleft", rect, p, NoScale, Qt::IgnoreAspectRatio, Left, Corner, false);
    renderItem(name+"-border-bottomright", rect, p, NoScale, Qt::IgnoreAspectRatio, Bottom, Corner, false);

    QStringList edges;
    edges << "top" << "bottom" << "left" << "right";
    Q_FOREACH(const QString& edge, edges) {
        ScaleBase scaleBase;
        Edge alignEdge;
        if(edge == QLatin1String("top")) {
            alignEdge = Top;
            scaleBase = Horizontal;
        } else if(edge == QLatin1String("bottom")) {
            alignEdge = Bottom;
            scaleBase = Horizontal;
        } else if(edge == QLatin1String("right")) {
            alignEdge = Right;
            scaleBase = Vertical;
        } else {
            alignEdge = Left;
            scaleBase = Vertical;
        }
        for(int inside = 1; inside>=0; inside--) {
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge,            rect, p, scaleBase, Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-ratio",   rect, p, scaleBase, Qt::KeepAspectRatio,   alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-noscale", rect, p, NoScale,   Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"top":"left"),     rect, p, NoScale,   Qt::IgnoreAspectRatio, alignEdge, LeftTop, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"bottom":"right"), rect, p, NoScale,   Qt::IgnoreAspectRatio, alignEdge, RightBottom, inside);
        }
    }
}

void ThemedBackgroundRenderer::renderItem(const QString& id, const QRect& rect, QPainter *p, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio, Edge edge, Align align, bool inside)
{
    if (!m_renderer.elementExists(id))
        return;
    QRect itemRect = m_renderer.boundsOnElement(id).toRect();
    if (itemRect.isNull() || rect.isNull())
        return;

    kDebug() << "draw item" << id;
//    kDebug() << "original item rect:" << itemRect << m_renderer.boundsOnElement(id);
    itemRect = scaleRect(itemRect, rect, scaleBase, aspectRatio);
//    kDebug() << "scaled" << itemRect;
    itemRect = alignRect(itemRect, rect, edge, align, inside);
//    kDebug() << "aligned" << itemRect;
    if (aspectRatio == Qt::KeepAspectRatioByExpanding) {
        //TODO: clip painter
    }
    QImage image;
    if (m_cache.imageSize(id) == itemRect.size()) {
        kDebug() << "found in cache:" << id;
        image = m_cache.getImage(id);
    } else {
        kDebug() << "not in cache, render svg:" << id;
        image = QImage(itemRect.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(Qt::transparent).rgba());
        QPainter painter(&image);
        m_renderer.render(&painter, id, QRect(QPoint(0, 0), itemRect.size()));
        m_cache.updateImage(id, image);
    }
    p->drawImage(itemRect.topLeft(), image);
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
        } else if(!inside) {
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
        } else if (!inside) {
            x += itemRect.width();
        }
        itemRect.moveTo(x, y);
        return itemRect;
    }
    kDebug() << "unhandled alignment option";
    return itemRect;
}

#include "themedbackgroundrenderer.moc"
