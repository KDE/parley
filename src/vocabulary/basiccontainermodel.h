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

#ifndef BASICCONTAINERMODEL_H
#define BASICCONTAINERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

/**
  * Model for the tree of containers (lessons, word types).
  * BasicContainerModel is a read only tree model.
  * @see ContainerModel for its subclass that includes more options.
  */
namespace Editor {
    
class BasicContainerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BasicContainerModel(KEduVocContainer::EnumContainerType type, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(KEduVocContainer* container) const;

    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

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

}

#endif
