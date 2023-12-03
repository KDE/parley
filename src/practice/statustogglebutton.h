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
    StatusToggleButton(QWidget *parent = nullptr)
        : ImageWidget(parent)
    {
        setMouseTracking(true);
    }

    void setPixmaps(const QPixmap &defaultPixmap, const QPixmap &hoverPixmap, const QPixmap &pressedPixmap);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;

Q_SIGNALS:
    void clicked();

private:
    QPixmap m_defaultPixmap;
    QPixmap m_hoverPixmap;
    QPixmap m_pressedPixmap;
    int m_current{0};
};
}
#endif // PRACTICE_STATUSTOGGLEBUTTON_H
