//
// C++ Interface: plugintablemodel
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLUGINTABLEMODEL_H
#define PLUGINTABLEMODEL_H

#include <QAbstractTableModel>

/**
Table model for Plugins table

	@author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class PluginTableModel : public QAbstractTableModel
{
public:
    PluginTableModel(QObject* parent);

    ~PluginTableModel();

};

#endif
