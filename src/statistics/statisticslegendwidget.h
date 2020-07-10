/*
 *  Copyright 2014  Andreas Xavier <andxav at zoho dot com>
 *  Copyright 2014  Inge Wallin <inge@lysator.liu.se>
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

#ifndef STATISTICSLEGENDWIDGET_H
#define STATISTICSLEGENDWIDGET_H

#include <KEduVocText>

#include <QWidget>
#include <QColor>
#include <QList>


/**Draws the statistics legend and provides color information about the legend**/
class StatisticsLegendWidget : public QWidget
{
    Q_OBJECT
public:
    /// constructor
    explicit StatisticsLegendWidget(QWidget * parent = 0);


protected:
    /// paintEvent @p e event
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
};


#endif
