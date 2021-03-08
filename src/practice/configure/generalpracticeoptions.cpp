/*
    query options for Parley
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generalpracticeoptions.h"

#include <KLocalizedString>

#include "prefs.h"

GeneralPracticeOptions::GeneralPracticeOptions(QWidget* parent) : QWidget(parent)
{
    setupUi(this);

    //connect(kcfg_ShowSolutionAfterAnswer, SIGNAL(toggled(bool)), kcfg_ShowSolutionAfterAnswerTime, SLOT(setEnabled(bool)));
}
