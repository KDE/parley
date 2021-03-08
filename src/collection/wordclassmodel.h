/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WORDCLASSMODEL_H
#define WORDCLASSMODEL_H

#include "containermodel.h"

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
    KEduVocContainer * rootContainer() const Q_DECL_OVERRIDE;
};


#endif
