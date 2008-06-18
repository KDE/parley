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
#include <KServiceTypeTrader>
#include <kross/core/action.h>

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
void ScriptManager::loadScript(QString filename)
{
    Kross::Action action(this,"MyJSScript");
    action.setFile(filename);
    action.trigger();
//     QVariantList args;
//     args << "hello";
//     QVariant script_result = action.callFunction("fetchTranslation",args);
//     kDebug() << script_result;
}

/**
 * Reads the state of the plugins from parleyrc (configurarion file) and
 * loads the enabled ones.
 */
void ScriptManager::loadPlugins()
{
//     KService::List services;
//     KServiceTypeTrader* trader = KServiceTypeTrader::self();
// 
//     services = trader->query ( "Sonnet/SpellClient" );
// 
//     foreach ( KService::Ptr service, services )
//     {
//         kDebug() << "read write part" << service->name();
//     }
// 
//     services = trader->defaultOffers ( "ThumbCreator" );
//     if ( services.isEmpty() )
//     {
//         kDebug() << "no services found for ThumbCreator!";
//     }
// 
//     KService::Ptr service = trader->preferredService ( "Sonnet/SpellClient" );
//     if ( !service )
//     {
//         kDebug() << "no preferred service found for Parley/Script";
//     }

// ---------------------------

    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles ( ScriptManager::listAvailablePlugins() );
//     KPluginInfo inf;
    foreach ( KPluginInfo inf, pluginsInfoList )
    {
        inf.load ( cfg );
//         kDebug() << inf.name() << inf.isPluginEnabled() << inf.property ( "X-KDE-PluginInfo-Script" );
        kDebug() << inf.name() << inf.isPluginEnabled() << inf.pluginName();
    }

// Done in the dirty way.
    QStringList files = ScriptManager::listAvailablePlugins();
    foreach ( QString filePath, files) {
        KConfig scriptconfig(filePath, KConfig::SimpleConfig);
        KConfigGroup group = scriptconfig.group("Desktop Entry");
        kDebug() << group.readEntry("Script");
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



/**
 * This function makes sure that the saved status (enabled/disabled) in the configuration files
 * reflects the actual status of those scripts. So if any script was enabled/disabled in the
 * configuration files, it enables/disables it.
 *
 * This function is to be used by the ScriptDialog to enable/disable scripts.
 */
void ScriptManager::update()
{
    /// @todo implement me
}
