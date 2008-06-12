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

#include "generaloptions.h"
#include "viewoptions.h"
#include "pluginoptions.h"
#include <KConfigSkeleton>
#include <KPluginSelector>
#include <KPluginInfo>
#include <QList>
#include <KDebug>


ParleyPrefs::ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
  : KConfigDialog(parent, name, config)
{
    setButtons(Default|Ok|Apply|Cancel|Help);
    setDefaultButton(Ok);
    setModal(true);

    m_doc = doc;
    m_config = config;

    m_generalOptions = new GeneralOptions(0);
    addPage(m_generalOptions, i18nc("title:window general settings", "General"), "parley", i18n("General Settings"), true);

    m_viewOptions = new ViewOptions(0);
    addPage(m_viewOptions, i18n("View"), "view-choose", i18n("View Settings"), true);
    setHelp(QString(),"parley");

    m_pluginOptions = new PluginOptions(0);
    addPage(m_pluginOptions, i18n("Plugins"), "view-choose", i18n("Plugin Settings"), true);
    setHelp(QString(),"parley");

    KPluginSelector* m_kps = new KPluginSelector(0);
    addPage(m_kps, i18n("KPluginSelectorTest"), "view-choose", i18n("PluginSelector"), true);
    setHelp(QString(),"parley");

    KPluginInfo * inf = new KPluginInfo("/home/kde-devel/parley-plugins/test.desktop");
    QList<KPluginInfo> L;
     L.push_back(*inf);
    m_kps->addPlugins(L,KPluginSelector::ReadConfigFile,QString("Playlist"),QString("playlist"),KSharedConfig::Ptr());
    kDebug() << QString("Entry Path : ") << inf->entryPath();
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
