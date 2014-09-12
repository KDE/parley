/***************************************************************************
    Copyright 2014 Andreas Xavier
    Copyright 2014 Inge Wallin
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// Qt
#include <QPaintEvent>
#include <QWidget>

extern QColor gradeColor[11];


class BarWidget : public QWidget
{
public:
    BarWidget(QWidget *parent = 0);
    BarWidget(int [], int, int, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    int dueWords[8];
    int totalDueWords;
    int percentageCompleted;
};

