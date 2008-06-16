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

ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}


/*!
    \fn ScriptManager::listAvailablePlugins()
 */
QStringList ScriptManager::listAvailablePlugins()
{
    QStringList scriptsAvailable;
    QStringList files = KGlobal::dirs()->findAllResources("appdata", QString("plugins/*.desktop"), KStandardDirs::Recursive, scriptsAvailable);
    return files;
}


/*!
    \fn ScriptManager::loadPlugins()
 */
void ScriptManager::loadPlugins()
{
    /// @todo implement me
}
