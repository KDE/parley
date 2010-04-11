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

using namespace Practice;

ThemedBackgroundRenderer::ThemedBackgroundRenderer(QObject* parent)
    : QObject(parent)
{

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

}

void ThemedBackgroundRenderer::addRect(const QString& name, const QRect& rect)
{

}

void ThemedBackgroundRenderer::updateBackground()
{
    m_pixmap = QPixmap(m_size);
    m_pixmap.fill(QColor(255,200,200));
    emit backgroundChanged(m_pixmap);
}

QPixmap ThemedBackgroundRenderer::getPixmapForId(const QString& id)
{
    //TODO
    return QPixmap();
}

#include "themedbackgroundrenderer.moc"
