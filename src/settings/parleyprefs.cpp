/***************************************************************************
             parleyprefs.cpp  -  Parley configuration dialog

                             -------------------
    begin         : Fri Mar 25 2005

    copyright     : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "parleyprefs.h"

#include "kgametheme/kgamethemeselector.h"
#include "generaloptions.h"
#include "viewoptions.h"
#include <KConfigSkeleton>

ParleyPrefs::ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
  : KConfigDialog(parent, name, config)
{
    setButtons(Default|Ok|Apply|Cancel|Help);
    setDefaultButton(Ok);
    setModal(true);

    m_doc = doc;
    m_config = config;

    addPage(new KGameThemeSelector(this, config), i18n("Theme"), "games-config-theme");

    m_generalOptions = new GeneralOptions(0);
    addPage(m_generalOptions, i18nc("title:window general settings", "General"), "parley", i18n("General Settings"), true);

    m_viewOptions = new ViewOptions(0);
    addPage(m_viewOptions, i18n("View"), "view-choose", i18n("View Settings"), true);
    setHelp(QString(),"parley");
}

bool ParleyPrefs::hasChanged()
{
    return m_generalOptions->hasChanged();
}

bool ParleyPrefs::isDefault()
{
    return m_generalOptions->isDefault();
}

void ParleyPrefs::updateSettings()
{
    m_generalOptions->updateSettings();
    emit settingsChanged("");
}

void ParleyPrefs::updateWidgetsDefault()
{
    m_config->useDefaults(true);
    m_generalOptions->updateWidgets();
    m_config->useDefaults(false);
}

void ParleyPrefs::updateWidgets()
{
    m_generalOptions->updateWidgets();
}

#include "parleyprefs.moc"
