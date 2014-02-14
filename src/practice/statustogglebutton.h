/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_STATUSTOGGLEBUTTON_H
#define PRACTICE_STATUSTOGGLEBUTTON_H

#include "imagewidget.h"
#include <kdebug.h> //TODO

namespace Practice
{

class StatusToggleButton : public ImageWidget
{
    Q_OBJECT

public:
    StatusToggleButton(QWidget* parent = 0) : ImageWidget(parent), m_current(0) {
        setMouseTracking(true);
    }

    void setPixmaps(QPixmap defaultPixmap, QPixmap hoverPixmap, QPixmap pressedPixmap);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void clicked();

private:
    QPixmap m_defaultPixmap;
    QPixmap m_hoverPixmap;
    QPixmap m_pressedPixmap;
    int m_current;
};
}
#endif // PRACTICE_STATUSTOGGLEBUTTON_H
