//
// C++ Implementation: kvttablemodel
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kvttablemodel.h"

KVTTableModel::KVTTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

/*!
    \fn KVTTableModel::setDocument(KEduVocDocument * doc)
 */
void KVTTableModel::setDocument(KEduVocDocument * doc)
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::rowCount(const QModelIndex &parent) const
 */
int KVTTableModel::rowCount(const QModelIndex &parent) const
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::columnCount(const QModelIndex &parent) const
 */
int KVTTableModel::columnCount(const QModelIndex &parent) const
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::data(const QModelIndex &index, int role) const
 */
QVariant KVTTableModel::data(const QModelIndex &index, int role) const
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
 */
QVariant KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::flags(const QModelIndex &index) const
 */
Qt::ItemFlags KVTTableModel::flags(const QModelIndex &index) const
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
 */
bool KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    /// @todo implement me
}


/*!
    \fn KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
 */
bool KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    /// @todo implement me
}

#include "kvttablemodel.moc"
