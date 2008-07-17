//
// C++ Implementation: container
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectcontainer.h"

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

}
