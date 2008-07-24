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
            /**
             * Returns a list of all the entries of a lesson
             * @code
             * import Parley
             * entries = Parley.doc.rootLesson.entries(True)
             * for entry in entries
             *     print entry.translations
             * @endcode
             * @param recursive If true, then will return recursively all the entries below this lesson
             * @return A list of Expression objects (entries)
             */
            QVariantList entries ( bool recursive = false ) const;

            /** Returns the entry of the given @p row. If @p recursive is true then it considers all the entries of the sublessons too. */
            QObject * entry ( int row, bool recursive = false );

            /** Returns how many entries are in this lesson (or with sublessons if @p recursive is true) */
            int entryCount ( bool recursive = false );

            /**
             * Appends an entry at the end of the lesson
             * @code
             * import Parley
             * lesson = Parley.doc.findLesson("Lesson 2")
             * if lesson != None:
             *     new_entry = lesson.newEntry(["hello","bonjour"])
             *     lesson.appendEntry(new_entry)
             * @endcode
             * @param entry The entry to add the the lesson
             */
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

            /**
             * Insert an entry on a specific @p index
             * @param index Index where the entry will be added
             * @param entry The entry to add (can be created by newEntry() function)
             */
            void insertEntry ( int index, Expression * entry );

            /**
             * Removes an entry from this lesson
             * @code
             * #how to remove all the entries with the word "play" (from all lessons)
             * import Parley
             * for lesson in Parley.doc.allLessons():
             *     for entry in lesson.entries():
             *         for tr in entry.translations():
             *             if tr.text == "play":
             *                 lesson.remove(entry)
             * @endcode
             * @param entry A reference to the entry to remove
             */
            void removeEntry ( QObject * entry );

            /**
             * Creates and returns a new Expression Object
             * @code
             * #how to add a new entry
             * import Parley
             * lesson = Parley.doc.findLesson("Lesson 2")
             * if lesson != None:
             *     new_entry = lesson.newEntry()
             *     new_entry.setTranslation(0,"day")
             *     new_entry.setTranslation(0,"jour")
             *     lesson.appendEntry(new_entry)
             * @endcode
             */
            QObject* newEntry();

            /**
             * Creates and returns a new Expression Object
             * @code
             * import Parley
             * lesson = Parley.doc.findLesson("Lesson 2")
             * if lesson != None:
             *     new_entry = lesson.newEntry(["hello","bonjour"])
             *     lesson.appendEntry(new_entry)
             * @endcode
             * @param translations A list with the translations of this entry (must be in correct order)
             * @return A new Expression object
             */
            QObject* newEntry ( QStringList translations );


            // @note this one doesn't work with the previous one (python doesn't know which one to call)
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

            /**
             * Returns a list of all the child lessons
             * @code
             * #how to access the all the lessons
             * import Parley
             * for lesson in Parley.doc.rootLesson.childLessons(True):
             *     print lesson.name
             * @endcode
             * @param recursive If true, then will return the child lessons recursively
             * @return A List with Lesson objects (child lessons)
             */
            QVariantList childLessons ( bool recursive = false );

            /**
             * Searches through all the child lessons (recursively) and returns the first lesson the specified @p name
             * @code
             * #how to search for a lesson
             * import Parley
             * lesson = Parley.doc.rootLesson.findChildLesson("Lesson 5")
             * if lesson != None:
             *     print "found"
             *     print lesson.name
             * else:
             *     print "not found"
             * @endcode
             * @param name Name of the lesson to look for
             * @return A reference to a lesson if found. 0 otherwise
             */
            QObject * findChildLesson ( const QString& name );

            /**
             * Returns how many child lessons exist under this lesson
             */
            int childLessonCount() const { return childContainerCount(); }

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
