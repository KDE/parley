//
// C++ Implementation: script
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "script.h"

#include <kross/core/action.h>

/**
 * Script class constructor. Activates the @p script. In case of failure
 * sets the m_activated
 *
 * @param file The path to the script file to be activated (executed)
 */
Script::Script ( QString file )
{
    m_file = file;
    m_activated = false;
    activateScript();
}

/**
 * Script class destructor.
 */
Script::~Script()
{
}

/**
 * Returns if the script was successfully activated. If not, Script object should
 * be destroyed.
 */
bool Script::isActivated()
{
    return m_activated;
}

/**
 * Activates the script
 *
 * @param scriptFilePath The path to the script file to be activated (executed)
 */
void Script::activateScript()
{
    // Create the script container. myobject is the parent QObject,
    // so that our action instance will be destroyed once the myobject
    // is destroyed.

    Kross::Action* action = new Kross::Action ( this, m_file );
    // Publish our myobject instance and connect signals with
    // scripting functions.
//      action->addObject ( myobject, "myobject", Kross::ChildrenInterface::AutoConnectSignals );

    // Set the file we like to execute.
    action->setFile ( m_file );

    // Execute the script.
    action->trigger();

    // Now we emit the init(QTimer*,int) signal which in turn
    // should call our connected init(timer,interval) scripting
    // function if available.
//     myobject->callInit();
    /// @todo Add code to specify if activated or not
}



/**
 * Returns the filename of the script that Script class was created with
 *
 * @return filename The filename of the script (with path)
 */
QString Script::getScriptFileName()
{
    return m_file;
}
