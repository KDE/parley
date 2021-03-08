/*
    view options for Parley
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VIEWOPTIONS_H
#define VIEWOPTIONS_H

#include "ui_viewoptionsbase.h"

class ViewOptions : public QWidget, public Ui::ViewOptionsBase
{
    Q_OBJECT
public:
    explicit ViewOptions(QWidget* parent = 0);
};

#endif
