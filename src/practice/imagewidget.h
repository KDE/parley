/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_IMAGEWIDGET_H
#define PRACTICE_IMAGEWIDGET_H

#include <QWidget>

class QTimeLine;

namespace Practice
{

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);

    void setPixmap(const QPixmap& pixmap);
    void setScalingEnabled(bool scaling, bool onlyDownscaling = true);
    void setKeepAspectRatio(Qt::AspectRatioMode mode);
    void setFadingEnabled(bool fading);
    void setAlignment(Qt::Alignment alignment);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

signals:
    void sizeChanged();

private Q_SLOTS:
    void scalePixmap(bool smooth = true);
    void animationFinished();

private:
    bool m_fading;
    bool m_scaling;
    bool m_onlyDownscaling;
    Qt::AspectRatioMode m_keepAspectRatio;
    Qt::Alignment m_alignment;
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    QPixmap m_scaledBackupPixmap;
    QPixmap m_animationPixmap;
    bool m_scaledPixmapOutOfDate;
    /// Timer used to delay the smooth scaling
    QTimer *m_scaleTimer;
    QTimeLine *m_animation;
};

}

#endif // PRACTICE_IMAGEWIDGET_H
