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

#include "welcomescreen.h"

WelcomeScreen::WelcomeScreen(QWidget *parent)
 : QWidget(parent)
{
    ui = new Ui::WelcomeScreen();
    ui->setupUi(this);
}

#include "welcomescreen.moc"
