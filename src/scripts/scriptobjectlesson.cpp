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
#include "scriptobjectentry.h"

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

//     QList<Expression> Lesson::getEntries()
//     QList<QVariant> Lesson::getEntries()
//     {
/// @note This will be usefull somewhere!!
//             void setStyle(QObject* style) {
//                 ParagraphStyle* s = dynamic_cast<ParagraphStyle*>(style);

        //doesn't work (crashes)
//         QList<Expression> entries;
//         KEduVocExpression * entry;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             Expression e(entry);
//             entries.push_back(e);
// //             entries.push_back ( new Expression (entry) );
//         }
//         return entries;
//         QList<QVariant> list;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//         {
//             kDebug() << entry->translation ( 0 )->text();
//             QVariant q(Expression(entry));
//             list.push_back(q);
// //             entries.push_back ( new Expression (entry) );
//         }
//         return list;

//     }

    KEduVocContainer::EnumEntriesRecursive boolToEnum(bool value) {
        if (value)
            return KEduVocContainer::Recursive;
        return KEduVocContainer::NotRecursive;

    }

    Expression * Lesson::entry ( int row, bool recursive )
    {
        return new Expression(m_lesson->entry(row, boolToEnum(recursive)));
    }

    int Lesson::entryCount ( bool recursive )
    {
        return m_lesson->entryCount(boolToEnum(recursive));
    }

    void Lesson::appendEntry ( Expression * entry )
    {
        /// @todo try it out
        m_lesson->appendEntry(entry->kEduVocEntry());
    }

    void Lesson::insertEntry ( int index, Expression * entry )
    {
        /// @todo try it out
        m_lesson->insertEntry(index,entry->kEduVocEntry());
    }

    void Lesson::removeEntry ( QObject * entry )
    {
        /// @note parameter has to be QObject (tried with Expression * entry but didn't work)
        Expression * e = dynamic_cast<Expression*>(entry);
        if (e) {
            m_lesson->removeEntry(e->kEduVocEntry());
        } else {
            kDebug() << "The entry given does not exist";
        }
    }

}
