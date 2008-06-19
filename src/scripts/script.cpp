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
#include <KDebug>

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
}

/**
 * Script class destructor.
 */
Script::~Script()
{
    delete m_object;
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
    if ( isActivated() )
    {
        kDebug() << "Script already activated";
        return;
    }
    if ( !scriptExists() )
    {
        kDebug() << "Script file given does not exist";
        return;
    }
    
    // Create the script container. m_object is the parent QObject,
    // so that our action instance will be destroyed once the m_object
    // is destroyed.
    m_object = new QObject();
    Kross::Action* action = new Kross::Action ( m_object, m_file );
    // Publish our myobject instance and connect signals with
    // scripting functions.
    ScriptObjectParley * obj_parley = new ScriptObjectParley(); /// @todo delete it at the end of the class (to avoid memory leaks)
    action->addObject ( obj_parley , "Parley", Kross::ChildrenInterface::AutoConnectSignals );

    // Set the file we like to execute.
    action->setFile ( m_file );

    // Execute the script.
    action->trigger();

    // Now we emit the init(QTimer*,int) signal which in turn
    // should call our connected init(timer,interval) scripting
    // function if available.
//     myobject->callInit();
    m_activated = true;
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


/**
 * Deactivate the running script
 */
void Script::deactivateScript()
{
    if ( m_object )
        delete m_object;
    m_activated = false;
}


/**
 * Checks if the script file assigned to the Script object exists as a file on the
 * given path.
 *
 * @return True if the script file exists
 *         False if it does not exist
 */
bool Script::scriptExists()
{
    QFileInfo fileInfo ( m_file );
    return fileInfo.exists();
}


/**
 * Returns the file that was given as parameter to the constructor
 * @return
 */
QString Script::fileName()
{
    return m_file;
}
