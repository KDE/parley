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
#include <QFileInfo>

#include <kross/core/action.h>

ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}


/**
 * Finds all the available desktop files in {PARLEY_DATA_FOLDER}/plugins
 *
 * @return The list of desktop filenames available for parley
 */
QStringList ScriptManager::getDesktopFiles()
{
//     QStringList scriptsAvailable;
    return KGlobal::dirs()->findAllResources (
               "appdata",
               QString ( "plugins/*.desktop" ),
               KStandardDirs::Recursive
               /*, scriptsAvailable*/
           );
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
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles ( getDesktopFiles() );
//     KPluginInfo inf;
    foreach ( KPluginInfo inf, pluginsInfoList )
    {
        inf.load ( cfg );
//         kDebug() << inf.name() << inf.isPluginEnabled() << inf.property ( "X-KDE-PluginInfo-Script" );
        kDebug() << inf.name() << inf.isPluginEnabled() << inf.pluginName();
    }
// Done in the dirty way.
    QStringList files = ScriptManager::getDesktopFiles();
    foreach ( QString filePath, files )
    {
        kDebug() << this->getScriptEntry ( filePath );
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

/**
 * Returns a QMap (from from categories codenames to categories display label)
 * to be used in KPluginSelector (ScriptDialog) for displaying the various
 * categories
 *
 * @return the QMap described above
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


/**
 * Parses the desktop @p desktopFile given and returns the value of "Script" entry.
 *
 * @param desktopFile The .desktop file that will get the value from
 * @return The value of "Script" entry. Empty string of no "Script" entry is found
 */
QString ScriptManager::getScriptEntry ( QString desktopFile )
{
    //open its as a raw configuration file and read the script entry
    KConfig scriptconfig ( desktopFile, KConfig::SimpleConfig );
    KConfigGroup group = scriptconfig.group ( "Desktop Entry" );
    return group.readEntry ( "Script" );
}


/**
 * Returns the full path to the script name given in the @p desktopFile.
 *
 * @param desktopFile The desktop file for the parley plugin
 * @return The full-path to the script
 */
QString ScriptManager::getScriptFileName ( QString desktopFile )
{
    QFileInfo desktopFileInfo ( desktopFile );
    return desktopFileInfo.absolutePath() + "/" + ScriptManager::getScriptEntry ( desktopFile );
}


/**
 *
 * @return List of filenames of enalbed scripts
 */
QStringList ScriptManager::getEnabledScripts()
{
    QStringList enabledScripts;
    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles ( getDesktopFiles() );
    KPluginInfo inf;
    foreach ( inf, pluginsInfoList )
    {
        inf.load ( cfg );
        if ( inf.isPluginEnabled() )
            enabledScripts.push_back ( getScriptFileName ( inf.entryPath() ) );
//         kDebug() << inf.name() << inf.isPluginEnabled() << inf.pluginName();
    }
    return enabledScripts;
}


/**
 * Modify the parleyrc configuration so it disables the @p dektopFile plugin.
 * This function is to be used when the plugin is invalid (wrong script name,
 * incorrect desktop file etc)
 *
 * @param desktopFile 
 */
void ScriptManager::disablePlugin(QString desktopFile)
{
    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    KPluginInfo inf(desktopFile);
    //load parleyrc enabled value
    inf.load(cfg);
    inf.setPluginEnabled(false);
    //save enabled=true in parleyrc
    inf.save(cfg);
}


/**
 * Activates all the enabled scripts (if not already activated)
 */
void ScriptManager::activateEnabledScripts()
{
    /// @todo implement me
}

/**
 * Deactivates all disabled scripts (if not already deactivated)
 */
void ScriptManager::deactivateDisabledScripts()
{
    /// @todo implement me
}
