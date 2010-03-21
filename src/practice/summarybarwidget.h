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


#ifndef SUMMARYBARWIDGET_H
#define SUMMARYBARWIDGET_H

#include <QtGui/QWidget>

namespace Practice {

class SummaryBarWidget : public QWidget
{
    Q_OBJECT

public:
    SummaryBarWidget(QWidget *parent = 0);

    void setStatistics(int correct, int wrong, int notAnswered);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_correct;
    int m_wrong;
    int m_notAnswered;
};

}

#endif // SUMMARYBARWIDGET_H
