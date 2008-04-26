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

#include "statisticsmodel.h"

#include <KLocalizedString>
#include <QGradient>

StatisticsModel::StatisticsModel(QObject * parent)
    :ContainerModel(KEduVocContainer::Lesson, parent)
{
    setSupportedDragActions(0);
}

QVariant StatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= 2) {
        if(role == Qt::DisplayRole) {
            return i18nc("Grade in language, table header", "Grade (%1)", m_doc->identifier(section-2).name());
        }
    }
    return ContainerModel::headerData(section, orientation, role);
}

QVariant StatisticsModel::data(const QModelIndex & index, int role) const
{
    if (index.column() >= 2) {
        KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());
        switch (role) {
            case TotalPercent: // Average grade
                return container->averageGrade(index.column()-2);
            case TotalCount:
                return container->entryCount(KEduVocContainer::NotRecursive);
        }
    }
    return ContainerModel::data(index, role);
}

Qt::ItemFlags StatisticsModel::flags(const QModelIndex & index) const
{
    if (index.isValid()) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    return 0;
}

int StatisticsModel::columnCount(const QModelIndex & parent) const
{
    return m_doc->identifierCount() + 2;
}

#include "statisticsmodel.moc"


