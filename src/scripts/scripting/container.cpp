/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "container.h"

namespace Scripting
{

    Container::Container ( KEduVocContainer * container )
            : QObject(), m_container ( container )
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

    bool Container::enumToBool ( KEduVocContainer::EnumEntriesRecursive recursive )
    {
        if ( recursive == KEduVocContainer::Recursive )
            return true;
        return false;
    }

    KEduVocContainer::EnumEntriesRecursive Container::boolToEnum ( bool recursive )
    {
        if ( recursive )
            return KEduVocContainer::Recursive;
        return KEduVocContainer::NotRecursive;
    }

    QVariantList Container::childContainers()
    {
        return toVariantList<KEduVocContainer,Container> ( m_container->childContainers() );
    }

    QList<KEduVocContainer*>  Container::flattenContainer ( KEduVocContainer * root )
    {
        QList<KEduVocContainer*> list;
        if ( root )
        {
            list << root;
            foreach ( KEduVocContainer * child, root->childContainers() )
            list += flattenContainer ( child );
        }
        return list;
    }

    KEduVocContainer * Container::findContainer ( const QString& name )
    {
        QList<KEduVocContainer*> list = flattenContainer(m_container);
        foreach (KEduVocContainer * container, list)
            if (container->name() == name)
                return container;
        return 0;
    }


}
