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

#include <knuminput.h>
#include <kfile.h>

#include "prefs.h"

static const char *separator_id[] =
    {
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
    connect(SeparatorCombo, SIGNAL(activated(int)), this, SLOT(slotSeparatorComboActivated(int)));

    fillWidgets();
    updateWidgets();
    kcfg_BackupTime->setEnabled(kcfg_AutoBackup->isChecked());
}


void GeneralOptions::fillWidgets()
{
    SeparatorCombo->addItem(i18n(";"));
    SeparatorCombo->addItem(i18n("#"));
    SeparatorCombo->addItem(i18n("!"));
    SeparatorCombo->addItem(i18n("|"));
    SeparatorCombo->addItem(i18n(","));
    SeparatorCombo->addItem(i18n("TAB"));
    SeparatorCombo->addItem(i18n(">= 2 SPACES"));
    SeparatorCombo->addItem(i18n(" : "));
    SeparatorCombo->addItem(i18n(" :: "));
}

void GeneralOptions::updateWidgets()
{
    for (int i = 0; i < 9; ++i) {
        if (separator_id[i] == Prefs::separator()) {
            SeparatorCombo->setCurrentIndex(i);
            break;
        }
    }

}


void GeneralOptions::slotSeparatorComboActivated(int)
{
    emit widgetModified();
}


bool GeneralOptions::hasChanged()
{
    if (SeparatorCombo->currentIndex() < 0)
        return false;

    return (separator_id[SeparatorCombo->currentIndex()] != Prefs::separator());
}


bool GeneralOptions::isDefault()
{
    if (SeparatorCombo->currentIndex() < 0)
        return false;
    return !strcmp(separator_id[SeparatorCombo->currentIndex()], "\t");
}


void GeneralOptions::updateSettings()
{
    if (SeparatorCombo->currentIndex() < 0)
        return;
    Prefs::setSeparator((separator_id[SeparatorCombo->currentIndex()]));
}

#include "generaloptions.moc"
