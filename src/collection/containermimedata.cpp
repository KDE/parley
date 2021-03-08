/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "containermimedata.h"


void ContainerMimeData::addContainer(KEduVocContainer * container)
{
    m_containers.append(container);
}

QList<KEduVocContainer *> ContainerMimeData::containerList() const
{
    return m_containers;
}
