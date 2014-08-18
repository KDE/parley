/*
 *  Copyright 2014  Andreas Xavier <andxav at zoho dot com>
 *  Copyright 2014  Inge Wallin
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "statisticslegendwidget.h"
#include "prefs.h"

#include <KLocalizedString>

#include <QDebug>
#include <QPainter> ///

StatisticsLegendWidget::StatisticsLegendWidget(QWidget * parent)
    : QWidget( parent )
{
    QString ttip;
    ttip += "<table><tr><td>Progress gradients</td><td bgcolor=\""
            + Prefs::gradeColor().name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>Early progress gradients</td><td bgcolor=\""
            + Prefs::preGradeColor().name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>Invalid Entries</td><td bgcolor=\""
            + Prefs::invalidUnitColor().name()+"\" width=\"15%\"><nobr>    </nobr></td></tr></table>";
    setToolTip(ttip );
}

QColor StatisticsLegendWidget::gradeColor(int grade) {
    QColor color( Prefs::gradeColor() );
    color.setAlpha( alphaMax - ( KV_MAX_GRADE - grade - 1) * alphaStep );
    return color;
}

QColor StatisticsLegendWidget::preGradeColor(int grade) {
    QColor color( Prefs::preGradeColor() );
    color.setAlpha( alphaMax - ( KV_MAX_GRADE - grade - 1) * alphaStep );
    return color;
}

QColor StatisticsLegendWidget::invalidColor() {
    return Prefs::invalidUnitColor();
}

void StatisticsLegendWidget::paintStatisticsBar(
    QPainter & painter, const QRect & rect,  const QList< QVariant> &fractions) {
    QRect roundedRect(rect);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

    QColor color;
    int xPosition = 0;
    for(int ii = KV_MAX_GRADE + KV_MAX_GRADE; ii >= 0; --ii ) {
        double fraction( fractions[ii].toDouble() );
        int barElementWidth = fraction * rect.width();
        QRectF barElement(rect.x() + xPosition, rect.y(), barElementWidth, rect.height());
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        xPosition += barElementWidth;
        // Intersect the QPainterPath of inner rectangle with outer,
        // so that the inner rectangle takes the shape of the outer rounded rectangle.
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);

        if ( ii > KV_MAX_GRADE ) {
            color = StatisticsLegendWidget::gradeColor( ii - KV_MAX_GRADE - 1);
        } else if ( ii != 0 ) {
            color = StatisticsLegendWidget::preGradeColor( ii - 1 );
        } else {
            color = StatisticsLegendWidget::invalidColor();
        }

        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }

}


void StatisticsLegendWidget::paintEvent(QPaintEvent *) {

    const int legendWidth = 320;
    const int legendHeight = 30;
    const int legendOffsetY = 10;
    const int legendOffsetX = (width() / 2) - (legendWidth / 2);
    const int extraRoomInRectSize = 10;
    const int horizontalDistanceFromLegend = 10;

    QPainter painter(this);

    QString leftString = i18nc("adjective, The word has been properly and fully learned by the user","Fully learned");
    QString rightString = i18nc("adjective, The word has not even been practiced once by the user","Not practiced");
    QFontMetrics fontMetrics(painter.font());

    // Calculate the size and position of the rectangle that will contain the string on the left side of the legend
    QRect leftRect = fontMetrics.boundingRect(leftString);
    leftRect.setWidth(leftRect.width() + extraRoomInRectSize);
    leftRect.setHeight(leftRect.height() + extraRoomInRectSize);
    leftRect.moveBottomRight(QPoint(legendOffsetX - horizontalDistanceFromLegend, legendOffsetY + legendHeight));

    // Calculate the size and position of the rectangle that will contain the string on the right side of the legend
    QRect rightRect = fontMetrics.boundingRect(rightString);
    rightRect.setWidth(rightRect.width() + extraRoomInRectSize);
    rightRect.setHeight(rightRect.height() + extraRoomInRectSize);
    rightRect.moveBottomLeft(QPoint(legendOffsetX + legendWidth + horizontalDistanceFromLegend, legendOffsetY + legendHeight));

    QRect rect(x() + legendOffsetX, legendOffsetY, legendWidth, legendHeight);
    QList<QVariant> fractions(QList<QVariant>::fromVector( QVector<QVariant>(KV_MAX_GRADE+KV_MAX_GRADE+1, ( double ) 1.0/(KV_MAX_GRADE+KV_MAX_GRADE+1) ) ) );
    paintStatisticsBar( painter, rect, fractions);

    painter.setPen(Qt::black);
    painter.drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, leftString);
    painter.drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, rightString);

}
