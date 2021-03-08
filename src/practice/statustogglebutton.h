/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_STATUSTOGGLEBUTTON_H
#define PRACTICE_STATUSTOGGLEBUTTON_H

#include "imagewidget.h"
#include <QDebug> //TODO

namespace Practice
{

class StatusToggleButton : public ImageWidget
{
    Q_OBJECT

public:
    StatusToggleButton(QWidget* parent = 0) : ImageWidget(parent), m_current(0) {
        setMouseTracking(true);
    }

    void setPixmaps(const QPixmap &defaultPixmap, const QPixmap &hoverPixmap, const QPixmap &pressedPixmap);

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

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
