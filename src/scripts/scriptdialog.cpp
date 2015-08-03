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

#include <keduvoctranslator.h>
#include <keduvocscriptmanager.h>

#include <KLocalizedString>
#include <KPluginInfo>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <kross/core/action.h>
#include <kross/core/manager.h>

ScriptDialog::ScriptDialog( KEduVocScriptManager * scriptManager )
    : QDialog()
{
    m_scriptManager = scriptManager;

    //Configure window
    setWindowTitle(i18n("Script Dialog"));

    QDialogButtonBox * button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    //Add KPluginSelector as the main widget of the dialog
    m_kps = new KPluginSelector(0);
    m_kps->setMinimumSize(500, 500);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( m_kps );
    layout->addWidget( button_dialog );

    setLayout( layout );

    connect(button_dialog, &QDialogButtonBox::accepted, this, &ScriptDialog::accept);
    connect(button_dialog, &QDialogButtonBox::rejected, this, &ScriptDialog::reject);

    //Load available plugins
    pluginsInfoList = KPluginInfo::fromFiles( KEduVocScriptManager::getDesktopFiles() );

//     m_kps->addPlugins ( pluginsInfoList,KPluginSelector::ReadConfigFile,i18n ( "Playlist" ),QString ( "playlist" ),KSharedConfig::openConfig ( "parleyrc" ) );
    ///@todo frameworks scripts dialog is not finding any scripts
    m_kps->addPlugins(pluginsInfoList,
                      KPluginSelector::ReadConfigFile,
                      QString(),
                      QString(),
                      KSharedConfig::openConfig("parleyrc"));
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
    done(0);
}
