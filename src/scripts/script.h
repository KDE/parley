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
class Script{
public:
    Script();

    ~Script();
    bool isActivated();
private:
    bool m_activated;
};

#endif
