/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "imagewidget.h"

#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtCore/QTimeLine>

#include <kdebug.h>

using namespace Practice;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), m_scaling(true), m_onlyDownscaling(true)
{
    m_scaleTimer = new QTimer(this);
    m_scaleTimer->setSingleShot(true);
    m_scaleTimer->setInterval(500);

    m_animation = new QTimeLine(300, this);

    m_scaledPixmapOutOfDate = false;
    connect(m_scaleTimer, SIGNAL(timeout()), this, SLOT(scalePixmap()));
    connect(m_animation, SIGNAL(valueChanged(qreal)), this, SLOT(update()));
    connect(m_animation, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void ImageWidget::setPixmap(const QPixmap& pixmap)
{
    kDebug() << "set new pixmap, size:" << pixmap.size();
    if (m_animation->state() == QTimeLine::Running) {
        m_animation->stop();
        animationFinished();
    }
    m_animationPixmap = m_scaledPixmap;
    m_originalPixmap = pixmap;
    m_scaledPixmap = QPixmap();
    m_scaledBackupPixmap = QPixmap();
    m_scaledPixmapOutOfDate = true;
    if (!m_scaling) {
        m_scaledPixmap = pixmap;
    }
    scalePixmap(true);
    m_animation->start();
}

void ImageWidget::setScalingEnabled(bool scaling, bool onlyDownscaling)
{
    m_scaling = scaling;
    m_onlyDownscaling = onlyDownscaling;
}

void ImageWidget::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    if (m_scaling && m_scaledPixmapOutOfDate) {
        m_scaleTimer->start();
        scalePixmap(false);
    }
    if (m_animation->state() == QTimeLine::Running) {
        painter.setOpacity(1-m_animation->currentValue());
        int x = (size().width() - m_animationPixmap.width()) / 2;
        int y = (size().height() - m_animationPixmap.height()) / 2;
        painter.drawPixmap(x, y, m_animationPixmap);
        painter.setOpacity(m_animation->currentValue());
    }

    int x = (size().width() - m_scaledPixmap.width()) / 2;
    int y = (size().height() - m_scaledPixmap.height()) / 2;
    painter.drawPixmap(x, y, m_scaledPixmap);
}

void ImageWidget::resizeEvent(QResizeEvent* e)
{
    if (!m_scaledPixmapOutOfDate) {
        m_scaledBackupPixmap = m_scaledPixmap;
    }
    // stop animations when resizing
    if (m_animation->state() == QTimeLine::Running) {
        m_animation->stop();
        animationFinished();
    }
    m_scaledPixmapOutOfDate = true;
    QWidget::resizeEvent(e);
    emit sizeChanged();
}

void ImageWidget::scalePixmap(bool smooth)
{
    bool scaleUp = m_originalPixmap.width() <= size().width() && m_originalPixmap.height() <= size().height();
    if ((m_onlyDownscaling && scaleUp) || m_originalPixmap.size() == size()) {
        kDebug() << "no need to scale pixmap";
        m_scaledPixmapOutOfDate = false;
        m_scaledPixmap = m_originalPixmap;
        m_scaledBackupPixmap = QPixmap();
    } else if (smooth) {
        kDebug() << "smooth scaling to" << size();
        if (m_originalPixmap.isNull() || size().isEmpty()) {
            m_scaledPixmapOutOfDate = false;
            m_scaledPixmap = QPixmap();
            update();
            return;
        }
        m_scaledPixmap = m_originalPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_scaledBackupPixmap = QPixmap();
        m_scaledPixmapOutOfDate = false;
        update();
    } else {
        kDebug() << "fast scaling to" << size();
        // try to find out if it makes sense to use the scaled backup pixmap
        // if the scaled backup gets too small, we use the orignal image
        float ratio = 0;
        if (!size().isEmpty()) {
            ratio = qMin(float(m_scaledBackupPixmap.width())/size().width(),
                        float(m_scaledBackupPixmap.height())/size().height());
        }
        if (ratio > 0.4 && !m_scaledBackupPixmap.isNull()) {
            m_scaledPixmap = m_scaledBackupPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::FastTransformation);
        } else {
            if (m_originalPixmap.isNull() || size().isEmpty()) {
                m_scaledPixmap = QPixmap();
                return;
            }
            // use the original pixmap
            m_scaledPixmap = m_originalPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::FastTransformation);
            m_scaledBackupPixmap = m_scaledPixmap;
        }
        m_scaledPixmapOutOfDate = true;
    }
}

void ImageWidget::animationFinished()
{
    m_animationPixmap = QPixmap();
}

#include "imagewidget.moc"
