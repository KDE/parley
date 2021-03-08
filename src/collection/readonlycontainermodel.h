/*
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef READONLYCONTAINERMODEL_H
#define READONLYCONTAINERMODEL_H

#include <memory>

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <KEduVocDocument>
#include <KEduVocLesson>

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
    virtual void setDocument(const std::shared_ptr<KEduVocDocument> &doc);

protected:
    virtual KEduVocContainer *rootContainer() const = 0;
    KEduVocContainer::EnumContainerType m_type;
    std::shared_ptr<KEduVocDocument> m_doc;
};


#endif
