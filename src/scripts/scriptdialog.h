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
#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include "scriptmanager.h"

#include <QDialog>
#include <KPluginSelector>

/**
    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptDialog : public QDialog
{
public:
    explicit ScriptDialog(ScriptManager* scriptManager);
    ~ScriptDialog();
    /**
     * Executed when user clicks OK button.
     * Saves the state of the plugins (which ones are loaded) in the config file
     * and makes the necessary loads/unloads of plugins
     */
    void accept() Q_DECL_OVERRIDE;

private:
    KPluginSelector* m_kps;
    QList<KPluginInfo> pluginsInfoList;
    KConfigGroup* scriptsConfigGroup;
    ScriptManager* m_scriptManager;

};

#endif
