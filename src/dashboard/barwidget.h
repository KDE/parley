/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


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
    explicit BarWidget(QWidget *parent = 0);
    explicit BarWidget(WordCount *dueWords, QWidget *parent = 0);

    void setDue(WordCount &wc);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    int m_dueWords[8];
    int m_totalDueWords;
    int m_percentageCompleted;
};


#endif // BARWIDGET_H
