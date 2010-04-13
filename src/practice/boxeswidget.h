/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_BOXESWIDGET_H
#define PRACTICE_BOXESWIDGET_H

#include <QtGui/QWidget>


namespace Practice {

class BoxesWidget : public QWidget
{
    Q_OBJECT

public:
    BoxesWidget(QWidget* parent = 0);
    virtual QSize minimumSizeHint() const;

    void setBoxCount(int boxCount);
    void setBoxes(int currentBox, int lastBox = -1);

protected:
    virtual void paintEvent(QPaintEvent* e);

private:
    int m_boxCount;
    int m_currentBox;
    int m_lastBox;

    static const int boxWidth = 20;
    static const int boxHeight = 20;
    static const int boxSpacing = 4;
    static const int arrowHeight = 10;
    static const int arrowSize = 5;
};

}

#endif // PRACTICE_BOXESWIDGET_H
