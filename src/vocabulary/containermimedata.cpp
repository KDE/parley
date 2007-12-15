/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "containermimedata.h"

void ContainerMimeData::addContainer(KEduVocContainer * container)
{
    m_containers.append(container);
}

QList<KEduVocContainer *> ContainerMimeData::containerList() const
{
    return m_containers;
}


#include "containermimedata.moc"


