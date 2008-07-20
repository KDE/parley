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
        Lesson * l = new Lesson( m_doc->lesson() );
        return QVariantList() << qVariantFromValue(static_cast<QObject*>(l)) <<  l->childLessons(true);
    }

}

