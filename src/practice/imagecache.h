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
#include <QStringList>
#include <QDateTime>
#include <QImage>

class QSize;
class QDebug;

namespace Practice
{

class ImageCache
{
public:
    ImageCache() {}
    // set a list of filenames which should be checked for timestamps
    void setFilenames(const QStringList& filename);

    void updateImage(const QString& id, const QImage& image);
    QSize imageSize(const QString& id);
    QImage getImage(const QString& id);
    bool isEmpty() {
        return m_images.isEmpty();
    }

    void setSaveFilename(const QString &filename);
    void openCache();
    void saveCache();

    friend QDebug operator<<(QDebug dbg, const Practice::ImageCache &c);
private:
    QHash<QString, QImage> m_images;
    QList<QDateTime> m_timestamps;
    QStringList m_filenames;
    QString m_saveFilename;
};

QDebug operator<<(QDebug dbg, const Practice::ImageCache &c);

}



#endif // PRACTICE_IMAGECACHE_H
