/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    explicit StatusToggle(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;

    void setRenderer(ThemedBackgroundRenderer *renderer);
    void setResultState(AbstractFrontend::ResultState state);

    void updatePixmap();
    void updateToggle();

signals:
    void toggle();

private:
    ThemedBackgroundRenderer *m_renderer{nullptr};

    StatusToggleButton *m_toggle{nullptr};
    QAction *m_toggleAction{nullptr};

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
