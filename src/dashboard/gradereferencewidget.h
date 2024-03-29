/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GRADEREFERENCEWIDGET_H
#define GRADEREFERENCEWIDGET_H

#include <QWidget>

class GradeReferenceWidget : public QWidget
{
public:
    explicit GradeReferenceWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif
