/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "script.h"

#include <QDebug>
#include <KLocalizedString>

#include <kross/core/action.h>


Script::Script(const QString &file)
{
    m_file = file;
    m_activated = false;
}


Script::~Script()
{
    delete  m_object;
}


bool Script::isActivated()
{
    return m_activated;
}


void Script::activate()
{
    qDebug() << "Activating Script" << fileName();
    if (isActivated()) {
        qDebug() << "Script already activated";
        return;
    }
    if (!exists()) {
        qDebug() << "Script file given does not exist";
        m_errorMessage = i18n("The script file does not exist.");
        return;
    }

    // Create the script container. m_object is the parent QObject,
    // so that our action instance will be destroyed once the m_object
    // is destroyed.
    m_object = new QObject();
    Kross::Action* action = new Kross::Action(m_object, m_file);
    // Publish our myobject instance and connect signals with
    // scripting functions.
    QMapIterator<QString, QObject*> i(m_scriptObjects);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key();
        action->addObject(i.value() , i.key(), Kross::ChildrenInterface::AutoConnectSignals);
    }

    // Set the file to be execute
    action->setFile(m_file);

    // Execute the script.
    action->trigger();

    m_activated = !action->isFinalized();
    if (!m_activated) {
        qDebug() << "Script not activated";
        QString msg = action->errorMessage();
        QString trace = action->errorTrace();
        msg.replace('<', QLatin1String("&lt;")).replace('\n', QLatin1String("<br/>"));
        trace.replace('<', QLatin1String("&lt;")).replace('\n', QLatin1String("<br/>"));
        m_errorMessage = "<p><strong>" + i18n("Error in file %1 at line %2:", fileName(),
                                              action->errorLineNo()) + "</strong><br/>" + msg + "<br/><strong>"
                         + i18nc("debug information in error message", "Backtrace:") +
                         "</strong><br/>" + trace + "</p>";
    }
}


void Script::deactivate()
{
    delete m_object;
    m_activated = false;
}


bool Script::exists()
{
    QFileInfo fileInfo(m_file);
    return fileInfo.exists();
}


QString Script::fileName()
{
    return m_file;
}


void Script::addObject(const QString &name, QObject * object)
{
    m_scriptObjects.insert(name, object);
}


void Script::addObjects(const QMap<QString, QObject*> &objects)
{
    m_scriptObjects.unite(objects);
}

QString Script::errorMessage()
{
    return m_errorMessage;
}
