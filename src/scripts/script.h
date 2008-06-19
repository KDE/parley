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

/**
 * This class represents the activated script and is used by the ScriptManager
 * to activate/deactivate scripts. This class is used as the parent object when running
 * the script with Kross and uppon destruction of this class, the script stops running.
 *
 * This class is to be used in this way:
 * @code
 * Script * newScript = new Script(script);
 * if (!newScript.activated())
 *     delete newScript;
 * @endcode
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
        QString getScriptFileName();
        void activateScript();
        void deactivateScript();
        bool scriptExists();
    private:
        bool m_activated;
        QString m_file;
        QObject* m_object;
};

#endif
