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

#ifndef CONTAINERMODEL_H
#define CONTAINERMODEL_H

#include "readonlycontainermodel.h"

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

namespace Editor
{
/**
  * Model for the tree of containers (lessons, word types).
  */
class ContainerModel : public ReadonlyContainerModel
{
    Q_OBJECT

public:
    enum ColumnType {
        ContainerNameColumn = 0,
        TotalCountColumn,
        FirstDataColumn
    };

    explicit ContainerModel(KEduVocContainer::EnumContainerType type, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual Qt::DropActions supportedDropActions() const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData * mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);

    /** Change the name or checkbox of a lesson.
     * @param index which lesson
     * @param value new name
     * @param role
     * @return bool @c true it worked */
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);


    QModelIndex appendContainer(const QModelIndex& parent, const QString & containerName = QString());

    void deleteContainer(const QModelIndex& containerIndex);

signals:
    /**
     * emitted when the inPractice state or name of a lesson changed.
     */
    void documentModified();
};

}

#endif
