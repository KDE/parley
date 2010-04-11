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
    m_renderer.render(p, name+"-bg", rect);
}

#include "themedbackgroundrenderer.moc"
