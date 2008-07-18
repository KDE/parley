//
// C++ Interface: scriptobjectlesson
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGSCRIPTOBJECTLESSON_H
#define SCRIPTINGSCRIPTOBJECTLESSON_H

#include <keduvoclesson.h>
#include "scriptobjectcontainer.h"

#include <QObject>
#include <KSharedPtr>

namespace Scripting
{

    class Expression;

    /**
    Lesson script object class

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Lesson : public Container
    {
            Q_OBJECT
        public:

            Lesson ( KEduVocLesson * lesson );

            Lesson ( KEduVocContainer * container );

            Lesson ( const QString& name );

            ~Lesson();

        public slots:
            QVariantList entries ( bool recursive = false ) const;
            Expression * entry ( int row, bool recursive = false );
            int entryCount ( bool recursive = false );
            void appendEntry ( Expression * entry );
            void insertEntry ( int index, Expression * entry );
            void removeEntry ( QObject * entry );


            /**
             * Creates and returns a new Expression Object
             * @return A new Expression object
             */
            Expression* newEntry();

            /**
             * Creates and returns a new Expression Object
             * @param expression
             * @return A new Expression object
             */
            Expression* newEntry ( const QString & expression );

            /**
             * Creates and returns a new Expression Object
             * @param translations
             * @return A new Expression object
             */
            Expression* newEntry ( const QStringList & translations );

            //child lesson public functions (just to change the names from "Container" to "Lesson")
            void appendChildLesson ( Lesson *child ) { appendChildContainer ( child ); }
            void insertChildLesson ( int row, Lesson *child ) { insertChildContainer ( row, child ); }
            void deleteChildLesson ( int row ) { deleteChildContainer ( row ); }
            void removeChildLesson ( int row ) { removeChildContainer ( row ); }
            Lesson *childLesson ( int row ) { return new Lesson(m_lesson->childContainer(row)); }

            /**
             * Retrieve a child container by its name
             * Returns 0 if no container is found
             * @param name lesson name
             * @return the child lesson
             */
            Lesson *childLesson ( const QString& name ) { return new Lesson(m_lesson->childContainer(name)); }

            QVariantList childLessons(bool recursive = false);

            int childLessonCount() const { return childContainerCount(); }

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
