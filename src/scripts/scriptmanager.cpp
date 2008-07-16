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
// #include "scriptobjectparley.h"

#include <KStandardDirs>
#include <KDebug>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <QFileInfo>
#include <KActionCollection>

#include <kross/core/action.h>
#include <kross/core/manager.h>

ScriptManager::ScriptManager(ParleyApp * parleyApp)
        : m_parleyApp(parleyApp)
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
 * Returns a QMap (from from categories codenames to categories display label)
 * to be used in KPluginSelector (ScriptDialog) for displaying the various
 * categories
 *
 * @return the QMap described above
 */
QMap<QString, QString> ScriptManager::categories()
{
    QMap<QString, QString> categories;
    categories["translation"] = "Translation";
    return categories;
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
QStringList ScriptManager::enabledScripts()
{
    QStringList enabledScripts;
    // Open parleyrc to read the state of the plugins (enabled/disabled)
    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    // Get list of KPluginInfo for each of the desktop files found
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles ( getDesktopFiles() );
    // Find which plugins are enabled and add them to enabledScripts list
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
void ScriptManager::disablePlugin ( QString desktopFile )
{
    KConfigGroup cfg ( KSharedConfig::openConfig ( "parleyrc" ),"Plugins" );
    KPluginInfo inf ( desktopFile );
    //load parleyrc enabled value
    inf.load ( cfg );
    inf.setPluginEnabled ( false );
    //save enabled=true in parleyrc
    inf.save ( cfg );
}


/**
 * Load all the available scripts so they can be activated afterwards
 */
void ScriptManager::loadScripts()
{
    QStringList scripts = enabledScripts();
    foreach ( QString script, scripts )
    {
        //create a new Script and add it to the m_scripts list
        Script * s = new Script ( script );
        s->addObjects ( m_scriptObjects );
        s->activateScript();
        m_scripts.push_back ( s );
    }
}


/**
 * Adds a QObject as a module for the script
 * @param obj The QObject to be added to the script
 * @param name The name of the object as it will appear in the script
 */
void ScriptManager::addObject ( QObject * obj, const QString & name )
{
    m_scriptObjects[name] = obj;
}


/**
 * Reload all the scripts
 */
void ScriptManager::reloadScripts()
{
    //deactivate (delete) all the active scripts
    foreach ( Script * s, m_scripts )
    if ( s ) delete s;
    m_scripts.clear();

    //reload the scripts menu
    m_parleyApp->unplugActionList("scripts_actionlist");
    m_scriptActions.clear();
    m_parleyApp->plugActionList("scripts_actionlist",m_scriptActions);

    //load all the enabled scripts
    loadScripts();
}


/**
 *
 */
void ScriptManager::addScriptAction (const QString & name, KAction * action )
{
    //unplug action list (orelse it will add twice the same entries
    m_parleyApp->unplugActionList ( "scripts_actionlist" );
    
    //add to action collection
    m_parleyApp->actionCollection()->addAction ( name,action );
    //add it to actions menu list
    m_parleyApp->m_scriptManager->m_scriptActions.push_back ( action );
//         m_scriptActions.push_back(action);
    //plug the action list
    m_parleyApp->plugActionList ( "scripts_actionlist",m_scriptActions );

}
