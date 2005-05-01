/***************************************************************************

                      general options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Mar 29 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

GeneralOptions::GeneralOptions(QWidget* parent, const char* name, WFlags fl)
: GeneralOptionsBase(parent,name,fl)
{
 kcfg_BackupTime->setEnabled(kcfg_AutoBackup->isChecked());
}


#include "generaloptions.moc"

