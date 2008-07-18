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
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "script.h"

#include "../parley.h"

#include <QStringList>

/**
This class finds the scripts installed in the application folder and manages loading and unloading of plugin scripts

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptManager : public QObject
{
    public:
        ScriptManager(ParleyApp * parleyApp);

        ~ScriptManager();
        static QStringList getDesktopFiles();
        static QMap<QString, QString> categories();
        static QString getScriptEntry ( QString desktopFile );
        QString getScriptFileName ( QString desktopFile );
        QStringList enabledScripts();
        void disablePlugin ( QString desktopFile );
        QStringList availableScripts();
        void loadScripts();
        void addObject ( QObject * obj, const QString & name );
        void reloadScripts();
        void addScriptAction(const QString & name, KAction * action);

    private:
        ParleyApp * m_parleyApp;
        QList<Script*> m_scripts;
        QMap<QString, QObject*> m_scriptObjects;
        QList<QAction*> m_scriptActions;

//         friend class Scripting::Parley;
};

#endif
