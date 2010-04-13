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
#include "scriptdialog.h"

#include "scripting/parley.h"

#include <KLocale>
#include <KPluginInfo>
#include <KSharedConfig>
#include <KConfigGroup>

#include <kross/core/action.h>
#include <kross/core/manager.h>

ScriptDialog::ScriptDialog ( ScriptManager * scriptManager )
        : KDialog()
{
    m_scriptManager = scriptManager;

    //Configure window
    setCaption ( i18n ( "Script Dialog" ) );
    setButtons ( Ok|Cancel );

    //Add KPluginSelector as the main widget of the dialog
    m_kps = new KPluginSelector ( 0 );
    m_kps->setMinimumSize ( 500,500 );

    setMainWidget ( m_kps );

    //Load available plugins
    pluginsInfoList = KPluginInfo::fromFiles ( ScriptManager::getDesktopFiles() );

//     m_kps->addPlugins ( pluginsInfoList,KPluginSelector::ReadConfigFile,i18n ( "Playlist" ),QString ( "playlist" ),KSharedConfig::openConfig ( "parleyrc" ) );
    m_kps->addPlugins ( pluginsInfoList,
                        KPluginSelector::ReadConfigFile,
                        QString(),
                        QString (),
                        KSharedConfig::openConfig ( "parleyrc" ) );
}

ScriptDialog::~ScriptDialog()
{
    delete m_kps;
}

void ScriptDialog::accept()
{
    //Update KPluginInfo object changes
    m_kps->updatePluginsState();   //necessary for KPluginInfo::isPluginEnabled to work

    //Save changes in config file (parleyrc)
    m_kps->save();

    //Reload scripts
    m_scriptManager->reloadScripts();

    //Close dialog
    done ( 0 );
}
