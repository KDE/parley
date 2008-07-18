//
// C++ Implementation: scriptobjectdocument
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectdocument.h"

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

