/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


// Own
#include "wordclassmodel.h"

#include <KEduVocWordtype>


WordClassModel::WordClassModel(QObject * parent)
    : ContainerModel(KEduVocContainer::WordType, parent)
{
}

KEduVocContainer * WordClassModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }
    return m_doc->wordTypeContainer();
}
