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
#ifndef SCRIPTOBJECTPARLEY_H
#define SCRIPTOBJECTPARLEY_H

#include "document.h"

#include <QObject>
#include <KAction>
#include <KDebug>

#include <keduvocdeclension.h>

class ParleyApp;
class Translator;

/**
 * Namespace that contains all the classes exposed to Kross scripts.
 *
 * Main entry point is the Scripting::Parley class which allows us to access all the other Scripting classes (see code below).
 *
 * Class Hierarchy:
 *
 * - Parley
 *   - Document
 *     - Lesson (inherits Container)
 *       - Expression (known as Entry in functions)
 *         - Translation (inherits Text)
 *   - Identifier
 */
namespace Scripting
{


    /**
     * Parley class is the main entry point of Parley scripting classes. Through it you can access the Document class (Parley.doc or Parley.document) which provides functionality for viewing/modifying a %Parley document (%KEduVocDocument), that means access lessons, entries, document languages etc.
     *
     * The Parley class has to do more with the active %Parley application. Here it follows a list of possible usages of Parley class:
     *
     * - Add a new Action to the script menu (see Parley::newAction() function)
     * - Add a new translation script (see Parley::translateWord() signal)
     * - Have direct access to the active lesson (see Parley::activeLesson property)
     * - Have access to various enumerations (see Parley::Number, Parley::Case, Parley::Person, Parley::Gender and Parley::Definiteness enumerations)
     * - Create a new %Parley %Document (see Parley::newDocument() function)
     *
     * Signals and Slots: To connect a script function (slot) to a signal you <b> just define a function with the same name as the signal </b> or use the Parley.connect function:
     * @code
     * #how to connect a function to a signal (example with Parley::translateWord() signal)
     * def translateFromInternet(word,fromLang,toLang):
     *     print "Translating from Internet!!.."
     *
     * Parley.connect("translateWord(const QString &,const QString &,const QString &)",translateFromInternet)
     * @endcode
     *
     *   @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
     */
    class Parley : public QObject
    {
            Q_OBJECT
            /// Read-only property of the active document
            Q_PROPERTY ( QObject * document READ getDocument )
            /// Abreviation of document property (same as Parley.document)
            Q_PROPERTY ( QObject * doc READ getDocument )
            /// Currently active lesson
            Q_PROPERTY ( QObject * activeLesson READ getActiveLesson )

            Q_ENUMS ( Number Case Person Gender Definiteness )

        public:
            ///Conjugation/Declension number
            enum Number
            {
                Singular = 0,
                Dual,
                Plural
            };

            ///Declension case
            enum Case
            {
                Nominative = 0,
                Genitive,
                Dative,
                Accusative,
                Ablative,
                Locative,
                Vocative
            };

            ///Conjugation Person
            enum Person
            {
                First,
                Second,
                ThirdMale,
                ThirdFemale,
//                 ThirdNeutralCommon
                Third ///use this for Third Neutral form as well
            };

            ///Conjugation gender
            enum Gender
            {
                Masculine,
                Feminine,
                Neutral
            };

            ///Article Definiteness
            enum Definiteness
            {
                Definite,
                Indefinite
            };

            Parley ( ParleyApp * parley );

            ~Parley();

            /*
             * Emits translateWord signal so the script function connected to it translate the @p word
             * @param word Word to translate
             * @param fromLanguage The language of @p word to translate from
             * @param toLanguage The language you want to translate to
             */
            void callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage );

            //sets the translator object
            void setTranslator ( Translator* translator ) { m_translator = translator; }

            //Returns the active document
            QObject* getDocument() { return m_doc; }

            //Returns the active lesson
            QObject* getActiveLesson();

        public Q_SLOTS:

            /**
             * Adds the found @p translation of the @p word from language @p fromLanguage to language @p toLanguage to %Parley translations to be used for translating lesson entries (or anything else). This function is ment to be used by scripts to add a translation of a word by parsing either online or offline dictionaries.
             *
             * @code
             * #example usage of addTranslation function
             * import Parley
             * #function called by Parley whenever a translation of a word is needed
             * def translateWord(word,fromLang,toLang):
             *     <<look for the word translation>>
             *     Parley.addTranslation(word,fromLang,toLang,foundWord)
             *     <<look for more translations>>
             * @endcode
             *
             * @param word Translated word
             * @param fromLanguage From language
             * @param toLanguage To language
             * @param translation %Translation of word
             */
            void addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation );
            /**
             * Returns a list of all available locales (to be used by the scripts)
             * @return
             */
            QStringList locales();

            /**
             * Gives the language name of the given @p locale
             * @param locale Language locale
             * @return Language name
             */
            QString localeName ( QString locale );

            /// @todo Make this function working (not very important function)
            /*
             * Open the Parley Document @p file
             * Usage:
             * @code
             * import Parley
             * Parley.open("Vocab/MyVocab.kvtml")
             * @endcode
             * @param file Parley Document file path (ex. /home/kde-user/MyVocab.kvtml)
             */
            void open ( QString filename );

//             void updateDocument() { m_parleyApp->updateDocument(); }

            /**
             * Creates a new document and returns a reference to it
             *
             * @code
             * #how to create a new document, add lessons, add entries and save it to a kvtml file
             * import Parley
             *
             * #create new document
             * doc = Parley.newDocument()
             * doc.title = "New document"
             *
             * #set identifiers
             * doc.appendNewIdentifier("English","en_US")
             * doc.appendNewIdentifier("French","fr")
             *
             * #lessons
             * l1 = doc.newLesson("Lesson1")
             * doc.rootLesson.appendChildLesson(l1)
             *
             * #add a new entry (first way)
             * e = l1.newEntry()
             * e.setTranslation(0,"dog")
             * e.setTranslation(1,"chien")
             * l1.appendEntry(e)
             *
             * #add a new entry (second way)
             * ee = l1.newEntry(["glass","verre"])
             * l1.appendEntry(ee)
             * #third way
             * ee = l1.appendNewEntry(["book","livre"])
             *
             * #add a new lesson (fast way)
             * l2 = doc.appendNewLesson("Lesson 2")
             *
             * #add a new child lesson under "Lesson 2"
             * l3 = doc.appendNewLesson("Lesson 3",l2)
             *
             * #add a new entry (third way)
             * l2.appendNewEntry(["I","je"]);
             * l3.appendNewEntry(["good morning","bonjour"])
             *
             * #save document
             * doc.saveAs("/home/kde-devel/test_new_document.kvtml")
             * @endcode
             *
             * @return A Document object, the newly created lesson
             */
            QObject * newDocument() { return new Document(); }

            /**
             * Creates and adds to the Scripts menu a new KAction (see KAction documentation)
             *
             * @code
             * #how to add two new Scripts menu entries
             * import Parley
             *
             * def convertLessonToPDF():
             *     print "Converting lesson to PDF.."
             *
             * def convertLessonToHTML():
             *     print "Converting lesson to HTML.."
             *
             * #one way of creating a new action
             * newaction_pdf = Parley.newAction("convert_lesson_pdf")
             * newaction_pdf.text="Convert Lesson to PDF"
             * Parley.connect(newaction_pdf,"triggered()",convertLessonToPDF)
             *
             * #second way of creating a new action (short)
             * newaction_html = Parley.newAction("convert_lesson_html","Convert Lesson to HTML")
             * Parley.connect(newaction_html,"triggered()",convertLessonToHTML)
             * @endcode
             *
             * @param name Unique action name
             * @param text Action's text (what text appears in the scripts menu)
             * @return A reference to a KAction object (accessible by scripts)
             */
            QObject * newAction ( const QString & name, const QString & text = QString() );

        Q_SIGNALS:
            /**
             * Slots (script functions) connected to this signal are called when a translation of @p word is requested
             * @param word Word to translate
             * @param fromLanguage The language of @p word to translate from
             * @param toLanguage The language you want to translate to
             */
            void translateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
            /* Emits when the translation of a word is finished (from all the scripts) [not to be used by scripts] */
            void translationFinished ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
            /* Emits when the translation of a word starts [not to be used by scripts] */
            void translationStarted ( const QString & word,const QString& fromLanguage,const QString& toLanguage );

        private:
            Translator* m_translator;
            Document* m_doc;
            ParleyApp * m_parleyApp;
    };

}
#endif
