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
    explicit ImageWidget(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);
    void setScalingEnabled(bool scaling, bool onlyDownscaling = true);
    void setKeepAspectRatio(Qt::AspectRatioMode mode);
    void setFadingEnabled(bool fading);
    void setAlignment(Qt::Alignment alignment);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

Q_SIGNALS:
    void sizeChanged();

private Q_SLOTS:
    void scalePixmap(bool smooth = true);
    void animationFinished();

private:
    bool m_fading{true};
    bool m_scaling{true};
    bool m_onlyDownscaling{true};
    Qt::AspectRatioMode m_keepAspectRatio{Qt::KeepAspectRatio};
    Qt::Alignment m_alignment{Qt::AlignCenter};
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    QPixmap m_scaledBackupPixmap;
    QPixmap m_animationPixmap;
    bool m_scaledPixmapOutOfDate;
    /// Timer used to delay the smooth scaling
    QTimer *m_scaleTimer{nullptr};
    QTimeLine *m_animation{nullptr};
};

}

#endif // PRACTICE_IMAGEWIDGET_H
