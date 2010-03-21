/***************************************************************************
    Copyright 2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "summarybarwidget.h"

#include <kcolorscheme.h>

#include <QPainter>

using namespace Practice;

SummaryBarWidget::SummaryBarWidget(QWidget *parent)
    : QWidget(parent), m_correct(0), m_wrong(0), m_notAnswered(0)
{
    setMinimumHeight(30);
}

void SummaryBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient linearGrad(QPoint(0, 0), QPoint(rect().width(), 0));

    KColorScheme scheme(QPalette::Active);
    QColor correctColor = scheme.foreground(KColorScheme::PositiveText).color();
    QColor wrongColor = scheme.foreground(KColorScheme::NegativeText).color();
    QColor notAnsweredColor = scheme.foreground(KColorScheme::NormalText).color();

    int total = m_correct + m_wrong + m_notAnswered;
    if (!total) {
        return;
    }
    double correctPos = double(m_correct)/total;
    double wrongPos = correctPos + double(m_wrong)/total;

    double margin = double(2)/rect().width();

    linearGrad.setColorAt(correctPos - margin, correctColor);
    linearGrad.setColorAt(correctPos + margin, wrongColor);

    linearGrad.setColorAt(wrongPos - margin, wrongColor);
    linearGrad.setColorAt(wrongPos + margin, notAnsweredColor);

    QRect r = rect().adjusted(1, 1, -1, -1);

    QPainterPath path;
    path.addRoundedRect( r, 3.0, 3.0 );
    painter.setBrush(QBrush(linearGrad));
    painter.drawPath(path);
}

void SummaryBarWidget::setStatistics(int correct, int wrong, int notAnswered)
{
    m_correct = correct;
    m_wrong = wrong;
    m_notAnswered = notAnswered;
    update();
}

#include "summarybarwidget.moc"
