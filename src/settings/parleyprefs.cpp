/*
    parleyprefs.cpp  -  Parley configuration dialog
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "parleyprefs.h"
#include "generaloptions.h"
#include "kgametheme/kgamethemeselector.h"
#include "translationshelloptions.h"
#include "viewoptions.h"
#include <KConfigSkeleton>
#include <KLocalizedString>

ParleyPrefs::ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
    : KConfigDialog(parent, name, config)
    , m_config{config}
    , m_doc{doc}
{
    button(QDialogButtonBox::Ok)->setDefault(true);
    setModal(true);

    addPage(new KGameThemeSelector(this, config), i18n("Theme"), QStringLiteral("games-config-theme"));

    m_generalOptions = new GeneralOptions(this);
    addPage(m_generalOptions, i18nc("title:window general settings", "General"), QStringLiteral("parley"), i18n("General Settings"));

    m_viewOptions = new ViewOptions(this);
    addPage(m_viewOptions, i18n("View"), QStringLiteral("view-choose"), i18n("View Settings"));

    m_translationShellOptions = new TranslationShellOptions(this);
    addPage(m_translationShellOptions, i18n("Online Translations"), QStringLiteral("internet-services"), i18n("Online Translation Services"));

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
    Q_EMIT settingsChanged(QLatin1String(""));
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

#include "moc_parleyprefs.cpp"
