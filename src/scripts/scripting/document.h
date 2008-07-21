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
#ifndef SCRIPTOBJECTDOCUMENT_H
#define SCRIPTOBJECTDOCUMENT_H

#include <keduvocdocument.h>

#include "lesson.h"

#include <QObject>

namespace Scripting
{
    class Translation;

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

            KEduVocWordType * wordTypeFromString ( const QString & name );

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
            QObject * newLesson ( const QString & name ) { return new Lesson ( name ); }

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

            void setWordType ( QObject * tr, const QString & wordtype );

            QStringList wordTypes();

            // *** tense methods ***

            /**
             * Set the name of a tense
             * @param index            number of tense
             * @param tense            name of tense
             */
            void setTenseName ( int index, const QString &tense ) { m_doc->setTenseName(index,tense); }

            /**
             * Returns the tense string
             *
             * @param index            number of tense
             * @returns                string
             */
            QString tenseName ( int index ) const { return m_doc->tenseName(index); }

            /**
             * Sets the description of the tenses
             */
//             void setTenseDescriptions ( QObject * names );

            /**
             * Gets the descriptions of the tenses
             */
            QStringList tenseDescriptions() const { return m_doc->tenseDescriptions(); }

        private:
            KEduVocDocument * m_doc;
    };

}
#endif
