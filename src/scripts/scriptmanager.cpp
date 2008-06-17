//
// C++ Implementation: scriptmanager
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptmanager.h"
#include <KStandardDirs>
#include <KDebug>
#include <KPluginInfo>

ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}


/**
    Returns a @list of the paths of all the available plugins
    by looking in the parley's plugin folder ({DATA_FOLDER}/plugins)

    @return @list
 */
QStringList ScriptManager::listAvailablePlugins()
{
//     QStringList scriptsAvailable;
    return KGlobal::dirs()->findAllResources ( "appdata", QString ( "plugins/*.desktop" ), KStandardDirs::Recursive/*, scriptsAvailable*/ );
}


/**
 * Reads the state of the plugins from parleyrc (configurarion file) and 
 * loads the enabled ones.
 */
void ScriptManager::loadPlugins()
{

    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles ( ScriptManager::listAvailablePlugins() );
//     KPluginInfo inf;
    foreach ( KPluginInfo inf, pluginsInfoList )
    {
        inf.load ( cfg );
        kDebug() << inf.isPluginEnabled();
    }
}


/**
    @fn ScriptManager::getCategories()

    @brief

    @param c
    @param n

    @exception Famoutin

    @return
 */
QMap<QString, QString> ScriptManager::getCategories()
{
    QMap<QString, QString> categories;
    categories["translation"] = "Translation";
    return categories;
}

