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
            : Container ( lesson ), m_lesson ( lesson )
    {
    }

    Lesson::Lesson ( const QString& name )
    {
        m_lesson = new KEduVocLesson ( name );
        m_container = m_lesson;
    }


    Lesson::~Lesson()
    {
    }

// Didn't work either
//     QObjectList Lesson::getEntries()
//     {
//         QObjectList oList;
//         KEduVocExpression* entry;
//         foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) )
//             oList.push_back( new Expression(entry) );
//         return oList;
//     }


    QVariantList Lesson::getEntries()
    {
        QVariantList list;
        KEduVocExpression* entry;
        foreach ( entry, m_lesson->entries ( KEduVocContainer::Recursive ) ) {
//              QVariant v(QVariant::UserType,new Expression(entry));
            QObject * obj = new Expression(entry);
             QVariant v = qVariantFromValue(obj);
             list.push_back( v );
        }
        return list;
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

/// @note this function is not needed cause it's inherited from Scripting::Container
//     KEduVocContainer::EnumEntriesRecursive boolToEnum ( bool value )
//     {
//         if ( value )
//             return KEduVocContainer::Recursive;
//         return KEduVocContainer::NotRecursive;
//
//     }

    Expression * Lesson::entry ( int row, bool recursive )
    {
        return new Expression ( m_lesson->entry ( row, boolToEnum ( recursive ) ) );
    }

    int Lesson::entryCount ( bool recursive )
    {
        return m_lesson->entryCount ( boolToEnum ( recursive ) );
    }

    void Lesson::appendEntry ( Expression * entry )
    {
        /// @todo try it out
        m_lesson->appendEntry ( entry->kEduVocEntry() );
    }

    void Lesson::insertEntry ( int index, Expression * entry )
    {
        /// @todo try it out
        m_lesson->insertEntry ( index,entry->kEduVocEntry() );
    }

    void Lesson::removeEntry ( QObject * entry )
    {
        /// @note parameter has to be QObject (tried with Expression * entry but didn't work)
        Expression * e = dynamic_cast<Expression*> ( entry );
        if ( e )
        {
            m_lesson->removeEntry ( e->kEduVocEntry() );
        }
        else
        {
            kDebug() << "The entry given does not exist";
        }
    }

    Expression* Lesson::newEntry()
    {
        return new Expression();
    }

    Expression* Lesson::newEntry ( const QString & expression )
    {
        return new Expression ( expression );
    }

    Expression* Lesson::newEntry ( const QStringList & translations )
    {
        return new Expression ( translations );
    }

}
