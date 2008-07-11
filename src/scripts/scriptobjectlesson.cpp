//
// C++ Implementation: scriptobjectlesson
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectlesson.h"
#include <keduvocexpression.h>

namespace Scripting
{

    ScriptObjectLesson::ScriptObjectLesson ( KEduVocLesson * lesson )
            : QObject()
    {
        m_lesson = lesson;
    }


    ScriptObjectLesson::~ScriptObjectLesson()
    {
    }

    QList<QObject*> ScriptObjectLesson::getEntries()
    {
        QList<QObject*> entries;
        KEduVocExpression * entry;
        foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) ) {
            entries.push_back(new ScriptObjectEntry(entry));
        }
        return entries;
    }

    QObject * ScriptObjectLesson::firstEntry() {
        return new ScriptObjectEntry( m_lesson->entry(0,KEduVocContainer::Recursive) );
    }

}
