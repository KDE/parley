/***************************************************************************

                      general options for Parley

    -----------------------------------------------------------------------

    begin        : Tue Mar 29 2005

    copyright    : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "generaloptions.h"

#include <QCheckBox>
#include <KLocalizedString>

#include <QSpinBox>
#include <kfile.h>

#include "prefs.h"

static const char *separator_id[] = {
    ";",              // 0
    "#",              // 1
    "!",              // 2
    "|",              // 3
    ",",              // 4
    "\t",             // 5
    "  ",             // 6
    ":",              // 7
    "::",             // 8
    0
};

GeneralOptions::GeneralOptions(QWidget* parent) : QWidget(parent)
{
    setupUi(this);
    connect(kcfg_AutoBackup, SIGNAL(toggled(bool)), kcfg_BackupTime, SLOT(setEnabled(bool)));

    fillWidgets();
    updateWidgets();
    kcfg_BackupTime->setEnabled(kcfg_AutoBackup->isChecked());
}


void GeneralOptions::fillWidgets()
{
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", ";"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", "#"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", "!"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", "|"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", ","));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator: tabulator", "TAB"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", ">= 2 SPACES"));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", " : "));
    kcfg_SeparatorCombo->addItem(i18nc("CSV separator", " :: "));
}

void GeneralOptions::updateWidgets()
{
    for (int i = 0; i < 9; ++i) {
        if (separator_id[i] == Prefs::separator()) {
            kcfg_SeparatorCombo->setCurrentIndex(i);
            break;
        }
    }

}


bool GeneralOptions::hasChanged()
{
    if (kcfg_SeparatorCombo->currentIndex() < 0)
        return false;

    return (separator_id[kcfg_SeparatorCombo->currentIndex()] != Prefs::separator());
}


bool GeneralOptions::isDefault()
{
    if (kcfg_SeparatorCombo->currentIndex() < 0)
        return false;
    return !strcmp(separator_id[kcfg_SeparatorCombo->currentIndex()], "\t");
}


void GeneralOptions::updateSettings()
{
    if (kcfg_SeparatorCombo->currentIndex() < 0)
        return;
    Prefs::setSeparator((separator_id[kcfg_SeparatorCombo->currentIndex()]));
}
