/***************************************************************************

                      general options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Mar 29 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <qcheckbox.h>

#include <knuminput.h>

#include "generaloptions.h"

GeneralOptions::GeneralOptions(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
  connect(kcfg_AutoBackup, SIGNAL(toggled(bool)), kcfg_BackupTime, SLOT(setEnabled(bool)));

  kcfg_BackupTime->setEnabled(kcfg_AutoBackup->isChecked());
}

#include "generaloptions.moc"
