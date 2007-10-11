/***************************************************************************

      view options for Parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 31 2005

    copyright     :(C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "viewoptions.h"

#include <QCheckBox>

ViewOptions::ViewOptions(QWidget* parent) : QWidget(parent)
{
    setupUi(this);
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor7, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor6, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor5, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor4, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor3, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor2, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor1, SLOT(setEnabled(bool)));
    connect(kcfg_UseGradeColors, SIGNAL(toggled(bool)), kcfg_GradeColor0, SLOT(setEnabled(bool)));

    kcfg_GradeColor0->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor1->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor2->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor3->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor4->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor5->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor6->setEnabled(kcfg_UseGradeColors->isChecked());
    kcfg_GradeColor7->setEnabled(kcfg_UseGradeColors->isChecked());
}

#include "viewoptions.moc"
