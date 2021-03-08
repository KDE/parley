/*
    SPDX-FileCopyrightText: 2009-2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_BOXESWIDGET_H
#define PRACTICE_BOXESWIDGET_H

#include "imagewidget.h"

namespace Practice
{

class ThemedBackgroundRenderer;

class BoxesWidget : public ImageWidget
{
    Q_OBJECT

public:
    explicit BoxesWidget(QWidget* parent = 0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    void setRenderer(ThemedBackgroundRenderer *renderer);
    void setBoxCount(int boxCount);
    void setBoxes(int currentBox, int lastBox = -1);

    void updatePixmap();

protected:
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    void drawElement(QPainter *p, const QString& id);

    int m_boxCount;
    int m_currentBox;
    int m_lastBox;

    ThemedBackgroundRenderer *m_renderer;

    QPixmap m_box;
    QPixmap m_activeBox;

    QPixmap m_arrowBegin;
    QPixmap m_arrowCenter;
    QPixmap m_arrowEnd;

    QRect m_rect;
    bool m_fixedSize;

    int m_arrowHint;
    int m_spacingHint;
};

}

#endif // PRACTICE_BOXESWIDGET_H
