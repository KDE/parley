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

#include "settings/kgametheme/kgametheme.h"
#include <kdebug.h>
#include <kstandarddirs.h>
#include <unistd.h>

#include <QtConcurrentRun>
#include <QPainter>
#include <QMargins>
#include <QPalette>
#include <QApplication>

using namespace Practice;

ThemedBackgroundRenderer::ThemedBackgroundRenderer(QObject* parent, const QString& cacheFilename)
    : QObject(parent), m_haveCache(true), m_queuedRequest(false), m_isFastScaledRender(true)
{
    m_theme = new KGameTheme();
    m_cache.setSaveFilename(KStandardDirs::locateLocal("appdata", cacheFilename));
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateBackgroundTimeout()));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(renderingFinished()));
}

ThemedBackgroundRenderer::~ThemedBackgroundRenderer()
{
    if (m_future.isRunning()) {
        kDebug() << "Waiting for rendering to finish";
        m_future.waitForFinished();
    }
    m_cache.saveCache();
    delete m_theme;
}

void ThemedBackgroundRenderer::setTheme(const QString &theme)
{
    if(!m_theme->load(theme)) {
        kDebug() << "could not load theme" << theme;
    }
    m_renderer.load(m_theme->graphics());
    m_cache.setFilenames(QStringList(m_theme->graphics()) << m_theme->path());
    m_haveCache = !m_cache.isEmpty();
    m_lastScaledRenderRects.clear();
    m_lastFullRenderRects.clear();
    m_rectMappings.clear();
}

void ThemedBackgroundRenderer::clearRects()
{
    m_rects.clear();
    m_rectMappings.clear();
}

void ThemedBackgroundRenderer::addRect(const QString& name, const QRect& rect)
{
    m_rects.append(qMakePair<QString, QRect>(name, rect));
    if (!m_rectMappings.contains(name)) {
        QString mapped = m_theme->property("X-Parley-"+name);
        m_rectMappings[name] = mapped.isEmpty() ? name : mapped;
    }
}

QPixmap ThemedBackgroundRenderer::getScaledBackground()
{
    if (m_rects.isEmpty() || m_rects[0].second.isEmpty()) {
        return QPixmap();
    }
    if (m_future.isRunning()  || m_future.resultCount()) {
        return QPixmap();
    }
    if (m_cache.isEmpty()) {
        m_timer.start(0);
        return QPixmap();
    }
    if (m_lastScaledRenderRects == m_rects) {
        // we already renderered an image with that exact sizing, no need to waste resources on it again
        return QPixmap();
    }

    QFutureWatcher<QImage> watcher;
    m_future = QtConcurrent::run(this, &ThemedBackgroundRenderer::renderBackground, true);
    watcher.setFuture(m_future);
    watcher.waitForFinished();

    QPixmap result =  QPixmap::fromImage(m_future.result());
    m_future = QFuture<QImage>();
    m_lastScaledRenderRects = m_rects;
    return result;
}

QColor ThemedBackgroundRenderer::fontColor(const QString& context, const QColor& fallback)
{
    QString text = m_theme->property("X-Parley-Font-Color-"+context).toLower();
    if (text.length() == 6 && text.contains(QRegExp("[0-9a-f]{6}"))) {
        return QColor(text.mid(0,2).toInt(0,16),
                      text.mid(2,2).toInt(0,16),
                      text.mid(4,2).toInt(0,16));

    }

    return fallback;
}

void ThemedBackgroundRenderer::updateBackground()
{
    if (m_rects.isEmpty() || m_rects[0].second.isEmpty()) {
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
    if (m_lastFullRenderRects == m_rects && m_lastScaledRenderRects == m_rects) {
        // we already renderered an image with that exact sizing, no need to waste resources on it again
        return;
    }
    m_future = QtConcurrent::run(this, &ThemedBackgroundRenderer::renderBackground, fastScale);
    m_watcher.setFuture(m_future);
    m_lastFullRenderRects = m_rects;
}

void ThemedBackgroundRenderer::renderingFinished()
{
    if(!m_future.resultCount()) {
        //kDebug() << "there is no image!";
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

QRectF ThemedBackgroundRenderer::getRectForId(const QString& id)
{
    if (!m_renderer.elementExists(id))
        return QRectF();
    return m_renderer.boundsOnElement(id);
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
    QString rect;
    if (!m_rects.empty()) {
        rect = m_rects.at(0).first;
    }
    if (m_rectMappings.contains(rect)) {
        rect = m_rectMappings.value(rect);
    }
    QMargins margins;
    if (m_renderer.elementExists(rect+"-border-topleft"))
        margins.setTop(m_renderer.boundsOnElement(rect+"-border-topleft").toAlignedRect().height());
    if (m_renderer.elementExists(rect+"-border-bottomleft"))
        margins.setBottom(m_renderer.boundsOnElement(rect+"-border-bottomleft").toAlignedRect().height());
    if (m_renderer.elementExists(rect+"-border-topleft"))
        margins.setLeft(m_renderer.boundsOnElement(rect+"-border-topleft").toAlignedRect().width());
    if (m_renderer.elementExists(rect+"-border-topright"))
        margins.setRight(m_renderer.boundsOnElement(rect+"-border-topright").toAlignedRect().width());
    return margins;
}

QImage ThemedBackgroundRenderer::renderBackground(bool fastScale)
{
    m_isFastScaledRender = false;

    QTime t = QTime::currentTime();
    QImage image(m_rects[0].second.size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter p(&image);

    QPair<QString, QRect> rect;
    Q_FOREACH(rect, m_rects) {
        if (!m_rects.isEmpty() && rect == m_rects[0]) {
            QMargins margins = contentMargins();
            rect.second = QRect(QPoint(margins.left(),margins.top()), rect.second.size()-QSize(margins.right()+margins.left(), margins.bottom()+margins.top()));
        }
        renderRect(rect.first, rect.second, &p, fastScale);
    }

    //kDebug() << "image rendered, time:" << t.elapsed();
    return image;
}

void ThemedBackgroundRenderer::renderRect(const QString& name, const QRect& rect, QPainter *p, bool fastScale)
{
    renderItem(name, "center", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name, "center-ratio", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);
    renderItem(name, "center-crop", rect, p, fastScale, Rect, Qt::IgnoreAspectRatio, Center, Centered, true);

    renderItem(name, "border-topleft", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Top, Corner, false);
    renderItem(name, "border-topright", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Right, Corner, false);
    renderItem(name, "border-bottomleft", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Left, Corner, false);
    renderItem(name, "border-bottomright", rect, p, fastScale, NoScale, Qt::IgnoreAspectRatio, Bottom, Corner, false);

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
            renderItem(name, QString(inside?"inside":"border")+"-"+edge,            rect, p, fastScale, scaleBase, Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name, QString(inside?"inside":"border")+"-"+edge+"-ratio",   rect, p, fastScale, scaleBase, Qt::KeepAspectRatio,   alignEdge, Centered, inside);
            renderItem(name, QString(inside?"inside":"border")+"-"+edge+"-noscale", rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, Centered, inside);
            renderItem(name, QString(inside?"inside":"border")+"-"+edge+"-repeat",  rect, p, fastScale, scaleBase, Qt::IgnoreAspectRatio, alignEdge, Repeated, inside);
            renderItem(name, QString(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"top":"left"),     rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, LeftTop, inside);
            renderItem(name, QString(inside?"inside":"border")+"-"+edge+"-"+(scaleBase==Vertical?"bottom":"right"), rect, p, fastScale, NoScale,   Qt::IgnoreAspectRatio, alignEdge, RightBottom, inside);
        }
    }
}

void ThemedBackgroundRenderer::renderItem(const QString& idBase, const QString& idSuffix, const QRect& rect, QPainter *p, bool fastScale, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio, Edge edge, Align align, bool inside)
{
    // the id without the mapping, which we need to use for caching
    // (otherwise, images could share a place in the cache which makes it useless if they have different sizes)
    QString id = idBase+'-'+idSuffix;
    // the id according to the mapping specified in the desktop file
    QString mappedId = m_rectMappings.contains(idBase)? m_rectMappings.value(idBase)+'-'+idSuffix : id;

    if (!m_renderer.elementExists(mappedId))
        return;
    QRectF itemRectF = m_renderer.boundsOnElement(mappedId);
    if (itemRectF.isNull() || rect.isNull())
        return;

    //kDebug() << "draw item" << id;
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
       // kDebug() << "found in cache:" << id;
        image = m_cache.getImage(id);
    } else if(fastScale && !m_cache.imageSize(id).isEmpty()) {
       // kDebug() << "FAST SCALE for:" << id;
        image = m_cache.getImage(id).scaled(itemRect.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        m_isFastScaledRender = true;
    } else {
       // kDebug() << "NOT IN CACHE, render svg:" << id;
        image = QImage(itemRect.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(Qt::transparent).rgba());
        QPainter painter(&image);
        if(align == Repeated) {
            QImage tile(itemRectF.toRect().size(), QImage::Format_ARGB32_Premultiplied);
            tile.fill(QColor(Qt::transparent).rgba());
            QPainter tilePainter(&tile);
            m_renderer.render(&tilePainter, mappedId, QRect(QPoint(0, 0), tile.size()));
            painter.fillRect(image.rect(), QBrush(tile));
        } else {
            m_renderer.render(&painter, mappedId, QRect(QPoint(0, 0), itemRect.size()));
        }
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
   // kDebug() << "unhandled scaling option";
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
        case Repeated:
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
        case Repeated:
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
   // kDebug() << "unhandled alignment option";
    return itemRect;
}

#include "themedbackgroundrenderer.moc"
