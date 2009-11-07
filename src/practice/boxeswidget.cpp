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

#include "boxeswidget.h"

#include <keduvoctext.h>

#include <kdebug.h>
#include <QtGui/QPainter>
#include <qbrush.h>

using namespace Practice;

BoxesWidget::BoxesWidget(QWidget* parent)
    : QWidget(parent), m_boxCount(1), m_currentBox(-1), m_lastBox(-1)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setBoxCount(KV_MAX_GRADE);
}


void BoxesWidget::setBoxCount(int boxCount)
{
    m_boxCount = boxCount;
    updateGeometry();
    update();
}

void BoxesWidget::setBoxes(int currentBox, int lastBox)
{
    kDebug() << "SET BOXES" << currentBox << lastBox;
    m_currentBox = currentBox;
    m_lastBox = lastBox;
    update();
}

QSize BoxesWidget::minimumSizeHint() const
{
    return QSize(m_boxCount*boxWidth + (m_boxCount-1)*boxSpacing+1, boxHeight+arrowHeight+1);
}

void BoxesWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QFont font = painter.font();
    QFont boldFont = font;
    boldFont.setWeight(QFont::Bold);
    if (m_lastBox != -1 && m_currentBox != -1 && m_lastBox != m_currentBox) {
        int arrowStart = (m_lastBox-1) * (boxWidth+boxSpacing) + boxWidth/2;
        int arrowEnd = (m_currentBox-1) * (boxWidth+boxSpacing) + boxWidth/2;
        QPolygon arrowLine;
        arrowLine << QPoint(arrowStart, arrowHeight-arrowSize) << QPoint(arrowStart, 0) << QPoint(arrowEnd, 0) << QPoint(arrowEnd, arrowHeight);
        painter.drawPolyline(arrowLine);
        QPolygon arrowHead;
        arrowHead << QPoint(arrowEnd, arrowHeight) << QPoint(arrowEnd-arrowSize, arrowHeight-arrowSize) << QPoint(arrowEnd+arrowSize, arrowHeight-arrowSize);
        painter.setBrush(Qt::black);
        painter.drawPolygon(arrowHead);
    }
    for(int i = 0; i < m_boxCount; i++) {
        int boxBaseX = i*(boxSpacing+boxWidth);
        int boxBaseY = arrowHeight;
        QPolygon box;
        box << QPoint(boxBaseX, boxBaseY) << QPoint(boxBaseX, boxBaseY+boxHeight) << QPoint(boxBaseX+boxWidth, boxBaseY+boxHeight) << QPoint(boxBaseX+boxWidth, boxBaseY);
        QRect rect(boxBaseX, boxBaseY, boxHeight, boxWidth);
        QLinearGradient gradient(0,boxBaseY,0,boxBaseY+boxHeight);
        if (i+1 == m_currentBox) {
            gradient.setColorAt(0.3, QColor(0,0,0,0));
            gradient.setColorAt(1, QColor(0,0,0,200));
            painter.setFont(boldFont);
        } else {
            gradient.setColorAt(0.3, QColor(0,0,0,0));
            gradient.setColorAt(1, QColor(0,0,0,50));
            painter.setFont(font);
        }
        painter.fillRect(rect, gradient);
        painter.drawPolyline(box);
        painter.drawText(rect, Qt::AlignCenter, QString::number(i+1));
    }
}

