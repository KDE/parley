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

const char* identifier = "parleyimagecache2";

void ImageCache::setFilenames(const QStringList& filenames)
{
    m_timestamps.clear();
    Q_FOREACH(const QString& filename, filenames) {
        QFileInfo info(filename);
        m_timestamps.append(info.lastModified());
    }
    m_images.clear();
    m_filenames = filenames;
    if(!m_saveFilename.isNull()) {
        openCache();
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
    QStringList filenames;
    QList<QDateTime> timestamps;
    stream >> filenames >> timestamps;
    if (filenames != m_filenames || timestamps != m_timestamps) {
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
    stream << QString(identifier) << m_filenames << m_timestamps << m_images;
}

QDebug Practice::operator<<(QDebug dbg, const ImageCache &c)
{
    dbg.nospace() << "(ImageCache, " << c.m_filenames << ", " << c.m_timestamps << ")";
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
