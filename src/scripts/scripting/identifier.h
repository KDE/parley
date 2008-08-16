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
#ifndef SCRIPTINGIDENTIFIER_H
#define SCRIPTINGIDENTIFIER_H

#include <keduvocidentifier.h>
#include <keduvocwordflags.h>

#include <QObject>

namespace Scripting
{

    /**
     * Identifier class can be accessed from Document class and is used for specifying the document languages, articles and personal pronouns.
     *
    @code
    #how to specify the articles and personal pronouns of a language (German in this example)
    import Parley
    newid = Parley.doc.newIdentifier()
    newid.name = "German"
    newid.locale = "de"
    newid.setArticle("der",Parley.Singular,Parley.Definite,Parley.Masculine)
    newid.setArticle("die",Parley.Singular,Parley.Definite,Parley.Feminine)
    newid.setArticle("das",Parley.Singular,Parley.Definite,Parley.Neutral)
    newid.setArticle("ein",Parley.Singular,Parley.Indefinite,Parley.Masculine)
    newid.setArticle("eine",Parley.Singular,Parley.Indefinite,Parley.Feminine)
    newid.setArticle("ein",Parley.Singular,Parley.Indefinite,Parley.Neutral)
    newid.setPersonalPronoun("ich",Parley.Singular,Parley.First)
    newid.setPersonalPronoun("du",Parley.Singular,Parley.Second)
    newid.setPersonalPronoun("er",Parley.Singular,Parley.ThirdMale)
    newid.setPersonalPronoun("sie",Parley.Singular,Parley.ThirdFemale)
    newid.setPersonalPronoun("es",Parley.Singular,Parley.Third)
    newid.setPersonalPronoun("wir",Parley.Plural,Parley.First)
    newid.setPersonalPronoun("ihr",Parley.Plural,Parley.Second)
    newid.setPersonalPronoun("sie",Parley.Plural,Parley.Third)
    print newid.personalPronouns()
    index = Parley.doc.appendIdentifier(newid)
    @endcode
     *
     *   @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
     */
    class Identifier : public QObject
    {
            Q_OBJECT
            Q_PROPERTY ( QString name READ name WRITE setName )
            Q_PROPERTY ( QString locale READ locale WRITE setLocale )
        public:

            Identifier ();

            Identifier ( KEduVocIdentifier * identifier );

            Identifier ( KEduVocIdentifier & identifier );

            ~Identifier();

            KEduVocIdentifier * kEduVocIdentifier() { return m_identifier; }

            /**
                * Name of this identifier. (English, Anatomy, Fruit salad...)
                * @return name
                */
            QString name() const { return m_identifier->name(); }
            /**
             * Set the name
             * @param name
             */
            void setName ( const QString& name ) { m_identifier->setName ( name ); }

            /**
             * The locale of the contents: en, de, es, ...
             * @return locale
             */
            QString locale() const { return m_identifier->locale(); }
            /**
             * Set the locale
             * @param name
             */
            void setLocale ( const QString& name ) { m_identifier->setLocale ( name ); }

        public Q_SLOTS:

            /**** Article Functions ****/

            /**
             * Articles (a, the in English, el, la,... in Spanish)
             * @returns articles
             */
//             KEduVocArticle article() const;

            /**
             * Returns the article in the given @p number, definiteness (@p definite) and @p gender
             * @param number Article number. Values: "Singular", "Dual", "Plural"
             * @param definite Article definiteness. Values: "Definite", "Indefinite"
             * @param gender Article gender. Values: "Masculine", "Feminine", "Neuter"
             * @return A string containing the requested article. Empty string if does not exist
             */
            QString article ( const KEduVocWordFlags& flags );

            /**
             * Sets the article in the given @p number, definiteness (@p definite) and @p gender
             * @param article The article to set
             * @param number Article number. Values: "Singular", "Dual", "Plural"
             * @param definite Article definiteness. Values: "Definite", "Indefinite"
             * @param gender Article gender. Values: "Masculine", "Feminine", "Neuter"
             */
            void setArticle ( const QString& article, const KEduVocWordFlags& flags );

            /**** Personal Pronoun Functions ****/

            /**
             * Get the personal pronouns for this identifier
             * @returns a KEduVocPersonalPronoun containing the personal pronouns
             */
//             KEduVocPersonalPronoun personalPronouns() const;

//              QString personalPronoun ( const QString & number, const QString & person );
            QString personalPronoun ( const KEduVocWordFlags& flags ) const;


            /**
             * Sets personal pronouns
             * @param pronouns a KEduVocConjugation containing the personal pronouns
             */
//             void setPersonalPronouns ( const KEduVocPersonalPronoun &pronouns );

//             void setPersonalPronoun ( const QString& personalPronoun, const QString & number, const QString & person );
            void setPersonalPronoun ( const QString& conjugation, const KEduVocWordFlags& flags );

            QStringList personalPronouns ();

            /**
            * Sets the document tenses
            * @code
            * #how to add new tenses to a language
            * import Parley
            * tenses = Parley.doc.tenses()
            * print tenses
            * tenses.append("Present Perfect")
            * tenses.append("Past Simple")
            * Parley.doc.setTenses(tenses)
            * print Parley.doc.tenses()
            * @endcode
            * @param names A string list of the document tenses we want to be using
* @todo Change the example for this function
            */
            void setTenses ( QStringList names ) { m_identifier->setTenseList ( names ); }

            /**
             * Gets the language tenses
             * @return A string list of all the language tenses
             */
            QStringList tenses() const { return m_identifier->tenseList(); }


        private:
            KEduVocIdentifier * m_identifier;
    };

}

#endif
