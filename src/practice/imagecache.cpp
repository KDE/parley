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
#include <QDataStream>

#include <kdebug.h>

using namespace Practice;

const char* identifier = "parleyimagecache1";

void ImageCache::setFilename(const QString& filename)
{
    QFileInfo info(filename);
    QDateTime timestamp = info.lastModified();
    if (filename == m_filename && timestamp == m_timestamp) { // cache is up-to-date
        return;
    }
    m_timestamp = timestamp;
    m_images.clear();
    if(m_filename.isEmpty() && !m_saveFilename.isNull()) {
        m_filename = filename;
        openCache();
    } else {
        m_filename = filename;
    }
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
    QFile file(m_saveFilename);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    // check identifier
    QString temp;
    stream >> temp;
    if (temp != QString(identifier)) {
        kDebug() << "not loading cache because the identifier doesn't match";
        return;
    }
    // check filename and timestamp, no need to load images for the wrong file or outdated images
    QDateTime timestamp;
    stream >> temp >> timestamp;
    if (temp != m_filename || timestamp != m_timestamp) {
        kDebug() << "not loading cache because it contains the wrong theme or the timestamp has changed";
        return;
    }
    // finally load data
    stream >> m_images;
    QHashIterator<QString, QImage> i(m_images); //TODO: do on demand
    while (i.hasNext()) {
        i.next();
        m_images[i.key()] = i.value().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    kDebug() << "opened cache:" << m_saveFilename;
    kDebug() << *this;
}

void ImageCache::saveCache()
{
    kDebug() << "save cache to:" << m_saveFilename;
    kDebug() << *this;
    QFile file(m_saveFilename);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream << QString(identifier) << m_filename << m_timestamp << m_images;
}

QDebug Practice::operator<<(QDebug dbg, const ImageCache &c)
{
    dbg.nospace() << "(ImageCache, " << c.m_filename << ", " << c.m_timestamp << ")";
    int pixels = 0;
    QHashIterator<QString, QImage> i(c.m_images);
    while (i.hasNext()) {
        i.next();
        dbg.nospace() << "\n\tcontains: " << qPrintable(i.key().leftJustified(35)) << ": " << i.value().size();
        pixels += i.value().size().width()*i.value().height();
    }

    dbg.nospace() << "\n\ttotal pixel count: " << pixels << " (approx. " << double(pixels)*4/1024/1024 << " MiB)";
    return dbg.space();
 }
