/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "imagecache.h"

#include <QFileInfo>
#include <QDir>
#include <QDataStream>

#include <QDebug>

using namespace Practice;

const char* identifier = "parleyimagecache2";

void ImageCache::setFilenames(const QStringList& filenames)
{
    m_timestamps.clear();
    Q_FOREACH(const QString & filename, filenames) {
        QFileInfo info(filename);
        m_timestamps.append(info.lastModified());
    }
    m_images.clear();
    m_filenames = filenames;
    if (!m_saveFilename.isNull()) {
        openCache();
    }
}

void ImageCache::updateImage(const QString& id, const QImage& image)
{
    m_images[id] = image;
}

QSize ImageCache::imageSize(const QString& id)
{
    if (!m_images.contains(id)) {
        return QSize();
    }
    return m_images.value(id).size();
}

QImage ImageCache::getImage(const QString& id)
{
    if (!m_images.contains(id)) {
        return QImage();
    }
    return m_images.value(id);
}

void ImageCache::setSaveFilename(const QString &filename)
{
    m_saveFilename = filename;
    QDir fileDir = QFileInfo(filename).absoluteDir();
    if (!fileDir.exists() && !fileDir.mkpath(QStringLiteral("."))) {
        qWarning() << QStringLiteral("Couldn't create image cache path: ") << fileDir.absolutePath();
    }
}

void ImageCache::openCache()
{
    QFile file(m_saveFilename);
    if (!file.open(QIODevice::ReadOnly)) {
        // If cache is used for the first time it's normal to fail here,
        // because cache file doesn't exist at this time.
        return;
    }
    QDataStream stream(&file);
    // check identifier
    QString temp;
    stream >> temp;
    if (temp != QString(identifier)) {
        //qDebug() << "not loading cache because the identifier doesn't match";
        return;
    }
    // check filename and timestamp, no need to load images for the wrong file or outdated images
    QStringList filenames;
    QList<QDateTime> timestamps;
    stream >> filenames >> timestamps;
    if (filenames != m_filenames || timestamps != m_timestamps) {
        //qDebug() << "not loading cache because it contains the wrong theme or the timestamp has changed";
        return;
    }
    // finally load data
    stream >> m_images;
    QHashIterator<QString, QImage> i(m_images); //TODO: do on demand
    while (i.hasNext()) {
        i.next();
        m_images[i.key()] = i.value().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    //qDebug() << "opened cache:" << m_saveFilename;
    //qDebug() << *this;
}

void ImageCache::saveCache()
{
    //qDebug() << "save cache to:" << m_saveFilename;
    //qDebug() << *this;
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
        pixels += i.value().size().width() * i.value().height();
    }

    dbg.nospace() << "\n\ttotal pixel count: " << pixels << " (approx. " << double(pixels) * 4 / 1024 / 1024 << " MiB)";
    return dbg.space();
}

