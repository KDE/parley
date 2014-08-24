/*
 *  Copyright 2014  Andreas Xavier <andxav at zoho dot com>
 *  Copyright 2014  Inge Wallin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License or any later version accepted by the membership of
 * KDE e.V. (or its successor approved by the membership of KDE
 * e.V.), which shall act as a proxy defined in Section 14 of
 * version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Own
#include "statisticslegendwidget.h"

// Qt
#include <QDebug>
#include <QPainter> ///

// KDE
#include <KLocalizedString>

// Parley
#include <keduvocexpression.h>
#include "prefs.h"


StatisticsLegendWidget::StatisticsLegendWidget(QWidget * parent)
    : QWidget(parent)
{
    QString tooltip;
    tooltip += "<table><tr><td>"+i18n("Progress gradients")+"</td><td bgcolor=\""
            + gradeColor(KV_MAX_GRADE - 1).name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Early progress")+"</td><td bgcolor=\""
            + preGradeColor().name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Not Practiced")+"</td><td bgcolor=\""
            + unpracticedColor().name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Invalid Entries")+"</td><td bgcolor=\""
            + invalidColor().name()+"\" width=\"15%\"><nobr>    </nobr></td></tr></table>";
    setToolTip(tooltip);
}


QColor StatisticsLegendWidget::gradeColor(int grade)
{
    QColor color(Prefs::gradeColor());
    color.setAlpha(alphaMax - (KV_MAX_GRADE - grade - 1) * alphaStep);
    return color;
}

QColor StatisticsLegendWidget::preGradeColor()
{
    QColor color(Prefs::preGradeColor());
    color.setAlpha(alphaMax);
    return color;
}

QColor StatisticsLegendWidget::unpracticedColor()
{
    QColor color("#FFFFFF");
    color.setAlpha(alphaMax);
    return color;
}

QColor StatisticsLegendWidget::invalidColor()
{
    return Prefs::invalidUnitColor();
}

QList< QVariant > StatisticsLegendWidget::legendFractions(KEduVocContainer & container,  int index)
{
    QVector<double> sums(KV_MAX_GRADE + fractionsOffset + 1, 0);
    double count(0);

    // 'index' is the index of the translation inside the KEduVocExpression.
    foreach (KEduVocExpression *entry, container.entries(KEduVocContainer::Recursive)) {
        KEduVocTranslation & trans(*entry->translation(index));
        ++count;
        if (!trans.isEmpty()) {
            if (trans.grade() != 0) {
                sums[trans.grade() + fractionsOffset - 1] += 1;
            } else {
                if (trans.preGrade() != 0) {
                    sums[2] += 1;
                } else {
                    sums[1] += 1; //unpracticed
                }
            }
        } else {
            sums[0] += 1; //invalid
        }
    }

    QList< QVariant > fractions;
    for (int ii = 0 ;ii < KV_MAX_GRADE + fractionsOffset; ++ii) {
        fractions.push_back((double) (sums[ii] / count));
    }

    return fractions;
}

void StatisticsLegendWidget::paintStatisticsBar(QPainter &painter, const QRect &rect,
                  const QList< QVariant> &fractions)
{
    QRect roundedRect(rect);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

    QColor color;
    int xPosition = 0;
    for (int ii = KV_MAX_GRADE + fractionsOffset - 1; ii >= 0; --ii) {
        double fraction(fractions[ii].toDouble());
        int barElementWidth = fraction * rect.width();
        QRectF barElement(rect.x() + xPosition, rect.y(), barElementWidth, rect.height());
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        xPosition += barElementWidth;

        // Intersect the QPainterPath of inner rectangle with outer,
        // so that the inner rectangle takes the shape of the outer rounded rectangle.
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);

        if (ii >= fractionsOffset) {
            color = StatisticsLegendWidget::gradeColor(ii - fractionsOffset);
        } else {
            switch (ii) {
            case 2:
                color = StatisticsLegendWidget::preGradeColor();
                break;
            case 1:
                color = StatisticsLegendWidget::unpracticedColor();
                break;
            case 0:
            default:
                color = StatisticsLegendWidget::invalidColor();
                break;
            }
        }

        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }
}


void StatisticsLegendWidget::paintEvent(QPaintEvent *)
{
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

    // Calculate the size and position of the rectangle that will contain the
    // string on the left side of the legend.
    QRect leftRect = fontMetrics.boundingRect(leftString);
    leftRect.setWidth(leftRect.width() + extraRoomInRectSize);
    leftRect.setHeight(leftRect.height() + extraRoomInRectSize);
    leftRect.moveBottomRight(QPoint(legendOffsetX - horizontalDistanceFromLegend,
                legendOffsetY + legendHeight));

    // Calculate the size and position of the rectangle that will contain the
    // string on the right side of the legend.
    QRect rightRect = fontMetrics.boundingRect(rightString);
    rightRect.setWidth(rightRect.width() + extraRoomInRectSize);
    rightRect.setHeight(rightRect.height() + extraRoomInRectSize);
    rightRect.moveBottomLeft(QPoint(legendOffsetX + legendWidth + horizontalDistanceFromLegend,
                legendOffsetY + legendHeight));

    QRect rect(x() + legendOffsetX, legendOffsetY, legendWidth, legendHeight);
    QList<QVariant> fractions(
        QList<QVariant>::fromVector(
            QVector<QVariant>(KV_MAX_GRADE + fractionsOffset + 1,
               (double) 1.0 / (KV_MAX_GRADE + fractionsOffset))));
    paintStatisticsBar(painter, rect, fractions);

    painter.setPen(Qt::black);
    painter.drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, leftString);
    painter.drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, rightString);
}
