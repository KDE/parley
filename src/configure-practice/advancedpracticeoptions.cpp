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
    connect(NoTimeoutRadio, SIGNAL(toggled(bool)), label_mqtime, SLOT(setDisabled(bool)));
}


#include "advancedpracticeoptions.moc"
