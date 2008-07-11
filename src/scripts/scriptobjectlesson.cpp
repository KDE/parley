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

    Lesson::Lesson ( KEduVocLesson * lesson )
            : QObject(), m_lesson ( lesson )
    {
    }


    Lesson::~Lesson()
    {
    }

//     QList<Entry> Lesson::getEntries()
    QList<QVariant> Lesson::getEntries()
    {
/// @note This will be usefull somewhere!!
//             void setStyle(QObject* style) {
//                 ParagraphStyle* s = dynamic_cast<ParagraphStyle*>(style);

        //doesn't work (crashes)
//         QList<Entry> entries;
//         KEduVocExpression * entry;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             Entry e(entry);
//             entries.push_back(e);
// //             entries.push_back ( new Entry (entry) );
//         }
//         return entries;
//         QList<QVariant> list;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             QVariant q(Entry(entry));
//             list.push_back(q);
// //             entries.push_back ( new Entry (entry) );
//         }
//         return list;

    }

    KEduVocContainer::EnumEntriesRecursive boolToEnum(bool value) {
        if (value)
            return KEduVocContainer::Recursive;
        return KEduVocContainer::NotRecursive;

    }

    Entry * Lesson::entry ( int row, bool recursive )
    {
        return new Entry(m_lesson->entry(row, boolToEnum(recursive)));
    }

    int Lesson::entryCount ( bool recursive )
    {
        return m_lesson->entryCount(boolToEnum(recursive));
    }

    void Lesson::appendEntry ( Entry * entry )
    {
        /// @todo try it out
        m_lesson->appendEntry(entry->kEduVocEntry());
    }

    void Lesson::insertEntry ( int index, Entry * entry )
    {
        /// @todo try it out
        m_lesson->insertEntry(index,entry->kEduVocEntry());
    }

    void Lesson::removeEntry ( QObject * entry )
    {
        Entry * e = dynamic_cast<Entry*>(entry);
        if (e) {
            m_lesson->removeEntry(e->kEduVocEntry());
        } else {
            kDebug() << "The entry given does not exist";
        }
    }

}
