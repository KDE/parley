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
#include "document.h"
#include <keduvocwordtype.h>

#include "translation.h"

#include <KDebug>

namespace Scripting
{

    Document::Document ( KEduVocDocument * doc )
            : QObject(), m_doc ( doc )
    {
    }

    Document::~Document()
    {
    }

    QVariantList Document::allLessons()
    {
        Lesson * l = new Lesson ( m_doc->lesson() );
        return QVariantList() << qVariantFromValue ( static_cast<QObject*> ( l ) ) <<  l->childLessons ( true );
    }

    QList<KEduVocContainer*>  flattenContainer ( KEduVocContainer * root )
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

    KEduVocWordType * Scripting::Document::wordTypeFromString ( const QString & name )
    {
        QList<KEduVocContainer*> list = flattenContainer ( m_doc->wordTypeContainer() );
        list.removeFirst();

//         foreach ( KEduVocContainer * child, list )
//             kDebug() << static_cast<KEduVocWordType*>(child)->name();

        foreach ( KEduVocContainer * child, list )
        {
            KEduVocWordType * wt = static_cast<KEduVocWordType*> ( child );
            if ( name == wt->name() )
                return wt;
        }
        return 0;
    }

    void Scripting::Document::setWordType ( QObject * translation, const QString & wordtype )
    {
        Translation * tr = dynamic_cast<Translation*> ( translation );
        if ( !tr )
        {
            kDebug() << "Invalid lesson entry";
            return;
        }
        KEduVocWordType * wt = wordTypeFromString ( wordtype );
        if ( wt )
            tr->setWordType ( wt );
        else
            kDebug() << "Invalid given wordtype: " << wordtype;
    }

    QStringList Scripting::Document::wordTypes()
    {
        QList<KEduVocContainer*> list = flattenContainer ( m_doc->wordTypeContainer() );
        list.removeFirst();
        QStringList strList;
        foreach ( KEduVocContainer * child, list )
        {
            strList << child->name();
        }
        return strList;
    }

}
