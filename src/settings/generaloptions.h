/*
    general options for parley
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H

#include "ui_generaloptionsbase.h"

class GeneralOptions : public QWidget, public Ui::GeneralOptionsBase
{
    Q_OBJECT
public:
    explicit GeneralOptions(QWidget *parent = nullptr);

    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

private Q_SLOTS:
    void fillWidgets();
};

#endif
