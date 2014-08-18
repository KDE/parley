/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include "vocabulary/containermodel.h"

using namespace Editor;

class StatisticsModel : public ContainerModel
{
    Q_OBJECT

public:
    enum GradeRoles {
        TotalPercent = Qt::UserRole,
        TotalCount,
        Grade0,
        Grade1,
        Grade2,
        Grade3,
        Grade4,
        Grade5,
        Grade6,
        Grade7,
        AllFractions
    };

    explicit StatisticsModel(QObject *parent = 0);

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    KEduVocContainer * rootContainer() const;
};


#endif
