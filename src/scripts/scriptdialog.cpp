//
// C++ Implementation: scriptdialog
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptdialog.h"

#include <KLocale>
#include <KPluginSelector>

ScriptDialog::ScriptDialog()
 : KDialog()
{
    setCaption(i18n("Script Dialog"));
//     setFaceType( List );
    setButtons(Ok|Cancel);

    KPluginSelector* m_kps = new KPluginSelector(0);
    setMainWidget(m_kps);
/*
    addPage(m_kps, i18n("Scripts"), "view-choose", i18n("PluginSelector"), true);
    setHelp(QString(),"parley");

    KPluginInfo * inf = new KPluginInfo("/home/kde-devel/parley-plugins/test.desktop");
    QList<KPluginInfo> L;
     L.push_back(*inf);
    m_kps->addPlugins(L,KPluginSelector::ReadConfigFile,QString("Playlist"),QString("playlist"),KSharedConfig::Ptr());
    kDebug() << QString("Entry Path : ") << inf->entryPath();*/
}


ScriptDialog::~ScriptDialog()
{
}


