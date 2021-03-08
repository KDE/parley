/*
    query options for Parley
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SPECIFICPRACTICEOPTIONS_H
#define SPECIFICPRACTICEOPTIONS_H

#include "ui_specificpracticeoptions.h"

class SpecificPracticeOptions : public QWidget, public Ui::SpecificPracticeOptions
{
    Q_OBJECT
public:
    explicit SpecificPracticeOptions(QWidget* parent = 0);
};

#endif
