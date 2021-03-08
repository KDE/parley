/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_THEMEDBACKGROUNDRENDERER_H
#define PRACTICE_THEMEDBACKGROUNDRENDERER_H

#include <QObject>

#include "imagecache.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QTimer>
#include <QSvgRenderer>

class QMargins;
class KGameTheme;

namespace Practice
{

class ThemedBackgroundRenderer : public QObject
{
    Q_OBJECT

public:
    enum ScaleBase {
        NoScale,
        Horizontal,
        Vertical,
        Rect
    };

    enum Edge {
        Top,
        Bottom,
        Left,
        Right,
        Center
    };

    enum Align {
        Corner,
        LeftTop, // left or top (depending on orientation of the edge)
        Centered,
        Repeated,
        RightBottom // right or bottom (depending on orientation of the edge)
    };

    ThemedBackgroundRenderer(QObject* parent, const QString& cacheFilename);
    ~ThemedBackgroundRenderer();

    void setTheme(const QString& theme);

    QSizeF getSizeForId(const QString& id);
    QRectF getRectForId(const QString& id);
    QPixmap getPixmapForId(const QString& id, QSize size = QSize());
    QMargins contentMargins();

    QPixmap getScaledBackground();

    QColor fontColor(const QString& context, const QColor& fallback);

public Q_SLOTS:
    void clearRects();
    void addRect(const QString& name, const QRect& rect);
    void updateBackground();

    void updateBackgroundTimeout();

    void renderingFinished();

signals:
    void backgroundChanged(QPixmap pixmap);

private:
    QImage renderBackground(bool fastScale);
    void renderRect(const QString& name, const QRect& rect, QPainter *p, bool fastScale);
    void renderItem(const QString& idBase, const QString& idSuffix, const QRect& rect, QPainter *p, bool fastScale, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio, Edge edge, Align align, bool inside);
    QRect scaleRect(QRectF itemRect, const QRect& baseRect, ScaleBase scaleBase, Qt::AspectRatioMode aspectRatio);
    QRect alignRect(QRect itemRect, const QRect& baseRect, Edge edge, Align align, bool inside);

    ImageCache m_cache;
    QFuture<QImage> m_future;
    QFutureWatcher<QImage> m_watcher;
    KGameTheme *m_theme;
    QHash<QString, QString> m_rectMappings;
    QSvgRenderer m_renderer;
    QList<QPair<QString, QRect> > m_rects;
    QList<QPair<QString, QRect> > m_lastScaledRenderRects; // the rects used for the last scaled render
    QList<QPair<QString, QRect> > m_lastFullRenderRects; // the rects used for the last full render
    bool m_haveCache;
    bool m_queuedRequest;
    bool m_isFastScaledRender;
    QTimer m_timer;
};

}

#endif // PRACTICE_THEMEDBACKGROUNDRENDERER_H
