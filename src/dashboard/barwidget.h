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

#include "collection.h" // For struct WordCount
#include "utils.h"

extern ConfidenceColors globalColors;

class BarWidget : public QWidget
{
public:
    explicit BarWidget(QWidget *parent = nullptr);
    explicit BarWidget(WordCount *dueWords, QWidget *parent = nullptr);

    void setDue(WordCount &wc);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int m_dueWords[8];
    int m_totalDueWords;
    int m_percentageCompleted;
};

#endif // BARWIDGET_H
