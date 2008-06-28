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

#include "leitnermodel.h"

#include <keduvocleitnerbox.h>

#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

LeitnerModel::LeitnerModel(QObject * parent)
    :ContainerModel(KEduVocContainer::Leitner, parent)
{
    setSupportedDragActions(0);
}

QVariant LeitnerModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::CheckStateRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        if (role == Qt::DecorationRole) {
            return KIcon("parley");
        }
    }
    return ContainerModel::data(index, role);
}

Qt::ItemFlags LeitnerModel::flags(const QModelIndex & index) const
{
    if (index.isValid()) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    return 0;
}

KEduVocContainer * LeitnerModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }

    return m_doc->leitnerContainer();
}

#include "leitnermodel.moc"


