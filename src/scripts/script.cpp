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

#include <KDebug>

#include <kross/core/action.h>


Script::Script ( QString file )
{
    m_file = file;
    m_activated = false;
}


Script::~Script()
{
    if (m_object)
        delete  m_object;
}


bool Script::isActivated()
{
    return m_activated;
}


void Script::activateScript()
{
    kDebug() << "Activating Script" << fileName();
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
    QMapIterator<QString,QObject*> i ( m_scriptObjects );
    while ( i.hasNext() )
    {
        i.next();
        kDebug() << i.key();
        action->addObject ( i.value() , i.key(), Kross::ChildrenInterface::AutoConnectSignals );
    }

    // Set the file we like to execute.
    action->setFile ( m_file );

    // Execute the script.
    action->trigger();
    /// @todo add error handling somewhere here

    // Now we emit the init(QTimer*,int) signal which in turn
    // should call our connected init(timer,interval) scripting
    // function if available.
//     myobject->callInit();
    m_activated = true;
    /// @todo Add code to specify if activated or not
}


void Script::deactivateScript()
{
    if ( m_object )
        delete m_object;
    m_activated = false;
}


bool Script::scriptExists()
{
    QFileInfo fileInfo ( m_file );
    return fileInfo.exists();
}


QString Script::fileName()
{
    return m_file;
}


void Script::addObject ( QString name, QObject * object )
{
    m_scriptObjects.insert ( name,object );
}


void Script::addObjects ( QMap<QString,QObject*> objects )
{
    m_scriptObjects.unite ( objects );
}
