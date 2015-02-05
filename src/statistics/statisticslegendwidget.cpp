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
#include "utils.h"


StatisticsLegendWidget::StatisticsLegendWidget(QWidget * parent)
    : QWidget(parent)
{
    ConfidenceColors colors(ConfidenceColors::ProgressiveColorScheme);

    QString tooltip;
    tooltip += "<table><tr><td>"+i18n("Progress gradients")+"</td><td bgcolor=\""
            + colors.longTermColors[KV_MAX_GRADE].name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Early progress")+"</td><td bgcolor=\""
            + colors.initialTermColor.name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Not Practiced")+"</td><td bgcolor=\""
            + colors.longTermColors[0].name()+"\"><nobr>    </nobr></td></tr>"
            + "<tr><td>"+i18n("Invalid Entries")+"</td><td bgcolor=\""
            + colors.invalidColor.name()+"\" width=\"15%\"><nobr>    </nobr></td></tr></table>";
    setToolTip(tooltip);
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

    I18N_NOOP("Initial phase");
    I18N_NOOP("Long term phase");
    I18N_NOOP("Invalid entry");
    I18N_NOOP("Newly started");

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

    // Draw the legend color bar.
    QRect rect(x() + legendOffsetX, legendOffsetY, legendWidth, legendHeight);
    WordCount  wordCount;
    for (int i = 0; i <= KV_MAX_GRADE; ++i) {
        wordCount.grades[i] = 1;
    }
    wordCount.invalid = 1;
    wordCount.initialWords = 1;
    wordCount.totalWords = 10;

    ConfidenceColors colors(ConfidenceColors::ProgressiveColorScheme);
    paintColorBar(painter, rect, wordCount, colors);

    // Draw the legend texts
    painter.setPen(Qt::black);
    painter.drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, leftString);
    painter.drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, rightString);
}
