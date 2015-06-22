/***************************************************************************
    Copyright 2009-2010 Daniel Laidig <d.laidig@gmx.de>
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

#include <keduvocimagewidget.h>

namespace Practice
{

class ThemedBackgroundRenderer;

class BoxesWidget : public KEduVocImageWidget
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
