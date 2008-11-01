/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "scriptmanager.h"

#include <KStandardDirs>
#include <KDebug>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <QFileInfo>
#include <KActionCollection>
#include <KMessageBox>

#include <kross/core/action.h>
#include <kross/core/manager.h>

ScriptManager::ScriptManager ( Editor * editor )
        : m_editor ( editor )
{
    //add Scripting::Parley
    m_scriptingParley = new Scripting::Parley ( editor );
    addObject ( m_scriptingParley,"Parley" );
}


ScriptManager::~ScriptManager()
{
}


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


QMap<QString, QString> ScriptManager::categories()
{
    QMap<QString, QString> categories;
    categories["translation"] = "Translation";
    return categories;
}


QString ScriptManager::getScriptEntry ( QString desktopFile )
{
    //open its as a raw configuration file and read the script entry
    KConfig scriptconfig ( desktopFile, KConfig::SimpleConfig );
    KConfigGroup group = scriptconfig.group ( "Desktop Entry" );
    return group.readEntry ( "Script" );
}


QString ScriptManager::getScriptFileName ( QString desktopFile )
{
    QFileInfo desktopFileInfo ( desktopFile );
    return desktopFileInfo.absolutePath() + '/' + ScriptManager::getScriptEntry ( desktopFile );
}


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


void ScriptManager::loadScripts()
{
    QStringList scripts = enabledScripts();
    foreach ( const QString& script, scripts )
    {
        //create a new Script and add it to the m_scripts list
        Script * s = new Script ( script );
        s->addObjects ( m_scriptObjects );
        s->activate();
        m_scripts.push_back ( s );
        //inform with a message box when a script could not be activated
        if ( !s->isActivated() )
            KMessageBox::information ( m_editor, i18n ( "The following script could not be activated due to errors in the script:\n" ) +script,"Script Activation" );
    }
}


void ScriptManager::addObject ( QObject * obj, const QString & name )
{
    m_scriptObjects[name] = obj;
}


void ScriptManager::reloadScripts()
{
    //deactivate (delete) all the active scripts
    foreach ( Script * s, m_scripts )
    {
        if ( s ) delete s;
    }
    m_scripts.clear();

    //reload the scripts menu
    m_editor->unplugActionList ( "scripts_actionlist" );
    m_scriptActions.clear();
    m_editor->plugActionList ( "scripts_actionlist",m_scriptActions );

    //load all the enabled scripts
    loadScripts();
}


void ScriptManager::addScriptAction ( const QString & name, KAction * action )
{
    //unplug action list (orelse it will add twice the same entries
    m_editor->unplugActionList ( "scripts_actionlist" );

    //add to action collection
    m_editor->actionCollection()->addAction ( name,action );

    //add it to actions menu list
    m_editor->m_scriptManager->m_scriptActions.push_back ( action );

    //plug the action list
    m_editor->plugActionList ( "scripts_actionlist",m_scriptActions );

}
