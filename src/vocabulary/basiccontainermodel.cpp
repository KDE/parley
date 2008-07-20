/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "containermodel.h"

#include "containermimedata.h"
#include "vocabularymimedata.h"

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

#include <KDebug>
#include <KLocalizedString>

/** @file
  * Implementation of BasicContainerModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */

BasicContainerModel::BasicContainerModel(KEduVocContainer::EnumContainerType type, QObject * parent) : QAbstractItemModel(parent)
{
    m_type = type;
    m_doc = 0;
}

void BasicContainerModel::setDocument(KEduVocDocument * doc)
{
    m_doc=doc;
    reset();
}

QModelIndex BasicContainerModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_doc || !hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    KEduVocContainer *parentContainer;

    if (!parent.isValid()) {
        parentContainer = 0;
    } else {
        parentContainer = static_cast<KEduVocContainer*>(parent.internalPointer());
    }

    KEduVocContainer *childContainer;
    if (!parentContainer) {
        childContainer = rootContainer();
    } else {
        childContainer = parentContainer->childContainer(row);
    }
    return createIndex(row, column, childContainer);
}

QModelIndex BasicContainerModel::index(KEduVocContainer * container) const
{
    if(!container) {
        return QModelIndex();
    }

    QModelIndex currentIndex = index(container->row(), 0, index(container->parent()));
    Q_ASSERT(container == currentIndex.internalPointer());

    return currentIndex;
}

QModelIndex BasicContainerModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    KEduVocContainer *childItem = static_cast<KEduVocContainer*>(index.internalPointer());
    if (!childItem) {
        return QModelIndex();
    }

    KEduVocContainer *parentItem = childItem->parent();

    if (!parentItem) {
        return QModelIndex();
    }

    QModelIndex parentIndex = createIndex(parentItem->row(), 0, parentItem);
    return parentIndex;
}

int BasicContainerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    KEduVocContainer *parentItem;
    if (!parent.isValid()) {
        // root element
        if (!m_doc) {
            return 0;
        }
        return 1;
    } else {
        parentItem =  static_cast<KEduVocContainer*>(parent.internalPointer());
        return parentItem->childContainerCount();
    }
}

QVariant BasicContainerModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
         return QVariant();
    }

    KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());

    switch (index.column()){
    case 0: // Container name
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (index.parent() == QModelIndex()) {
                return i18n("None");
            }
            return container->name();
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags BasicContainerModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        // the root element, not editable for now
        if (index.parent() == QModelIndex()) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        // the name column
        if ( index.column() == 0 ) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        } else { // every other element
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
    return  0;
}

int BasicContainerModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (!m_doc) {
        return 1;
    }

    return 1;
}

KEduVocContainer::EnumContainerType BasicContainerModel::containerType()
{
    return m_type;
}


#include "basiccontainermodel.moc"
