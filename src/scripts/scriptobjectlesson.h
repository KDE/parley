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
            Q_PROPERTY ( QVariantList childLessons READ childLessons )
        public:

            Lesson ( KEduVocLesson * lesson );

            Lesson ( const QString& name );

            ~Lesson();

            //just to change the name from childContainers to childLessons
            QVariantList childLessons() { return childContainers(); }

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
            void appendChildLesson ( Container *child ) { appendChildContainer ( child ); }
            void insertChildLesson ( int row, Container *child ) { insertChildContainer ( row, child ); }
            void deleteChildLesson ( int row ) { deleteChildContainer ( row ); }
            void removeChildLesson ( int row ) { removeChildContainer ( row ); }
            Container *childLesson ( int row ) { return childContainer ( row ); }

            /**
             * Retrieve a child container by its name
             * Returns 0 if no container is found
             * @param name lesson name
             * @return the child lesson
             */
            Container *childLesson ( const QString& name ) { return childContainer ( name ); }



            int childLessonCount() const { return childContainerCount(); }

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
