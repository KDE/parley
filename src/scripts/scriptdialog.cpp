//
// C++ Implementation: scriptdialog
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptdialog.h"

#include "scriptmanager.h"

#include <KLocale>
#include <KPluginInfo>
#include <KSharedConfig>
#include <KStandardDirs>
#include <KConfigGroup>
#include <KConfig>

#include <KDebug>


ScriptDialog::ScriptDialog()
 : KDialog()
{
    ScriptManager sm;
    kDebug() << sm.listAvailablePlugins();

    setCaption(i18n("Script Dialog"));
//     setFaceType( List );
    setButtons(Ok|Cancel);

    m_kps = new KPluginSelector(0);
    setMainWidget(m_kps);

//     kDebug() << KStandardDirs::locate("config","parleyrc");

    /* there is a slight problem here cause KSharedConfig doesn't write in a specific category
        in the config file. It writes in a top-level entry .. no the best behaviour
        Suggestion: create scriptrc or find a way to do this.
    */

//     QString configFile("parleyscriptsrc");

//      KConfig parleyScriptConfig(configFile);
//      KConfigGroup scriptsConfigGroup(&parleyScriptConfig,"Scripts");
//     if (!scriptsConfigGroup.exists()) {
//         scriptsConfigGroup.writeEntry("create","create");
//     }

//     KSharedConfig::Ptr config = KSharedConfig::openConfig(KStandardDirs::locate("config",configFile));
//     KSharedConfig::Ptr config = KSharedConfig::openConfig(KStandardDirs::locate("config","parleyrc"));
      KSharedConfig::Ptr config = KSharedConfig::openConfig("parleyrc");
    
//     KSharedConfig::Ptr config = KSharedConfig::openConfig(parleyScriptConfig);

//     kDebug() << config;

//     KConfigGroup * scripts_config_group = new KConfigGroup(config,"scripts");
     KConfig parleyConfig("parleyrc");
//     kDebug() << parleyConfig.groupList();
     scriptsConfigGroup = new KConfigGroup(&parleyConfig,"Plugins");
    
//     if (!scriptsConfigGroup.exists()) {
//         scriptsConfigGroup.writeEntry("create","configgroup");
//     }

//     inf->setConfig(scriptsConfigGroup);
//     inf->load();
//     kDebug() << inf->isPluginEnabled();
    pluginsInfoList = KPluginInfo::fromFiles(sm.listAvailablePlugins()/*,scriptsConfigGroup*/);
//     pluginsInfoList.push_back(inf);
//     pluginsInfoList.push_back(inf2);
    m_kps->addPlugins(pluginsInfoList,KPluginSelector::ReadConfigFile,QString("Playlist"),QString("playlist"),config);


//     m_kps->load();
//     m_kps->updatePluginsState();

//  *** This way just adds "Enabled = True" under the scripts category
//     foreach(KPluginInfo inf, pluginsInfoList) {
//         kDebug() << QString("Is valid? : ") << inf.isValid();
//         inf.save(scriptsConfigGroup);
//     }
}


ScriptDialog::~ScriptDialog()
{
    delete m_kps;
}


/*!
    Executed when user clicks OK button.
    Saves the state of the scripts (which ones are loaded) in the config file

    \fn ScriptDialog::accept()
 */
void ScriptDialog::accept()
{
    m_kps->updatePluginsState();
//     scriptsConfigGroup->dele
//     foreach(KPluginInfo inf, pluginsInfoList) {
    for (int i = 0; i < pluginsInfoList.size(); i++) {
        kDebug() << QString("Is enabled? : ") << pluginsInfoList[i].pluginName() << pluginsInfoList[i].property("PluginName") << pluginsInfoList[i].isPluginEnabled();
    }
//     KPluginInfo i; i.property("PluginName");
    
    m_kps->save();
    done(0); //not sure if I put the right return code
}
