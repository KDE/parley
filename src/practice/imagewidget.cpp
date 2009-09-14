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
#include <kdebug.h>
#include <QtGui/QPainter>
#include <QtCore/QTimer>

using namespace Practice;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), m_scaling(false)
{
    m_scaleTimer = new QTimer(this);
    m_scaleTimer->setSingleShot(true);
    m_scaleTimer->setInterval(500);
    m_scaledPixmapOutOfDate = false;
    connect(m_scaleTimer, SIGNAL(timeout()), this, SLOT(scalePixmap()));
}

void ImageWidget::setPixmap(const QPixmap& pixmap)
{
    m_originalPixmap = pixmap;
    m_scaledPixmap = QPixmap();
    m_scaledBackupPixmap = QPixmap();
    m_scaledPixmapOutOfDate = true;
    update();
}

void ImageWidget::setScalingEnabled(bool scaling)
{
    m_scaling = scaling;
}

void ImageWidget::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    if (m_scaling && m_scaledPixmapOutOfDate) {
        m_scaleTimer->start();
        scalePixmap(false);
    }
    if (m_scaling) {
        int x = (size().width() - m_scaledPixmap.width()) / 2;
        int y = (size().height() - m_scaledPixmap.height()) / 2;
        painter.drawPixmap(x, y, m_scaledPixmap);
    } else {
        painter.drawPixmap(0, 0, m_originalPixmap);
    }
}

void ImageWidget::resizeEvent(QResizeEvent* e)
{
    if (!m_scaledPixmapOutOfDate) {
        m_scaledBackupPixmap = m_scaledPixmap;
    }
    m_scaledPixmapOutOfDate = true;
    QWidget::resizeEvent(e);
}

void ImageWidget::scalePixmap(bool smooth)
{
    if (smooth) {
        if (m_originalPixmap.isNull() || size().isEmpty()) {
            m_scaledPixmap = QPixmap();
            update();
            return;
        }
        m_scaledPixmap = m_originalPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_scaledBackupPixmap = QPixmap();
        m_scaledPixmapOutOfDate = false;
        update();
    } else {
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

#include "imagewidget.moc"
