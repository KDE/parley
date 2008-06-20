//
// C++ Interface: scriptdialog
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include "scriptmanager.h"

#include <KDialog>
#include <KPluginSelector>

/**
    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptDialog : public KDialog
{
    public:
        ScriptDialog(ScriptManager* scriptManager);
        ~ScriptDialog();
        void accept();

    private:
        KPluginSelector* m_kps;
        QList<KPluginInfo> pluginsInfoList;
        KConfigGroup* scriptsConfigGroup;
        ScriptManager* m_scriptManager;

};

#endif
