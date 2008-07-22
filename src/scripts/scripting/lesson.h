/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTINGSCRIPTOBJECTLESSON_H
#define SCRIPTINGSCRIPTOBJECTLESSON_H

#include <keduvoclesson.h>
#include "container.h"

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
            QObject * entry ( int row, bool recursive = false );
            int entryCount ( bool recursive = false );
            void appendEntry ( Expression * entry );

            /**
             * Creates and appends a new entry with the given @p translations
             * @code
             * #how to add a new entry with appendNewEntry()
             * import Parley
             * lesson = Parley.doc.rootLesson
             * lesson.appendNewEntry(["good morning","bonjour"])
             * lesson.appendNewEntry(["play","jouer"])
             * @endcode
             * @param translations A string list with the translations (in same order as their identifiers)
             */
            void appendNewEntry ( QStringList translations );
            void insertEntry ( int index, Expression * entry );
            void removeEntry ( QObject * entry );


            /**
             * Creates and returns a new Expression Object
             * @return A new Expression object
             */
            QObject* newEntry();

            /**
             * Creates and returns a new Expression Object
             * @param translations
             * @return A new Expression object
             */
            ///@todo try this one if it works (list as parameter)
            QObject* newEntry ( QStringList translations );


            /// @note this one doesn't work with the previous one (python doesn't know which one to call)
            /*
             * Creates and returns a new Expression Object
             * @param expression
             * @return A new Expression object
             */
//             QObject* newEntry ( const QString & expression );


            //child lesson public functions (just to change the names from "Container" to "Lesson")
            void appendChildLesson ( Lesson *child ) { appendChildContainer ( child ); }
            void insertChildLesson ( int row, Lesson *child ) { insertChildContainer ( row, child ); }
            void deleteChildLesson ( int row ) { deleteChildContainer ( row ); }
            void removeChildLesson ( int row ) { removeChildContainer ( row ); }
            QObject *childLesson ( int row ) { return new Lesson ( m_lesson->childContainer ( row ) ); }

            /**
             * Retrieve a child container by its name
             * Returns 0 if no container is found
             * @param name lesson name
             * @return the child lesson
             */
            QObject *childLesson ( const QString& name ) { return new Lesson ( m_lesson->childContainer ( name ) ); }

            QVariantList childLessons ( bool recursive = false );

            int childLessonCount() const { return childContainerCount(); }

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
