/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "container.h"

namespace Scripting
{

Container::Container(KEduVocContainer * container)
    : QObject(), m_container(container)
{

}

//     Container::Container ( const QString& name, EnumContainerType type, KEduVocContainer *parent = 0 )
//             : QObject()
//     {
//         m_container = new KEduVocContainer ( name,type,parent );
//     }

//     Container::Container ( const Container &other )
//             : QObject()
//     {
//         m_container = new KEduVocLesson ( other.kEduVocLesson() );
//     }


Container::~Container()
{
}

bool Container::enumToBool(KEduVocContainer::EnumEntriesRecursive recursive)
{
    if (recursive == KEduVocContainer::Recursive)
        return true;
    return false;
}

KEduVocContainer::EnumEntriesRecursive Container::boolToEnum(bool recursive)
{
    if (recursive)
        return KEduVocContainer::Recursive;
    return KEduVocContainer::NotRecursive;
}

QVariantList Container::childContainers()
{
    return toVariantList<KEduVocContainer, Container> (m_container->childContainers());
}

QList<KEduVocContainer*>  Container::flattenContainer(KEduVocContainer * root)
{
    QList<KEduVocContainer*> list;
    if (root) {
        list << root;
        foreach(KEduVocContainer * child, root->childContainers())
        list += flattenContainer(child);
    }
    return list;
}

KEduVocContainer * Container::findContainer(const QString& name)
{
    QList<KEduVocContainer*> list = flattenContainer(m_container);
    foreach(KEduVocContainer * container, list)
    if (container->name() == name)
        return container;
    return 0;
}


}
