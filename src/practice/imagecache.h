/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
