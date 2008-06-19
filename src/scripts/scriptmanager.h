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

#include "script.h"

#include <QStringList>

/**
This class finds the scripts installed in the application folder and manages loading and unloading of plugin scripts

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptManager : public QObject
{
    public:
        ScriptManager();

        ~ScriptManager();
        static QStringList getDesktopFiles();
        void loadPlugins();
        QMap<QString, QString> getCategories();
        void update();
        static QString getScriptEntry ( QString desktopFile );
        QString getScriptFileName ( QString desktopFile );
        QStringList getEnabledScripts();
        void disablePlugin ( QString desktopFile );
        void activateEnabledScripts();
        void deactivateDisabledScripts();

    protected:
        QList<Script> m_scripts;
};

#endif
