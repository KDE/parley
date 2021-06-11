/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


// Own
#include "gradereferencewidget.h"

// Qt
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPainterPath>

// KDE
#include <KLocalizedString>

// Parley
#include "barwidget.h"  // for gradeColor^WglobalColors


GradeReferenceWidget::GradeReferenceWidget(QWidget *parent)
    : QWidget(parent)
{
}

void GradeReferenceWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const int legendWidth = this->width();
    const int legendHeight = this->height();
    const int legendOffsetY = 0;
    const int legendOffsetX = (this->width() / 2) - (legendWidth / 2);
    const int gradeBarWidth = this->width()/8;
    //const int alphaValueIncrement = 35;
    QRect roundedRect(0 + legendOffsetX, 0 + legendOffsetY, legendWidth, legendHeight);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

    for (int i = 7; i >= 0; --i) {
        QRectF barElement(0 + legendOffsetX + (7 - i) * gradeBarWidth,
			  0 + legendOffsetY,
			  gradeBarWidth, legendHeight);
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
        QColor color = globalColors.longTermColors[i];
        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }
}
