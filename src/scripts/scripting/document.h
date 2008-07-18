//
// C++ Interface: scriptobjectdocument
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTOBJECTDOCUMENT_H
#define SCRIPTOBJECTDOCUMENT_H

#include <keduvocdocument.h>

#include "lesson.h"

#include <QObject>

namespace Scripting
{
    //Implements the Document object to be used by the scripts
    /**
     * Parley Document scripting object. Gives access to all the document properties and lessoons
     *
     * @code
     * #how to get a reference to the Parley's active document
     * import Parley
     * doc = Parley.document
     * @endcode
     *
     * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
     */
    class Document : public QObject
    {
            Q_OBJECT

            /** Read-Only property that gives the root of the lesson tree
            * @code
            * import Parley
            * root = Parley.document.rootLesson
            * @endcode
            */
            Q_PROPERTY ( QObject * rootLesson READ rootLesson )
        public:
            Document ( KEduVocDocument * doc );

            ~Document();

            //Property: rootLesson (READ)
            QObject * rootLesson() { return new Lesson ( m_doc->lesson() ); }

        public slots:

            /**
             * Creates and returns a new lesson (doesn't add it as a sublesson to any lesson)
             * @code
             * #how to add a new lesson to the root lesson
             * import Parley
             * newlesson = Parley.document.newLesson("My New Lesson")
             * Parley.document.rootLesson.appendChildLesson(newlesson)
             * @endcode
             * @param name Name of the lesson
             * @return The new lesson
             */
            Lesson * newLesson ( const QString & name ) { return new Lesson ( name ); }

            /**
             * Returns all the lessons in the document (including sublessons)
             * @code
             * #how to print the names of all the lessons
             * import Parley
             * for lesson in Parley.document.allLessons():
             *    print lesson.name
             * @endcode
             * @return A list of all the lessons in the document
             */
            QVariantList allLessons();

        private:
            KEduVocDocument * m_doc;
    };

}
#endif
