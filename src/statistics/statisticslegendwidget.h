/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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
