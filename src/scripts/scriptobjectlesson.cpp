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

#include <KDebug>

namespace Scripting
{

    ScriptObjectLesson::ScriptObjectLesson ( KEduVocLesson * lesson )
            : QObject(), m_lesson ( lesson )
    {
    }


    ScriptObjectLesson::~ScriptObjectLesson()
    {
    }

    QList<QObject*> ScriptObjectLesson::getEntries()
    {
/// @note This will be usefull somewhere!!
//             void setStyle(QObject* style) {
//                 ParagraphStyle* s = dynamic_cast<ParagraphStyle*>(style);

        //doesn't work (crashes)
//         QList<QObject*> entries;
//         KEduVocExpression * entry;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             entries.push_back ( new ScriptObjectEntry (entry) );
//         }
//         return entries;
    }

    KEduVocContainer::EnumEntriesRecursive boolToEnum(bool value) {
        if (value)
            return KEduVocContainer::Recursive;
        return KEduVocContainer::NotRecursive;

    }

    QObject * ScriptObjectLesson::entry ( int row, bool recursive )
    {
        return new ScriptObjectEntry(m_lesson->entry(row, boolToEnum(recursive)));
    }

    int ScriptObjectLesson::entryCount ( bool recursive )
    {
        return m_lesson->entryCount(boolToEnum(recursive));
    }

    void ScriptObjectLesson::appendEntry ( ScriptObjectEntry * entry )
    {
        m_lesson->appendEntry(entry->kEduVocEntry());
    }

    void ScriptObjectLesson::insertEntry ( int index, ScriptObjectEntry * entry )
    {
        m_lesson->insertEntry(index,entry->kEduVocEntry());
    }

    void ScriptObjectLesson::removeEntry ( ScriptObjectEntry * entry )
    {
        m_lesson->removeEntry(entry->kEduVocEntry());
    }

}
