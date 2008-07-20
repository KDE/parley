/***************************************************************************
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "ui_welcomescreen.h"

#include <QWidget>

class WelcomeScreen : public QWidget
{
Q_OBJECT
public:
    WelcomeScreen(QWidget *parent = 0);

private:
    Ui::WelcomeScreen* ui;
};

#endif
