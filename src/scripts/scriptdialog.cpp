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

    pluginsInfoList = KPluginInfo::fromFiles(sm.listAvailablePlugins());
    m_kps->addPlugins(pluginsInfoList,KPluginSelector::ReadConfigFile,QString("Playlist"),QString("playlist"),KSharedConfig::openConfig("parleyrc"));

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

     foreach(KPluginInfo inf, pluginsInfoList) {
        kDebug() << QString("Is enabled? : ") << inf.pluginName() << inf.property("PluginName") << inf.isPluginEnabled();
    }
    
    m_kps->save();
    done(0); //not sure if I put the right return code
}
