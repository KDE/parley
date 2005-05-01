/***************************************************************************

                      view options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 31 2005

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

#include "viewoptions.h"

ViewOptions::ViewOptions(QWidget* parent, const char* name, WFlags fl)
: ViewOptionsBase(parent,name,fl)
{
  kcfg_GradeCol0->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol1->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol2->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol3->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol4->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol5->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol6->setEnabled(kcfg_UseGradeCol->isChecked());
  kcfg_GradeCol7->setEnabled(kcfg_UseGradeCol->isChecked());
}

#include "viewoptions.moc"

