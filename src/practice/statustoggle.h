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

#ifndef PRACTICE_STATUSTOGGLE_H
#define PRACTICE_STATUSTOGGLE_H

#include "imagewidget.h"

#include "abstractfrontend.h"

namespace Practice
{

class ThemedBackgroundRenderer;
class StatusToggleButton;

class StatusToggle : public ImageWidget
{
    Q_OBJECT

public:
    StatusToggle(QWidget* parent = 0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    void setRenderer(ThemedBackgroundRenderer *renderer);
    void setResultState(AbstractFrontend::ResultState state);

    void updatePixmap();
    void updateToggle();

signals:
    void toggle();

private:
    ThemedBackgroundRenderer *m_renderer;

    StatusToggleButton *m_toggle;
    QAction *m_toggleAction;

    AbstractFrontend::ResultState m_resultState;

    QPixmap m_defaultStatus;
    QPixmap m_correctStatus;
    QPixmap m_wrongStatus;

    QPixmap m_toggleCorrect;
    QPixmap m_toggleCorrectHover;
    QPixmap m_toggleCorrectPressed;

    QPixmap m_toggleWrong;
    QPixmap m_toggleWrongHover;
    QPixmap m_toggleWrongPressed;
};

}

#endif // PRACTICE_STATUSTOGGLE_H
