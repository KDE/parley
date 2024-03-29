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
    explicit BoxesWidget(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;

    void setRenderer(ThemedBackgroundRenderer *renderer);
    void setBoxCount(int boxCount);
    void setBoxes(int currentBox, int lastBox = -1);

    void updatePixmap();

protected:
    bool event(QEvent *e) override;

private:
    void drawElement(QPainter *p, const QString &id);

    int m_boxCount{1};
    int m_currentBox{-1};
    int m_lastBox{-1};

    ThemedBackgroundRenderer *m_renderer{nullptr};

    QPixmap m_box;
    QPixmap m_activeBox;

    QPixmap m_arrowBegin;
    QPixmap m_arrowCenter;
    QPixmap m_arrowEnd;

    QRect m_rect;
    bool m_fixedSize{true};

    int m_arrowHint{0};
    int m_spacingHint{0};
};

}

#endif // PRACTICE_BOXESWIDGET_H
