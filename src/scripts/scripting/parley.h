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

namespace Scripting
{


    /**
    Implements the object that will be given to Kross scripts to use for accessing parley features

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Parley : public QObject
    {
            Q_OBJECT
            /// Read-only property of the active document
            Q_PROPERTY ( QObject * document READ getDocument )
            /// Abreviation of document property (same as Parley.document)
            Q_PROPERTY ( QObject * doc READ getDocument )

            /** Read-Only property that gives the active of the lesson (the one that is currently open by Parley)
            * @code
            * import Parley
            * activelesson = Parley.activeLesson
            * @endcode
            */
            Q_PROPERTY ( QObject * activeLesson READ getActiveLesson )

            Q_ENUMS ( Number Case Person Gender Definiteness)

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
                Third
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

            /**
             * Emits translateWord signal so the script function connected to it translate the @p word
             * @param word Word to translate
             * @param fromLanguage The language of @p word to translate from
             * @param toLanguage The language you want to translate to
             */
            void callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
            void callTest() { emit test(); }
            void setTranslator ( Translator* translator ) { m_translator = translator; }
            QObject* getDocument() { return m_doc; }
            QObject* getActiveLesson();

        public Q_SLOTS:
            void addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation );
            /**
             * Returns a list of all available language codes (to be used by the scripts)
             * @return
             */
            QStringList languageCodes();

            /**
             * Gives the language name of the given @p code language code.
             * @param code Language code
             * @return Language name
             */
            QString languageCodeToName ( QString code );

            /**
             * Open the Parley Document @p file
             * Usage:
             * @code
             * import Parley
             * Parley.open("Vocab/MyVocab.kvtml")
             * @endcode
             * @param file Parley Document file path (ex. /home/kde-user/MyVocab.kvtml)
             */
            void open ( QString filename );

            QString testFunction() { return "testFunction"; }

//             void updateDocument() { m_parleyApp->updateDocument(); }
            QObject * newDocument() { return new Document(); }

            QObject * newAction ( const QString & name );

            void listTest ( QString list ) { kDebug() << list; }

            void listTest ( QStringList list ) { kDebug() << list; }

            void whichEnum ( KEduVocDeclension::DeclensionNumber d )
            {
                kDebug() << d;
            }


        Q_SIGNALS:
            void translateWord ( QString word, QString fromLanguage, QString toLanguage );
            void translationFinished ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
            void translationStarted ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
            void test();



        private:
            Translator* m_translator;
            Document* m_doc;
            ParleyApp * m_parleyApp;
    };

}
#endif
