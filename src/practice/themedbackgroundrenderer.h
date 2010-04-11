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

#ifndef PRACTICE_THEMEDBACKGROUNDRENDERER_H
#define PRACTICE_THEMEDBACKGROUNDRENDERER_H

#include <QObject>
#include <QPixmap>

#include <ksvgrenderer.h>

namespace Practice {

class ThemedBackgroundRenderer : public QObject
{
    Q_OBJECT

public:
    ThemedBackgroundRenderer(QObject* parent = 0);
    ~ThemedBackgroundRenderer() {}

    void setSvgFilename(const QString& filename);
    
public Q_SLOTS:
    void setSize(const QSize& size);
    void clearRects();
    void addRect(const QString& name, const QRect& rect);
    void updateBackground();

    QPixmap getPixmapForId(const QString& id);

signals:
    void backgroundChanged(QPixmap pixmap);
   
private:
    KSvgRenderer m_renderer;
    QSize m_size;
    QPixmap m_pixmap;

};

}

#endif // PRACTICE_THEMEDBACKGROUNDRENDERER_H
