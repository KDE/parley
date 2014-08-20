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

#ifndef STATISTICSLEGENDWIDGET_H
#define STATISTICSLEGENDWIDGET_H

#include "keduvoctext.h"
#include <QWidget>
#include <QColor>
#include <QList>

/**Draws the statistics legend and provides color information about the legend**/
class StatisticsLegendWidget : public QWidget
{
    Q_OBJECT
public:
    /// constructor
    StatisticsLegendWidget(QWidget * parent = 0);

    /**@brief the color of the @p grade
       @param grade
       @returns the color **/
    static QColor gradeColor(int grade);
    /**@returns the color of the @p preGrade
       @param preGrade
    **/
    static QColor preGradeColor(int preGrade);
    /**@returns the color of the invalid entries**/
    static QColor invalidColor();

    /** With a @p painter draw the stats bar with @p fractions within @p rect
     @param painter supplied painter
     @param rect bounding box
     @param fractions the fractional widths of each of the grade categories
    **/
    static void paintStatisticsBar(QPainter & painter, const QRect & rect, const QList< QVariant> &fractions);

protected:
    /// paintEvent @p e event
    void paintEvent(QPaintEvent *e);
private:
    static const int alphaMax = 255;  ///< full brightness
    /// steps small enought that the last is still visible
    static const int alphaStep = ((alphaMax - 10) / KV_MAX_GRADE);
};

#endif
