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

#ifndef PRACTICE_IMAGECACHE_H
#define PRACTICE_IMAGECACHE_H

#include <QHash>
#include <QDateTime>
#include <QImage>

class QSize;
class QDebug;

namespace Practice {

class ImageCache {
public:
    ImageCache() {}
    void setFilename(const QString& filename);

    void updateImage(const QString& id, const QImage& image);
    QSize imageSize(const QString& id);
    QImage getImage(const QString& id);
    bool isEmpty() {return m_images.isEmpty();}

    void setSaveFilename(const QString& filename) {m_saveFilename = filename;}
    void openCache();
    void saveCache();

    friend QDebug operator<<(QDebug dbg, const Practice::ImageCache &c);
private:
    QHash<QString, QImage> m_images;
    QDateTime m_timestamp;
    QString m_filename;
    QString m_saveFilename;
};

QDebug operator<<(QDebug dbg, const Practice::ImageCache &c);

}



#endif // PRACTICE_IMAGECACHE_H
