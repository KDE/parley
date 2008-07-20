//
// C++ Implementation: pluginoptionsbase
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pluginoptions.h"

PluginOptions::PluginOptions(QWidget* parent)
 : QWidget()
{
    setupUi(this);
    connect(addButton, SIGNAL(clicked()), this, SLOT(renameRemoveButton()) );
}

void PluginOptions::renameRemoveButton() {
    removeButton->setText("Hi!!");
    return;
}

#include "pluginoptions.moc"
