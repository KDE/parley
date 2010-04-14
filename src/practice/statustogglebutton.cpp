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

#include "statustogglebutton.h"

using namespace Practice;

void StatusToggleButton::setPixmaps(QPixmap defaultPixmap, QPixmap hoverPixmap, QPixmap pressedPixmap)
{
    if (m_current == 1) {
        setPixmap(hoverPixmap);
    } else {
        setPixmap(defaultPixmap);
    }
    m_defaultPixmap = defaultPixmap;
    m_hoverPixmap = hoverPixmap;
    m_pressedPixmap = pressedPixmap;
}

void StatusToggleButton::mousePressEvent(QMouseEvent *e)
{
    if (!m_defaultPixmap.isNull()) {
        emit clicked();
    }
}

#include "statustogglebutton.moc"
