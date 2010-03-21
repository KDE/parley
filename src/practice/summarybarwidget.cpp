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
#include <klocale.h>

#include <QPainter>
#include <QEvent>
#include <QHelpEvent>

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

bool SummaryBarWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        int total = m_correct + m_wrong + m_notAnswered;
        if (!total)
            return QWidget::event(event);

        int correctPos = int(rect().width()*double(m_correct)/total);
        int wrongPos = correctPos+int(rect().width()*double(m_wrong)/total);
        if (helpEvent->x() <= correctPos) {
            setToolTip(i18n("Answered correctly on the first attempt: %1 of %2 (%3 %)", m_correct, total, m_correct*100/total));
        } else if (helpEvent->x() <= wrongPos) {
            setToolTip(i18n("Answered wrong on the first attempt: %1 of %2 (%3 %)", m_wrong, total, m_wrong*100/total));
        } else {
            setToolTip(i18n("Not answered during this practice: %1 of %2 (%3 %)", m_notAnswered, total, m_notAnswered*100/total));
        }
    }
    return QWidget::event(event);
}

#include "summarybarwidget.moc"
