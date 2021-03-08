/*
    query options for Parley
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GENERALPRACTICEOPTIONS_H
#define GENERALPRACTICEOPTIONS_H

#include "ui_generalpracticeoptions.h"

class GeneralPracticeOptions : public QWidget, public Ui::GeneralPracticeOptions
{
    Q_OBJECT
public:
    explicit GeneralPracticeOptions(QWidget* parent = 0);
};

#endif
