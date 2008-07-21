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

            KEduVocDocument * kEduVocDocument() { return m_doc; }

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



// ---------------------  copied from KEduVocDocument

            /**
             * Open a document file
             *
             * @param url      url to file to open
             * @returns        ErrorCode
             */
//             int open ( const KUrl& url );

            /**
             * Saves the data under the given name
             *
             * @param url        if url is empty (or NULL) actual name is preserved
             * @param ft         the filetype to be used when saving the document
             * @param generator  the name of the application saving the document
             * @returns          ErrorCode
             */
//             int saveAs ( const KUrl & url, FileType ft, const QString & generator );

//             QByteArray toByteArray ( const QString &generator );

            /**
             * Merges data from another document
             *
             * @param docToMerge       document containing the data to be merged
             * @param matchIdentifiers if true only entries having identifiers present in the
             *                         current document will be mergedurl is empty (or NULL) actual name is preserved
             */
//             void merge ( Document *docToMerge, bool matchIdentifiers );

            /**
             * Indicates if the document is modified
             *
             * @param dirty   new state
             */
            void setModified ( bool dirty = true ) { m_doc->setModified(dirty); }

            /** @returns the modification state of the doc */
            bool isModified() const { return m_doc->isModified(); }

            /**
             * Sets the URL of the XML file
             */
//             void setUrl ( const KUrl& url );

            /** @returns the URL of the XML file */
//             KUrl url() const;


            /** set the title of the file
             * @param title title to set */
            void setTitle ( const QString & title ) { m_doc->setTitle(title); }

            /** @returns the title of the file */
            QString title() const { return m_doc->title(); }

            /** set the author of the file
             * @param author author to set */
            void setAuthor ( const QString & author ) { m_doc->setAuthor(author); }

            /** @returns the author of the file */
            QString author() const { return m_doc->author(); }

            /** set the author contact info
             * @param contact email/contact info to set */
            void setAuthorContact ( const QString & authorContact ) { m_doc->setAuthorContact(authorContact); }

            /** @returns the author contact information */
            QString authorContact() const { return m_doc->authorContact(); }

            /** set the license of the file
             * @param license license to set */
            void setLicense ( const QString & license ) { m_doc->setLicense(license); }

            /** @returns the license of the file */
            QString license() const { return m_doc->license(); }

            /** set the comment of the file
             * @param comment comment to set */
            void setDocumentComment ( const QString & comment ) { m_doc->setDocumentComment(comment); }

            /** @return the comment of the file */
            QString documentComment() const { return m_doc->documentComment(); }

            /** set the category of the file
             * @param category category to set */
            void setCategory ( const QString & category ) { m_doc->setCategory(category); }

            /** @return the category of the file */
            QString category() const { return m_doc->category(); }

            /**
             * Sets the generator of the file
             */
            void setGenerator ( const QString & generator ) { m_doc->setGenerator(generator); }

            /** @returns the generator of the file */
            QString generator() const { m_doc->generator(); }

            /** Sets version of the loaded file
             * @param ver the new version */
            void setVersion ( const QString & ver ) { m_doc->setVersion(ver); }

            /** @returns the version of the loaded file */
            QString version() const { return m_doc->version(); }

            // *** identifier methods ***

            /**
             * @returns the number of different identifiers (usually languages)
             */
            int identifierCount() const { return m_doc->identifierCount(); }

            /**
             * Appends a new identifier (usually a language)
             *
             * @param identifier the identifier to append. If empty default names are used.
             * @returns the identifier number
             */
//             int appendIdentifier ( const KEduVocIdentifier & identifier = KEduVocIdentifier() );

            /**
             * Sets the identifier of translation
             *
             * @param index            number of translation 0..x
             * @param lang             thr language identifier: en=english, de=german, ...
             */
//             void setIdentifier ( int index, const KEduVocIdentifier& lang );

            /**
             * Returns the identifier of translation @p index
             *
             * @param index            number of translation 0..x
             * @returns                the language identifier: en=english, de=german, ...
             */
//             KEduVocIdentifier& identifier ( int index );

            /**
             * Removes identifier and the according translations in all entries
             *
             * @param index            number of translation 0..x
             */
            void removeIdentifier ( int index ) { m_doc->removeIdentifier(index); }

            /**
             * Determines the index of a given identifier
             *
             * @param lang             identifier of language
             * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
             */
//             int indexOfIdentifier ( const QString &name ) const { return m_doc->indexOfIdentifier(name); }

            // *** tense methods ***

            /**
             * Set the name of a tense
             * @param index            number of tense
             * @param tense            name of tense
             */
            void setTenseName ( int index, const QString &tense ) { m_doc->setTenseName ( index,tense ); }

            /**
             * Returns the tense string
             *
             * @param index            number of tense
             * @returns                string
             */
            QString tenseName ( int index ) const { return m_doc->tenseName ( index ); }

            /**
             * Sets the description of the tenses
             */
//             void setTenseDescriptions ( QObject * names );

            /**
             * Gets the descriptions of the tenses
             */
            QStringList tenseDescriptions() const { return m_doc->tenseDescriptions(); }

            // *** grade methods ***

            /**
             * Retrieves the identifiers for the current query
             * not written in the new version!
             *
             * @param org        identifier for original
             * @param trans      identifier for translation
             */
//             KDE_DEPRECATED void queryIdentifier ( QString &org, QString &trans ) const;

            /**
             * Sets the identifiers for the current query
             * not written in the new version!
             *
             * @param org        identifier for original
             * @param trans      identifier for translation
             */
//             KDE_DEPRECATED void setQueryIdentifier ( const QString &org, const QString &trans );

            // *** lesson methods ***

            /** get the lesson root object
             * @returns a pointer to the lesson object
             */
//             KEduVocLesson * lesson();

//             KEduVocWordType * wordTypeContainer();

//             KEduVocLeitnerBox * leitnerContainer();

            // *** file format specific methods ***

            /**
             * Returns the delimiter (separator) used for csv import and export.
             * The default is a single tab character
             *
             * @returns                the delimiter used
             */
            QString csvDelimiter() const { return m_doc->csvDelimiter(); }

            /**
             * Sets the delimiter (separator) used for csv import and export
             *
             * @param delimiter        the delimiter to use
             */
            void setCsvDelimiter ( const QString &delimiter ) { m_doc->setCsvDelimiter(delimiter); }

//             static FileType detectFileType ( const QString &fileName );

            /**
             * Create a string with the supported document types, that can be used
             * as filter in KFileDialog. It includes also an entry to match all the
             * supported types.
             *
             * @param mode             the mode for the supported document types
             * @returns                the filter string
             */
//             static QString pattern ( FileDialogMode mode );

//             static QString errorDescription ( int errorCode );

        private:
            KEduVocDocument * m_doc;
    };

}
#endif
