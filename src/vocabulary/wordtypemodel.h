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

#ifndef WORDTYPEMODEL_H
#define WORDTYPEMODEL_H

#include "containermodel.h"

namespace Editor
{
/**
  * Model for the tree of word types.
  */
class WordTypeModel : public ContainerModel
{
    Q_OBJECT

public:
    explicit WordTypeModel(QObject *parent = 0);

    QModelIndex appendWordType(const QModelIndex& parent, const QString & wordTypeName = QString());

    void deleteWordType(const QModelIndex& wordTypeIndex);

protected:
    KEduVocContainer * rootContainer() const;
};
}

#endif
