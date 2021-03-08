/*
    parleyprefs.cpp  -  Parley configuration dialog
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "parleyprefs.h"

#include "kgametheme/kgamethemeselector.h"
#include "generaloptions.h"
#include "viewoptions.h"
#include <KConfigSkeleton>
#include <KLocalizedString>

ParleyPrefs::ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
    : KConfigDialog(parent, name, config)
{
    button(QDialogButtonBox::Ok)->setDefault( true );
    setModal(true);

    m_doc = doc;
    m_config = config;

    addPage(new KGameThemeSelector(this, config), i18n("Theme"), QStringLiteral("games-config-theme"));

    m_generalOptions = new GeneralOptions(0);
    addPage(m_generalOptions, i18nc("title:window general settings", "General"), QStringLiteral("parley"), i18n("General Settings"), true);

    m_viewOptions = new ViewOptions(0);
    addPage(m_viewOptions, i18n("View"), QStringLiteral("view-choose"), i18n("View Settings"), true);
    setHelp(QString(), QStringLiteral("parley"));
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
    emit settingsChanged(QLatin1String(""));
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
