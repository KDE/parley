//
// C++ Interface: script
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPT_H
#define SCRIPT_H

#include "scripting/parley.h"

#include <QString>
#include <QObject>
#include <QPointer>
#include <QMap>

/**
 * This class represents the activated script and is used by the ScriptManager
 * to activate/deactivate scripts.
 *
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Script: public QObject
{
        Q_OBJECT
    public:
        /**
         * Script class constructor. Activates the @p file. In case of failure
         * sets the m_activated
         * @param file The path to the script file to be activated (executed)
         */
        Script ( QString file );

        /**
         * Deletes the m_object that was dynam
         */
        ~Script();
        /**
         * Returns if the script was successfully activated. If not, Script object should
         * be destroyed.
         */
        bool isActivated();
        /**
         * Activates the script
         *
         * @param scriptFilePath The path to the script file to be activated (executed)
         */
        void activateScript();
        /**
         * Deactivate the running script
         */
        void deactivateScript();
        /**
         * Checks if the script file assigned to the Script object exists as a file on the
         * given path.
         *
         * @return True if the script file exists
         *         False if it does not exist
        */
        bool scriptExists();
        /**
         * Returns the file that was given as parameter to the constructor
         */
        QString fileName();
        /**
         * Add an object to be accessible by the script
         * @param name
         * @param object Object to be accessible by the script
         */
        void addObject ( QString name, QObject * object );
        /**
         *
         * @param objects Map of the objects to add
         */
        void addObjects ( QMap<QString, QObject*> objects );
    private:
        bool m_activated;
        QString m_file;
        QPointer<QObject> m_object;
        QMap<QString, QObject*> m_scriptObjects;
};

#endif
