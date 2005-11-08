/***************************************************************************

      view options for kvoctrain

    -----------------------------------------------------------------------

    begin: Thu Mar 31 2005

    copyright    :(C) 2005 Peter Hedlund

    email: peter.hedlund@kdemail.net

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 * *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.   *
 * *
 ***************************************************************************/

#include <qcheckbox.h>

#include "viewoptions.h"

ViewOptions::ViewOptions(QWidget* parent, const char* name, Qt::WFlags fl)
: ViewOptionsBase(parent,name,fl)
{
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol7, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol6, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol5, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol4, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol3, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol2, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol1, SLOT(setEnabled(bool)));
  connect(kcfg_UseGradeCol, SIGNAL(toggled(bool)), kcfg_GradeCol0, SLOT(setEnabled(bool)));

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

