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

#include "imagecache.h"

#include <QFileInfo>

#include <kdebug.h>

using namespace Practice;

void ImageCache::setFilename(const QString& filename)
{
    QFileInfo info(filename);
    QDateTime timestamp = info.lastModified();
    if (filename == m_filename && timestamp == m_timestamp) { // cache is up-to-date
        return;
    }
    m_filename = filename;
    m_timestamp = timestamp;
    m_images.clear();
}

void ImageCache::updateImage(const QString& id, const QImage& image)
{
    m_images[id] = image;
}

QSize ImageCache::imageSize(const QString& id)
{
    if(!m_images.contains(id)) {
        return QSize();
    }
    return m_images.value(id).size();
}

QImage ImageCache::getImage(const QString& id)
{
    if(!m_images.contains(id)) {
        return QImage();
    }
    return m_images.value(id);
}

void ImageCache::openCache()
{
    //TODO
}

void ImageCache::saveCache()
{
    //TODO
}

QDebug Practice::operator<<(QDebug dbg, const ImageCache &c)
{
    dbg.nospace() << "(ImageCache, " << c.m_filename << ", " << c.m_timestamp << ")";
    int pixels = 0;
    QHashIterator<QString, QImage> i(c.m_images);
    while (i.hasNext()) {
        i.next();
        dbg.nospace() << "\n\tcontains:" << i.key().leftJustified(20) << "," << i.value().size();
        pixels += i.value().size().width()*i.value().height();
    }

    dbg.nospace() << "\n\ttotal pixel count: " << pixels << " (approx. " << double(pixels)*4/1024/1024 << " MiB)";
    return dbg.space();
 }
