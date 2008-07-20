//
// C++ Interface: pluginoptionsbase
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLUGINOPTIONS_H
#define PLUGINOPTIONS_H

#include "ui_pluginoptionsbase.h"

/**
	@author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class PluginOptions : public QWidget, public Ui::PluginOptionsBase
{
    Q_OBJECT
public:
    PluginOptions(QWidget* parent = 0);
public slots:
    void renameRemoveButton();

};

#endif
