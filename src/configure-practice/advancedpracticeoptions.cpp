/***************************************************************************

                      query options for Parley

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                          (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "advancedpracticeoptions.h"

#include "prefs.h"

AdvancedPracticeOptions::AdvancedPracticeOptions(QWidget* parent) : QWidget(parent)
{
    setupUi(this);

    // time limit
    connect(NoTimeoutRadio, SIGNAL(toggled(bool)), kcfg_MaxTimePer, SLOT(setDisabled(bool)));
    connect(NoTimeoutRadio, SIGNAL(toggled(bool)), kcfg_ShowCounter, SLOT(setDisabled(bool)));
    connect(NoTimeoutRadio, SIGNAL(toggled(bool)), label_mqtime, SLOT(setDisabled(bool)));

    // wether to split up translations
    connect(kcfg_Split, SIGNAL(toggled(bool)), kcfg_Periods, SLOT(setEnabled(bool)));
    connect(kcfg_Split, SIGNAL(toggled(bool)), kcfg_Colons, SLOT(setEnabled(bool)));
    connect(kcfg_Split, SIGNAL(toggled(bool)), kcfg_Semicolons, SLOT(setEnabled(bool)));
    connect(kcfg_Split, SIGNAL(toggled(bool)), kcfg_Commas, SLOT(setEnabled(bool)));
    connect(kcfg_Split, SIGNAL(toggled(bool)), kcfg_Fields, SLOT(setEnabled(bool)));

    connect(kcfg_Split, SIGNAL(toggled(bool)), label_at, SLOT(setEnabled(bool)));
    connect(kcfg_Split, SIGNAL(toggled(bool)), label_split_max_fields, SLOT(setEnabled(bool)));
}


#include "advancedpracticeoptions.moc"
