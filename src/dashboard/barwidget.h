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


#ifndef BARWIDGET_H
#define BARWIDGET_H

// Qt
#include <QPaintEvent>
#include <QWidget>


#include "utils.h"
#include "collection.h"  // For struct WordCount


extern ConfidenceColors globalColors;


class BarWidget : public QWidget
{
public:
    BarWidget(QWidget *parent = 0);
    BarWidget(WordCount *dueWords, QWidget *parent = 0);

    void setDue(WordCount &wc);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    int m_dueWords[8];
    int m_totalDueWords;
    int m_percentageCompleted;
};


#endif // BARWIDGET_H
