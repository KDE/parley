/***************************************************************************
    Copyright 2014 Andreas Xavier
    Copyright 2014 Inge Wallin
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// Own
#include "barwidget.h"

// Qt
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QPen>

// KDE
#include <klocalizedstring.h>

// Parley
#include "collectionwidget.h"  // for COLLWIDTH, etc


QColor gradeColor[11];



BarWidget::BarWidget(QWidget *parent)
    : QWidget(parent)
{

}

BarWidget::BarWidget(int dueWords[], int totalDueWords, int percentageCompleted, QWidget *parent)
    : QWidget(parent)
{
    QPalette palette(BarWidget::palette());
    palette.setColor(backgroundRole(), Qt::white);
    setPalette(palette);
    for (int i = 0; i < 8; i++) {
        this->dueWords[i] = dueWords[i];
    }
    this->totalDueWords = totalDueWords;
    this->percentageCompleted = percentageCompleted;
}


void BarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const int legendWidth = COLLWIDTH - 10;
    const int legendHeight = 20;
    const int legendOffsetY = 0;
    const int legendOffsetX = 0;
    //const int alphaValueIncrement = 35;

    int gradeBarWidth[9];
    gradeBarWidth[8] = 0;
    int gradeBarOffset[9];
    gradeBarOffset[8] = 0;
    if (percentageCompleted < 100) {
        for(int j = 7; j >= 0; j--) {
            gradeBarWidth[j] = (float)(dueWords[j]) / (float)(totalDueWords) * legendWidth;
            gradeBarOffset[j] = gradeBarOffset[j+1] + gradeBarWidth[j+1];
        }
    }
    else {
        for(int j = 6; j >= 0; j--) {
            gradeBarWidth[j] = 0;
            gradeBarOffset[j] = legendWidth;
        }
        gradeBarWidth[7] = legendWidth;
        gradeBarOffset[7] = 0;
    }
    if (percentageCompleted < 100 && totalDueWords == 0) {
        for(int j = 6; j >= 0; j--) {
            gradeBarWidth[j] = 0;
            gradeBarOffset[j] = legendWidth;
        }
        gradeBarWidth[7] = legendWidth;
        gradeBarOffset[7] = 0;
    }

    QPen penBar(QColor(255,255,255));
    painter.setPen(penBar);
    QRect roundedRect(0, 0, legendWidth, legendHeight);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 8.0, 8.0);

    for (int i = 7; i >= 0; i--) {
        QRectF barElement(0 + legendOffsetX + gradeBarOffset[i], 0 + legendOffsetY, gradeBarWidth[i], legendHeight);
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
        QColor color;
        if (totalDueWords == 0 && percentageCompleted < 100) {
            color = QColor(0, 0, 0, 128);
        }
        else {
            color = gradeColor[i];
        }
        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }
    QPen pen(QColor(255,255,255));
    painter.setPen(pen);
    if (percentageCompleted < 100) {
        painter.drawText(0, 0, legendWidth, 20, Qt::AlignCenter,
			 false  // Disabled until we get the number of words due working
			 ? i18np("%1 word due", "%1 words due", totalDueWords)
			 : "");
    }
    else {
        painter.drawText(0, 0, legendWidth, 20, Qt::AlignCenter, i18n("Fully learned"));
    }
}
