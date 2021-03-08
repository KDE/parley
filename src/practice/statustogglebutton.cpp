/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statustogglebutton.h"

#include <QMouseEvent>

using namespace Practice;

void StatusToggleButton::setPixmaps(const QPixmap &defaultPixmap, const QPixmap &hoverPixmap, const QPixmap &pressedPixmap)
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
    if (e->button() == Qt::LeftButton) {
        this->setFadingEnabled(false);
        setPixmap(m_pressedPixmap);
    }
}

void StatusToggleButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (!m_defaultPixmap.isNull() && e->button() == Qt::LeftButton) {
        emit clicked();
        if (m_current == 1) {
            setPixmap(m_hoverPixmap);
        } else {
            setPixmap(m_defaultPixmap);
        }
    }
    this->setFadingEnabled(true);
}

void StatusToggleButton::enterEvent(QEvent *)
{
    m_current = 1;
    setPixmap(m_hoverPixmap);
}

void StatusToggleButton::leaveEvent(QEvent *)
{
    m_current = 0;
    setPixmap(m_defaultPixmap);
}
