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
#include "scriptobjectexpression.h"

#include <keduvocexpression.h>

#include <KDebug>

namespace Scripting
{
    Lesson::Lesson ( KEduVocLesson * lesson )
            : Container ( lesson ), m_lesson ( lesson )
    {
    }

    Lesson::Lesson ( KEduVocContainer * container )
    {
        m_lesson = static_cast<KEduVocLesson*> ( container );
        m_container = m_lesson;
    }

    Lesson::Lesson ( const QString& name )
    {
        m_lesson = new KEduVocLesson ( name );
        m_container = m_lesson;
    }


    Lesson::~Lesson()
    {
    }

    QList<KEduVocLesson*>  flattenLessons(KEduVocLesson * rootLesson) {
        QList<KEduVocLesson*> lessonsList;
        foreach (KEduVocContainer * child, rootLesson->childContainers()) {
            lessonsList << static_cast<KEduVocLesson*>(child);
            lessonsList += flattenLessons(static_cast<KEduVocLesson*>(child));
        }
        return lessonsList;
    }


    QVariantList Lesson::childLessons(bool recursive)
    {
        if (recursive)
            return toVariantList<KEduVocLesson,Lesson> ( flattenLessons(m_lesson) );
        return toVariantList<KEduVocContainer,Lesson> ( m_lesson->childContainers() );
    }

    QVariantList Lesson::entries ( bool recursive ) const
    {
        return toVariantList<KEduVocExpression,Expression> ( m_lesson->entries ( boolToEnum ( recursive ) ) );
    }

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
        m_lesson->appendEntry ( entry->kEduVocEntry() );
    }

    void Lesson::insertEntry ( int index, Expression * entry )
    {
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
