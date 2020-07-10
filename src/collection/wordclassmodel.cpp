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
