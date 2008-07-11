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

//     QList<ScriptObjectEntry> ScriptObjectLesson::getEntries()
    QList<QVariant> ScriptObjectLesson::getEntries()
    {
/// @note This will be usefull somewhere!!
//             void setStyle(QObject* style) {
//                 ParagraphStyle* s = dynamic_cast<ParagraphStyle*>(style);

        //doesn't work (crashes)
//         QList<ScriptObjectEntry> entries;
//         KEduVocExpression * entry;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             ScriptObjectEntry e(entry);
//             entries.push_back(e);
// //             entries.push_back ( new ScriptObjectEntry (entry) );
//         }
//         return entries;
//         QList<QVariant> list;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             QVariant q(ScriptObjectEntry(entry));
//             list.push_back(q);
// //             entries.push_back ( new ScriptObjectEntry (entry) );
//         }
//         return list;

    }

    KEduVocContainer::EnumEntriesRecursive boolToEnum(bool value) {
        if (value)
            return KEduVocContainer::Recursive;
        return KEduVocContainer::NotRecursive;

    }

    ScriptObjectEntry * ScriptObjectLesson::entry ( int row, bool recursive )
    {
        return new ScriptObjectEntry(m_lesson->entry(row, boolToEnum(recursive)));
    }

    int ScriptObjectLesson::entryCount ( bool recursive )
    {
        return m_lesson->entryCount(boolToEnum(recursive));
    }

    void ScriptObjectLesson::appendEntry ( ScriptObjectEntry * entry )
    {
        /// @todo try it out
        m_lesson->appendEntry(entry->kEduVocEntry());
    }

    void ScriptObjectLesson::insertEntry ( int index, ScriptObjectEntry * entry )
    {
        /// @todo try it out
        m_lesson->insertEntry(index,entry->kEduVocEntry());
    }

    void ScriptObjectLesson::removeEntry ( QObject * entry )
    {
        ScriptObjectEntry * e = dynamic_cast<ScriptObjectEntry*>(entry);
        if (e) {
            m_lesson->removeEntry(e->kEduVocEntry());
        } else {
            kDebug() << "The entry given does not exist";
        }
    }

}
