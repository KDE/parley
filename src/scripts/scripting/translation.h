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
#ifndef SCRIPTINGSCRIPTOBJECTTRANSLATION_H
#define SCRIPTINGSCRIPTOBJECTTRANSLATION_H

#include "keduvoctranslation.h"
#include "keduvocwordtype.h"
#include "expression.h"
#include "text.h"
// #include "document.h"

#include <QObject>

namespace Scripting
{

    /**
        KEduVocTranslation wrapper class for scripting with Kross

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Translation : public Text
    {
            Q_OBJECT
            Q_PROPERTY ( QString pronunciation READ pronunciation WRITE setPronunciation )
            Q_PROPERTY ( QString comment READ comment WRITE setComment )
            Q_PROPERTY ( QString example READ example WRITE setExample )
            Q_PROPERTY ( QString paraphrase READ paraphrase WRITE setParaphrase )
//             Q_PROPERTY ( QString antonym READ antonym WRITE setAntonym )
//             Q_PROPERTY ( QString irregularPlural READ irregularPlural WRITE setIrregularPlural )
            Q_PROPERTY ( QString comparative READ comparative WRITE setComparative )
            Q_PROPERTY ( QString superlative READ superlative WRITE setSuperlative )
//             Q_PROPERTY ( QStringList multipleChoice READ multipleChoice )
            Q_PROPERTY ( QString soundUrl READ soundUrl WRITE setSoundUrl )
//             Q_PROPERTY ( QVariantList falseFriends READ falseFriends )
//             Q_PROPERTY ( QVariantList synonyms READ synonyms )
//             Q_PROPERTY ( QVariantList antonyms READ antonyms )
        public:
            /**
             * Default constructor for an empty translation.
             */
            Translation ( Expression * entry );

            /**
             * Constructor
             * @param translation is used as translation
             */
            Translation ( Expression * entry, const QString & translation );

            /**
             * Constructor from KEduVocTranslation (not used by scripts)
             * @param translation KEduVocTranslation to initialize Scripting::Translation
             */
            Translation ( KEduVocTranslation * translation );

            ~Translation();

            KEduVocTranslation* kEduVocTranslation() { return m_translation; }

            template <class T, class S>
            QVariantList toVariantList ( QList<T*> objList ) const;

            //Property: pronunciation (see KEduVocTranslation for details)
            QString pronunciation() const { return m_translation->pronunciation(); }
            void setPronunciation ( const QString & expression ) { m_translation->setPronunciation ( expression ); }

            //Property: comment (see KEduVocTranslation for details)
            QString comment() const { return m_translation->comment(); }
            void setComment ( const QString & expr ) { m_translation->setComment ( expr ); }


            void setExample ( const QString & expression ) { m_translation->setExample ( expression ); }

            /** returns example of this expression
            * @return                 example or "" if no string available
            */
            QString example() const { return m_translation->example(); }

            /** sets paraphrase of this expression
            * @param expression       paraphrase of this index
            */
            void setParaphrase ( const QString & expression ) { m_translation->setParaphrase ( expression ); }

            /** returns paraphrase of this expression
            * @return                 paraphrase or "" if no string available
            */
            QString paraphrase() const { return m_translation->paraphrase(); }

            /** sets antonym this expression
            * @param expression       antonym of this index
            */
//             void setAntonym ( const QString & expression ) { m_translation->setAntonym ( expression ); }

            /** returns antonym of this expression
            * @return                 antonym or "" if no string available
            */
//             QString antonym() const { return m_translation->antonym(); }

            /**
             * Set a irregular plural form.
             * @param plural plural of the word
             */
//             void setIrregularPlural ( const QString& plural ) { m_translation->setIrregularPlural( plural ); }

            /**
             * Get a irregular plural form.
             * @return plural of the word
             */
//             QString irregularPlural() const { return m_translation->irregularPlural(); }

            /** sets the word type of this expression
            * @param type             type of this expression ("" = none)
            */
            void setWordType ( KEduVocWordType* wordType ) { m_translation->setWordType ( wordType ); }

            /** returns the leitner box of this translation
             * @return the box
             */
//             KEduVocLeitnerBox* leitnerBox() const;

            /** sets the leitner box of this translation
             * @param leitnerBox the box
             */
//             void setLeitnerBox ( KEduVocLeitnerBox* leitnerBox );

            /**
             * returns a conjugation if available
             * @param tense tense of the requested conjugation
             * @return the conjugation
             */
//             KEduVocConjugation& conjugation ( const QString& tense );

            /**
             * Returns a pointer to the declension object of this translation.
             * Returns 0 if no declension object exists!
             * @return the declension
             */
//             KEduVocDeclension* declension();

            /**
             * Set a new declension for a translation
             * @param declension
             */
//             void setDeclension ( KEduVocDeclension* declension );

            /**
             * Bad, only used for tense entry page, will be deleted later. Deprecated.
             * @param conjugation
             */
//             void setConjugations ( const QMap<QString, KEduVocConjugation>& conjugations );



            /**
             * Bad, only compatibility. Deprecated.
             * @return
             */
//             QMap <QString, KEduVocConjugation> conjugations() const;

            /**
             * Comparison forms of adjectives/adverbs.
             */
            QString comparative() const { return m_translation->comparative(); }
            void setComparative ( const QString& comparative ) { m_translation->setComparative ( comparative ); }
            QString superlative() const { return m_translation->superlative(); }
            void setSuperlative ( const QString& superlative ) { m_translation->setSuperlative ( superlative ); }

            /** sets multiple choice
             * @param mc               multiple choice block
              */
///@note commented out in keduvoctranslation.h
//     void setMultipleChoice( const QStringList &mc );

            /** get the sound url for this translation if it exists */
            QString soundUrl() { return m_translation->soundUrl().path(); }

            /** set the sound url for this translation
             * @param url               url of the sound file */
            void setSoundUrl ( const QString &url ) { m_translation->setSoundUrl ( url ); }

            /** get the image url for this translation if it exists */
            QString imageUrl() { return m_translation->soundUrl().path(); }

            /** set the image url for this translation
             * @param url               url of the image
             */
            void setImageUrl ( const QString &url ) { m_translation->setImageUrl ( url ); }

            /**
             * Equal operator to assing a translation to another one.
             * @param translation translation to be copied
             * @return reference to the new translation
             */
//             KEduVocTranslation& operator= ( const KEduVocTranslation &translation );

            /**
             * Compare two translations, including word type etc.
             * @param translation
             * @return true if equal
             */
//             bool operator== ( const KEduVocTranslation &translation ) const;

//             void fromKVTML2 ( QDomElement& parent );
//             void toKVTML2 ( QDomElement& parent );
        public slots:
            /**
             * add a false friend
             * @param falseFriend false friend of this index
             */
            void addFalseFriend ( Translation* falseFriend ) { m_translation->addFalseFriend ( falseFriend->kEduVocTranslation() ); }

            /**
             * remove a false friend
             * @param falseFriend false friend of this index
             */
            void removeFalseFriend ( Translation* falseFriend ) { m_translation->removeFalseFriend ( falseFriend->kEduVocTranslation() ); }

            /**
             * add a synonym
             * @param synonym
             */
            void addSynonym ( Translation* synonym ) { m_translation->addSynonym ( synonym->kEduVocTranslation() ); }

            /**
             * remove a synonym
             * @param synonym
             */
            void removeSynonym ( Translation* synonym ) { m_translation->removeSynonym ( synonym->kEduVocTranslation() ); }

            /**
             * add a antonym
             * @param antonym
                 */
            void addAntonym ( Translation* antonym ) { m_translation->addAntonym ( antonym->kEduVocTranslation() ); }

            /**
             * remove a antonym
             * @param antonym
             */
            void removeAntonym ( Translation* antonym ) { m_translation->removeAntonym ( antonym->kEduVocTranslation() ); }

            /**
             * returns false friends of this expression
             * @return list of false friends
             */
            QVariantList falseFriends() const { return toVariantList<KEduVocTranslation,Translation> ( m_translation->falseFriends() ); }

            /**
             * returns synonyms of this expression
             * @return synonyms
             */
            QVariantList synonyms() const { return toVariantList<KEduVocTranslation,Translation> ( m_translation->synonyms() ); }

            /**
             * returns antonyms of this expression
             * @return antonyms
             */
            QVariantList antonyms() const { return toVariantList<KEduVocTranslation,Translation> ( m_translation->antonyms() ); }

            /** returns the word type of this expression
            *
            * @return                 type or "" if no type available
            */
            QString wordType() const;

            /** returns multiple choice if available */
            QStringList multipleChoice() { return m_translation->multipleChoice(); }

            /** returns a string list with the available conjugation tenses */
            QStringList conjugationTenses() const { return m_translation->conjugationTenses(); }

            /** adds conjugations or replaces them, if they exist.
            * @param conjugation      conjugation
            */
            void setConjugation ( const QString& conjugation, const QString& tense, const QString & number, const QString & person );

        private:
            KEduVocTranslation * m_translation;

    };

    //Template functions should not be separated from their definition (must be in the header file)
    template <class T, class S>
    QVariantList Translation::toVariantList ( QList<T*> objList ) const
    {
        QVariantList list;
        foreach ( T * t, objList )
        {
            QObject * obj = new S ( t );
            list.push_back ( qVariantFromValue ( obj ) );
        }
        return list;
    }


}

#endif
