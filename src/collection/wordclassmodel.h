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

#ifndef WORDCLASSMODEL_H
#define WORDCLASSMODEL_H

#include "containermodel.h"

namespace Editor
{
/**
  * Model for the tree of word types.
  */
class WordClassModel : public ContainerModel
{
    Q_OBJECT

public:
    explicit WordClassModel(QObject *parent = 0);

    QModelIndex appendWordClass(const QModelIndex& parent, const QString & wordTypeName = QString());

    void deleteWordClass(const QModelIndex& wordTypeIndex);

protected:
    KEduVocContainer * rootContainer() const;
};
}

#endif
