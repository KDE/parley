/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTOBJECTDOCUMENT_H
#define SCRIPTOBJECTDOCUMENT_H

#include <KEduVocDocument>

#include "lesson.h"
#include "identifier.h"

#include <QObject>

namespace Scripting
{
class Translation;

/**
 * @class Document
 * @brief KEduVocDocument wrapping class for Kross scripts
 *
 * The Document class provides methods and properties for accessing the document lessons, entries, languages, word types and general document parameters. The easiest way of accessing this class is through the Parley.document (Parley.doc) property like in the example.
 *
 * @code
 * #how to get a reference to the Parley's active document
 * import Parley
 * doc = Parley.doc
 * print doc.title, doc.author, doc.authorContact
 * @endcode
 *
 * You can access <b> document lessons </b> with the following ways:
 * - Document::rootLesson property
 * - allLessons() function
 * - findLesson() function
 *
 * and add a new lesson with
 * - newLesson() and appendLesson() function
 * - Document::appendNewLesson(const QString &) function
 * - Document::appendNewLesson(const QString &,Lesson*) function
 *
 * The <b> document identifiers </b> (see Identifier class) can be set by these functions:
 * - newIdentifier() and appendIdentifier() functions
 * - appendNewIdentifier() function
 * - removeIdentifier() function (for removing a language)
 *
 * and be accessed by:
 * - as a list with identifiers() function
 * - individually by identifier() function
 * - and get how many identifiers exist with identifierCount() function
 *
 * The saveAs() function can be used to save into a file a newly created document, or the active document. See Parley::newDocument().
 *
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Document : public QObject
{
    Q_OBJECT
    /// Document's root lesson
    Q_PROPERTY(QObject * rootLesson READ rootLesson)
    /// Document's title
    Q_PROPERTY(QString title READ title WRITE setTitle)
    /// Document author's name
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    /// Author contact info (email)
    Q_PROPERTY(QString authorContact READ authorContact WRITE setAuthorContact)
    /// Document license
    Q_PROPERTY(QString license READ license WRITE setLicense)
    /// Comment about the document
    Q_PROPERTY(QString documentComment READ documentComment WRITE setDocumentComment)
    /// Category that the lesson belongs
    Q_PROPERTY(QString category READ category WRITE setCategory)
    /// Generator program (which program generates this file)
    Q_PROPERTY(QString generator READ generator WRITE setGenerator)
    /// Document version
    Q_PROPERTY(QString version READ version WRITE setVersion)
    /// Delimiter (separator) used for csv import and export.
    Q_PROPERTY(QString csvDelimiter READ csvDelimiter WRITE setCsvDelimiter)
    /// URL of the XML file
    Q_PROPERTY(QString url READ url WRITE setUrl)

    Q_ENUMS(FileType)
public:

    /// known vocabulary file types
    enum FileType {
        KvdNone,
        Automatic,
        Kvtml,
        Wql,
        Pauker,
        Vokabeln,
        Xdxf,
        Csv,
        Kvtml1
    };

    /// the return code when opening/saving
    enum ErrorCode {
        NoError = 0,
        Unknown,
        InvalidXml,
        FileTypeUnknown,
        FileCannotWrite,
        FileWriterFailed,
        FileCannotRead,
        FileReaderFailed,
        FileDoesNotExist
    };

    /// used as parameter for pattern
    enum FileDialogMode {
        Reading,
        Writing
    };

    /// delete only empty lessons or also if they have entries
    enum LessonDeletion {
        DeleteEmptyLesson,
        DeleteEntriesAndLesson
    };

    explicit Document(QObject* parent = 0);

    explicit Document(KEduVocDocument * doc);

    ~Document();

    KEduVocDocument * kEduVocDocument() {
        return m_doc;
    }

    //Property: rootLesson (READ)
    QObject * rootLesson() {
        return new Lesson(m_doc->lesson());
    }

    KEduVocWordType * wordTypeFromString(const QString & name);

    //**** Property Methods ****//

    /* set the title of the file
     * @param title title to set */
    void setTitle(const QString & title) {
        m_doc->setTitle(title);
    }

    /* @returns the title of the file */
    QString title() const {
        return m_doc->title();
    }

    /* set the author of the file
     * @param author author to set */
    void setAuthor(const QString & author) {
        m_doc->setAuthor(author);
    }

    /* @returns the author of the file */
    QString author() const {
        return m_doc->author();
    }

    /* set the author contact info
     * @param contact email/contact info to set */
    void setAuthorContact(const QString & authorContact) {
        m_doc->setAuthorContact(authorContact);
    }

    /* @returns the author contact information */
    QString authorContact() const {
        return m_doc->authorContact();
    }

    /* set the license of the file
     * @param license license to set */
    void setLicense(const QString & license) {
        m_doc->setLicense(license);
    }

    /* @returns the license of the file */
    QString license() const {
        return m_doc->license();
    }

    /* set the comment of the file
     * @param comment comment to set */
    void setDocumentComment(const QString & comment) {
        m_doc->setDocumentComment(comment);
    }

    /* @return the comment of the file */
    QString documentComment() const {
        return m_doc->documentComment();
    }

    /* set the category of the file
     * @param category category to set */
    void setCategory(const QString & category) {
        m_doc->setCategory(category);
    }

    /* @return the category of the file */
    QString category() const {
        return m_doc->category();
    }

    /*
     * Sets the generator of the file
     */
    void setGenerator(const QString & generator) {
        m_doc->setGenerator(generator);
    }

    /* @returns the generator of the file */
    QString generator() const {
        return m_doc->generator();
    }

    /* Sets version of the loaded file
     * @param ver the new version */
    void setVersion(const QString & ver) {
        m_doc->setVersion(ver);
    }

    /* @returns the version of the loaded file */
    QString version() const {
        return m_doc->version();
    }

    /*
     * Returns the delimiter (separator) used for csv import and export.
     * The default is a single tab character
     *
     * @returns                the delimiter used
     */
    QString csvDelimiter() const {
        return m_doc->csvDelimiter();
    }

    /*
     * Sets the delimiter (separator) used for csv import and export
     *
     * @param delimiter        the delimiter to use
     */
    void setCsvDelimiter(const QString &delimiter) {
        m_doc->setCsvDelimiter(delimiter);
    }

    /*
     * Sets the URL of the XML file
     */
    void setUrl(const QString& url) {
        m_doc->setUrl(QUrl(url));
    }

    /* @returns the URL of the XML file */
    QString url() const {
        return m_doc->url().toString();
    }

public slots:

    /**
     * Creates and returns a new lesson (doesn't add it as a sublesson to any lesson)
     *
     * @code
     * #how to add a new lesson to the root lesson
     * import Parley
     * newlesson = Parley.doc.newLesson("My New Lesson")
     * Parley.doc.rootLesson.appendChildLesson(newlesson)
     * @endcode
     *
     * @param name Name of the lesson
     * @return Lesson object (the new lesson)
     */
    QObject * newLesson(const QString & name) {
        return new Lesson(name);
    }

    /**
     * Appends a lesson to the document
     *
     * @code
     * #how to add a new lesson with appendLesson() function
     * import Parley
     * newlesson = Parley.doc.newLesson("My New Lesson")
     * Parley.doc.appendLesson(newlesson)
     * @endcode
     *
     * @param lesson Lesson object (the lesson to be added)
     */
    void appendLesson(QObject * lesson);

    /**
     * Creates a new lesson and appends it to the root lesson
     * @code
     * #how to add a new lesson to the root lesson
     * import Parley
     * newlesson = Parley.doc.appendNewLesson("My New Lesson")
     * @endcode
     * @param name Lesson name
     * @return A reference to the new lesson
     */
    QObject * appendNewLesson(const QString & name);

    /**
     * Creates a new lesson and appends it to the @p parent lesson
     * @code
     * #how to add a new lesson under another lesson
     * import Parley
     * parentLesson = Parley.doc.appendNewLesson("My Parent Lesson")
     * childLesson = Parley.doc.appendNewLesson("My new child lesson",parentLesson)
     * @endcode
     * @param name Lesson name
     * @param parent Parent lesson
     * @return A reference to the new lesson
     */
    QObject * appendNewLesson(const QString & name, Lesson * parent);

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

    /**
     * Searches through all the lessons (recursively) and returns the first lesson the specified @p name
     * @code
     * #how to search for a lesson
     * import Parley
     * lesson = Parley.doc.findLesson("Lesson 2")
     * if lesson != None:
     *     print lesson.name
     * else:
     *     print "not found"
     * @endcode
     * @param name Name of the lesson to look for
     * @return A reference to a lesson if found. 0 otherwise
     */

    QObject * findLesson(const QString& name);

    /**
     * Sets the word type (@p wordtype) of the given @p tr translation object.
    *  If the @p wordtype is not valid, no changes are made to the translation object
     * @param tr Translation object to set it's word type
     * @param wordtype Word type name
     */
    void setWordType(QObject * tr, const QString & wordtype);

    /**
     * Returns a string list with all the available word type's names
     */
    QStringList wordTypes();

// ---------------------  copied from KEduVocDocument

    /*
     * Open a document file
     *
     * @param url      url to file to open
     * @returns        ErrorCode
     */
//             int open ( const QString & url ) { return m_doc->open ( url ); }

    /**
     * Saves the data under the given name
     *
     * @param url        if url is empty (or NULL) actual name is preserved
     * @param ft         the filetype to be used when saving the document (default value: Automatic). See enum FileType.
     * @param generator  the name of the application saving the document (default value: "Parley")
     * @returns          ErrorCode
     */
    int saveAs(const QString & url, KEduVocDocument::FileType ft = KEduVocDocument::Automatic, const QString & generator = QStringLiteral("Parley")) {
        m_doc->setGenerator(generator);
        return m_doc->saveAs(QUrl(url), ft);
    }

//             QByteArray toByteArray ( const QString &generator );

    /**
     * Merges data from another document
     *
     * @param docToMerge       document containing the data to be merged
     * @param matchIdentifiers if true only entries having identifiers present in the
     *                         current document will be mergedurl is empty (or NULL) actual name is preserved
     */
    void merge(Document *docToMerge, bool matchIdentifiers) {
        m_doc->merge(docToMerge->kEduVocDocument(), matchIdentifiers);
    }

// NOT NEEDED
    /*
     * Indicates if the document is modified
     *
     * @param dirty   new state
     */
//             void setModified ( bool dirty = true ) { m_doc->setModified ( dirty ); }

    /* @returns the modification state of the doc */
//             bool isModified() const { return m_doc->isModified(); }

    // *** identifier methods ***

    /**
     * @returns the number of different identifiers (usually languages)
     */
    int identifierCount() const {
        return m_doc->identifierCount();
    }

    /**
     * Creates a new identifier and returns a reference to it
     * @return
     */
    QObject * newIdentifier() {
        return new Identifier();
    }

    /**
     * Append a new identifier by giving the @p name and @p locale
     * @param name Language description ex. "American English"
     * @param locale Language locale ex. "en_US"
     */
    void appendNewIdentifier(const QString& name, const QString& locale);

    /**
     * Appends a new identifier (usually a language)
     *
     * @param identifier the identifier to append. If empty default names are used.
     * @returns the identifier number
     */
    int appendIdentifier(Identifier * identifier) {
        return m_doc->appendIdentifier(* (identifier->kEduVocIdentifier()));
    }

    /*
     * Sets the identifier of translation
     *
     * @param index            number of translation 0..x
     * @param lang             thr language identifier: en=english, de=german, ...
     */
//             void setIdentifier ( int index, Identifier * lang ) { m_doc->setIdentifier ( index,* ( lang->kEduVocIdentifier() ) ); }

    /**
     * Returns the identifier of translation @p index
     *
     * @param index            number of translation 0..x
     * @returns                the language identifier: en=english, de=german, ...
     */
    QObject * identifier(int index) {
        return new Identifier(m_doc->identifier(index));
    }

    /**
     * Removes identifier and the according translations in all entries
     *
     * @param index            number of translation 0..x
     */
    void removeIdentifier(int index) {
        m_doc->removeIdentifier(index);
    }

    /** Returns a list of all the identifiers of this document */
    QVariantList identifiers();

    /*
     * Determines the index of a given identifier
     *
     * @param lang             identifier of language
     * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
     */
// not implemented in KEduVocDocument
//             int indexOfIdentifier ( const QString &name ) const { return m_doc->indexOfIdentifier(name); }


    // *** grade methods ***

    /*
     * Retrieves the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
//             KDE_DEPRECATED void queryIdentifier ( QString &org, QString &trans ) const;

    /*
     * Sets the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
//             KDE_DEPRECATED void setQueryIdentifier ( const QString &org, const QString &trans );

    // *** lesson methods ***

    /* get the lesson root object
     * @returns a pointer to the lesson object
     */
//             KEduVocLesson * lesson();

//             KEduVocWordType * wordTypeContainer();

//             KEduVocLeitnerBox * leitnerContainer();

    // *** file format specific methods ***


    static KEduVocDocument::FileType detectFileType(const QString &fileName) {
        return KEduVocDocument::detectFileType(fileName);
    }

    /**
     * Create a string with the supported document types, that can be used
     * as filter in KFileDialog. It includes also an entry to match all the
     * supported types.
     *
     * @param mode             the mode for the supported document types. See FileDialogMode enum
     * @returns                the filter string
     */
    static QString pattern(KEduVocDocument::FileDialogMode mode) {
        return KEduVocDocument::pattern(mode);
    }

    /** Returns a more detailed description of the @p errorCode given */
    static QString errorDescription(int errorCode) {
        return KEduVocDocument::errorDescription(errorCode);
    }

private:
    KEduVocDocument * m_doc;
};

}
#endif
