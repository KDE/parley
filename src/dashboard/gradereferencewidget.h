/***************************************************************************
    Copyright 2014 Andreas Xavier
    Copyright 2014 Inge Wallin <inge@lysator.liu.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GRADEREFERENCEWIDGET_H
#define GRADEREFERENCEWIDGET_H

#include <QWidget>


class GradeReferenceWidget : public QWidget
{
public:
    explicit GradeReferenceWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

};

#endif
