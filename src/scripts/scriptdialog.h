/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
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
//    KConfigGroup* scriptsConfigGroup;
    ScriptManager* m_scriptManager;

};

#endif
