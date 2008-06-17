//
// C++ Interface: scriptmanager
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QStringList>

/**
This class finds the scripts installed in the application folder and manages loading and unloading of plugin scripts

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptManager
{
    public:
        ScriptManager();

        ~ScriptManager();
        static QStringList listAvailablePlugins();
        void loadPlugins();
        QMap<QString, QString> getCategories();

};

#endif
