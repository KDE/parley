/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// Own
#include "statisticslegendwidget.h"

// Qt
#include <QDebug>
#include <QPainter> ///

// KDE
#include <KLocalizedString>

// Parley
#include "utils.h"
#include <KEduVocExpression>

using namespace Qt::Literals::StringLiterals;

StatisticsLegendWidget::StatisticsLegendWidget(QWidget *parent)
    : QWidget(parent)
{
    ConfidenceColors colors(ConfidenceColors::ProgressiveColorScheme);

    QString tooltip;
    tooltip += u"<table><tr><td>"_s + i18n("Progress gradients") + u"</td><td bgcolor=\""_s + colors.longTermColors[KV_MAX_GRADE].name()
        + u"\"><nobr>    </nobr></td></tr>"_s + u"<tr><td>"_s + i18n("Early progress") + u"</td><td bgcolor=\""_s + colors.initialTermColor.name()
        + u"\"><nobr>    </nobr></td></tr>"_s + u"<tr><td>"_s + i18n("Not Practiced") + u"</td><td bgcolor=\""_s + colors.longTermColors[0].name()
        + u"\"><nobr>    </nobr></td></tr>"_s + u"<tr><td>"_s + i18n("Invalid Entries") + u"</td><td bgcolor=\""_s + colors.invalidColor.name()
        + u"\" width=\"15%\"><nobr>    </nobr></td></tr></table>"_s;
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

    QString leftString = i18nc("adjective, The word has been properly and fully learned by the user", "Fully learned");
    QString rightString = i18nc("adjective, The word has not even been practiced once by the user", "Not practiced");
    QFontMetrics fontMetrics(painter.font());

    // Calculate the size and position of the rectangle that will contain the
    // string on the left side of the legend.
    QRect leftRect = fontMetrics.boundingRect(leftString);
    leftRect.setWidth(leftRect.width() + extraRoomInRectSize);
    leftRect.setHeight(leftRect.height() + extraRoomInRectSize);
    leftRect.moveBottomRight(QPoint(legendOffsetX - horizontalDistanceFromLegend, legendOffsetY + legendHeight));

    // Calculate the size and position of the rectangle that will contain the
    // string on the right side of the legend.
    QRect rightRect = fontMetrics.boundingRect(rightString);
    rightRect.setWidth(rightRect.width() + extraRoomInRectSize);
    rightRect.setHeight(rightRect.height() + extraRoomInRectSize);
    rightRect.moveBottomLeft(QPoint(legendOffsetX + legendWidth + horizontalDistanceFromLegend, legendOffsetY + legendHeight));

    // Draw the legend color bar.
    QRect rect(x() + legendOffsetX, legendOffsetY, legendWidth, legendHeight);
    WordCount wordCount;
    for (int i = 0; i <= KV_MAX_GRADE; ++i) {
        wordCount.grades[i] = 1;
    }
    wordCount.invalid = 1;
    wordCount.initialWords = 1;
    wordCount.totalWords = 10;

    ConfidenceColors colors(ConfidenceColors::ProgressiveColorScheme);
    paintColorBar(painter, rect, wordCount, colors);

    // Draw the legend texts
    painter.setPen(QPalette().color(QPalette::WindowText));
    painter.drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, leftString);
    painter.drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, rightString);
}

#include "moc_statisticslegendwidget.cpp"
