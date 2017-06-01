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

#ifndef READONLYCONTAINERMODEL_H
#define READONLYCONTAINERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

/**
  * Model for the tree of containers (lessons, word types).
  * ReadonlyContainerModel is a read only tree model.
  * @see ContainerModel for its subclass that includes more options.
  */

class ReadonlyContainerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ReadonlyContainerModel(KEduVocContainer::EnumContainerType type, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QModelIndex index(KEduVocContainer* container) const;

    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    KEduVocContainer::EnumContainerType containerType();

public slots:
    /** Set the new source kvtml file
     * @param doc the new file */
    void setDocument(KEduVocDocument *doc);

protected:
    virtual KEduVocContainer *rootContainer() const = 0;
    KEduVocContainer::EnumContainerType m_type;
    KEduVocDocument *m_doc;
};


#endif
