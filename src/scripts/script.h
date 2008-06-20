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

#include "scriptobjectparley.h"

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
        Script ( QString file );

        ~Script();
        bool isActivated();
        void activateScript();
        void deactivateScript();
        bool scriptExists();
        QString fileName();
        void addObject ( QString name, QObject * object );
        void addObjects ( QMap<QString, QObject*> objects );
    private:
        bool m_activated;
        QString m_file;
        QPointer<QObject> m_object;
        QMap<QString, QObject*> m_scriptObjects;
};

#endif
