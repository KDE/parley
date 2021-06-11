/*
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "readonlycontainermodel.h"

#include <KLocalizedString>
#include <QDebug>

#include "containermimedata.h"
#include "vocabularymimedata.h"

#include <KEduVocDocument>
#include <KEduVocWordtype>
#include <KEduVocExpression>


/** @file
  * Implementation of ReadonlyContainerModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */


ReadonlyContainerModel::ReadonlyContainerModel(KEduVocContainer::EnumContainerType type, QObject * parent)
    : QAbstractItemModel(parent)
    , m_type(type)
{
}

void ReadonlyContainerModel::setDocument(const std::shared_ptr<KEduVocDocument> &doc)
{
    beginResetModel();
    m_doc = doc;
    if (m_doc) {
	//qDebug() << "Set Document: " << m_doc->url();
    } else {
        //qDebug() << "Set Invalid Document";
    }
    endResetModel();
}

QModelIndex ReadonlyContainerModel::index(int row, int column, const QModelIndex &parent) const
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

    KEduVocContainer *childContainer = 0;
    if (!parentContainer) {
        childContainer = rootContainer();
        if (!childContainer) {
            return QModelIndex();
        }
    } else {
        childContainer = parentContainer->childContainer(row);
    }
    return createIndex(row, column, childContainer);
}

QModelIndex ReadonlyContainerModel::index(KEduVocContainer * container) const
{
    if (!container) {
        return QModelIndex();
    }

    QModelIndex currentIndex = index(container->row(), 0, index(container->parent()));
    Q_ASSERT(container == currentIndex.internalPointer());

    return currentIndex;
}

QModelIndex ReadonlyContainerModel::parent(const QModelIndex &index) const
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

int ReadonlyContainerModel::rowCount(const QModelIndex &parent) const
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

QVariant ReadonlyContainerModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());

    switch (index.column()) {
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

Qt::ItemFlags ReadonlyContainerModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        // the root element, not editable for now
        if (index.parent() == QModelIndex()) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        // the name column
        if (index.column() == 0) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        } else { // every other element
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
    return Qt::NoItemFlags;
}

int ReadonlyContainerModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (!m_doc) {
        return 1;
    }

    return 1;
}

KEduVocContainer::EnumContainerType ReadonlyContainerModel::containerType()
{
    return m_type;
}
