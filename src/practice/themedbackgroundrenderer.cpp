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
    : QObject(parent), m_haveCache(true), m_queuedRequest(false), m_isFastScaledRender(true)
{
    m_cache.setSaveFilename(KStandardDirs::locateLocal("appdata", "practicethemecache.bin"));
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateBackgroundTimeout()));
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
    m_haveCache = !m_cache.isEmpty();
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

QPixmap ThemedBackgroundRenderer::getScaledBackground()
{
    if (m_size.isEmpty()) {
        kDebug() << "trying to render with an invalid size";
        return QPixmap();
    }
    if (m_future.isRunning()  || m_future.resultCount()) {
        return QPixmap();
    }
    if (m_cache.isEmpty()) {
        m_timer.start(0);
        return QPixmap();
    }

    QFutureWatcher<QImage> watcher;
    m_future = QtConcurrent::run(this, &ThemedBackgroundRenderer::renderBackground, true);
    watcher.setFuture(m_future);
    watcher.waitForFinished();

    QPixmap result =  QPixmap::fromImage(m_future.result());
    m_future = QFuture<QImage>();
    return result;
}

void ThemedBackgroundRenderer::updateBackground()
{
    if (m_size.isEmpty()) {
        kDebug() << "trying to render with an invalid size";
        return;
    }
    m_timer.start();

}

void ThemedBackgroundRenderer::updateBackgroundTimeout()
{
    bool fastScale = false;
    if (m_future.isRunning()) {
        m_timer.start(); // restart the timer again
        return;
    }
    m_future = QtConcurrent::run(this, &ThemedBackgroundRenderer::renderBackground, fastScale);
    m_watcher.setFuture(m_future);
}

void ThemedBackgroundRenderer::renderingFinished()
{
    if(!m_future.resultCount()) {
        kDebug() << "there is no image!";
        return;
    }
    emit backgroundChanged(QPixmap::fromImage(m_future.result()));
    m_future = QFuture<QImage>();
}

QSizeF ThemedBackgroundRenderer::getSizeForId(const QString& id)
{
    if (!m_renderer.elementExists(id))
        return QSizeF();
    return m_renderer.boundsOnElement(id).size();
}

QPixmap ThemedBackgroundRenderer::getPixmapForId(const QString& id, QSize size)
{
    if (!m_renderer.elementExists(id))
        return QPixmap();
    QRectF itemRect = m_renderer.boundsOnElement(id);
    if (itemRect.isNull())
        return QPixmap();
    if (size.isEmpty())
        size = itemRect.size().toSize();

    if (m_cache.imageSize(id) != size) {
        QImage image(size,  QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(Qt::transparent).rgba());
        QPainter p(&image);
        m_renderer.render(&p, id, QRectF(QPointF(0,0), size));
        m_cache.updateImage(id, image);
        return QPixmap::fromImage(image);
    } else {
        return QPixmap::fromImage(m_cache.getImage(id));
    }
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

QImage ThemedBackgroundRenderer::renderBackground(bool fastScale)
{
    m_isFastScaledRender = false;

    QTime t = QTime::currentTime();
    QImage image(m_size, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    QMargins margins = contentMargins();
    QRect backgroundRect(QPoint(margins.left(),margins.top()), m_size-QSize(margins.right()+margins.left(), margins.bottom()+margins.top()));

    renderRect("background", backgroundRect, &p, fastScale);
    QPair<QString, QRect> rect;
    Q_FOREACH(rect, m_rects) {
        renderRect(rect.first, rect.second, &p, fastScale);
    }

    kDebug() << "image rendered, time:" << t.elapsed();
    return image;
}

void ThemedBackgroundRenderer::renderRect(const QString& name, const QRect& rect, QPainter *p, bool fastScale)
{
    renderItem(name+"-center", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name+"-center-ratio", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name+"-center-crop", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);

    renderItem(name+"-border-topleft", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Top, Corner, false);
    renderItem(name+"-border-topright", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Right, Corner, false);
    renderItem(name+"-border-bottomleft", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Left, Corner, false);
    renderItem(name+"-border-bottomright", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Bottom, Corner, false);

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
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge,            rect, p, fastScale, scaleBase, Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-ratio",   rect, p, fastScale, scaleBase, Qt::KeepAspectRatio,   alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-noscale", rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"top":"left"),     rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, LeftTop, inside);
            renderItem(name+"-"+(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"bottom":"right"), rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, RightBottom, inside);
        }
    }
}

void ThemedBackgroundRenderer::renderItem(const QString& id, const QRect& rect, QPainter *p, bool fastScale, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio, Edge edge, Align align, bool inside)
{
    if (!m_renderer.elementExists(id))
        return;
    QRectF itemRectF = m_renderer.boundsOnElement(id);
    if (itemRectF.isNull() || rect.isNull())
        return;

    kDebug() << "draw item" << id;
//    kDebug() << "original item rect:" << itemRect << m_renderer.boundsOnElement(id);
    QRect itemRect = scaleRect(itemRectF, rect, scaleBase, aspectRatio);
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
    } else if(fastScale && !m_cache.imageSize(id).isEmpty()) {
        kDebug() << "FAST SCALE for:" << id;
        image = m_cache.getImage(id).scaled(itemRect.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        m_isFastScaledRender = true;
    } else {
        kDebug() << "NOT IN CACHE, render svg:" << id;
        image = QImage(itemRect.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(Qt::transparent).rgba());
        QPainter painter(&image);
        m_renderer.render(&painter, id, QRect(QPoint(0, 0), itemRect.size()));
        m_cache.updateImage(id, image);
        m_haveCache = true;
    }
    p->drawImage(itemRect.topLeft(), image);
    if (aspectRatio == Qt::KeepAspectRatioByExpanding) {
        //TODO: unclip painter
    }
}

QRect ThemedBackgroundRenderer::scaleRect(QRectF itemRect, const QRect& baseRect, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio)
{
    qreal verticalFactor = 0;
    qreal horizontalFactor = 0;
    switch (scaleBase) {
    case NoScale:
        return itemRect.toRect();
    case Horizontal:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setWidth(baseRect.width());
            return itemRect.toRect();
        case Qt::KeepAspectRatio:
            horizontalFactor = baseRect.width()/itemRect.width();
            itemRect.setWidth(baseRect.width());
            itemRect.setHeight(itemRect.height()*horizontalFactor);
            return itemRect.toRect();
        case Qt::KeepAspectRatioByExpanding:
            kWarning() << "KeepAspectRatioByExpanding only works for the center";
            return itemRect.toRect();
        }
        break;
    case Vertical:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setHeight(baseRect.height());
            return itemRect.toRect();
        case Qt::KeepAspectRatio:
            verticalFactor = baseRect.height()/itemRect.height();
            itemRect.setHeight(baseRect.height());
            itemRect.setWidth(itemRect.width()*verticalFactor);
            return itemRect.toRect();
        case Qt::KeepAspectRatioByExpanding:
            kWarning() << "KeepAspectRatioByExpanding only works for the center";
            return itemRect.toRect();
        }
        break;
    case Rect:
        switch (aspectRatio) {
        case Qt::IgnoreAspectRatio:
            itemRect.setWidth(baseRect.width());
            itemRect.setHeight(baseRect.height());
            return itemRect.toRect();
        case Qt::KeepAspectRatio:
            horizontalFactor = baseRect.width()/itemRect.width();
            verticalFactor = baseRect.height()/itemRect.height();
            if (verticalFactor < horizontalFactor) {
                itemRect.setHeight(baseRect.height());
                itemRect.setWidth(itemRect.width()*verticalFactor);
            } else {
                itemRect.setWidth(baseRect.width());
                itemRect.setHeight(itemRect.height()*horizontalFactor);
            }
            return itemRect.toRect();
        case Qt::KeepAspectRatioByExpanding:
            horizontalFactor = baseRect.width()/itemRect.width();
            verticalFactor = baseRect.height()/itemRect.height();
            if (verticalFactor > horizontalFactor) {
                itemRect.setHeight(baseRect.height());
                itemRect.setWidth(itemRect.width()*verticalFactor);
            } else {
                itemRect.setWidth(baseRect.width());
                itemRect.setHeight(itemRect.height()*horizontalFactor);
            }
            return itemRect.toRect();
        }
        break;
    }
    kDebug() << "unhandled scaling option";
    return itemRect.toRect();
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
