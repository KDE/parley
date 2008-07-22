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

#include <QObject>

namespace Scripting
{

    /**
    Implements the KEduVocIdentifier class

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
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
             * @param gender Article gender. Values: "Masculine", "Feminine", "Neutral"
             * @return A string containing the requested article. Empty string if does not exist
             */
            QString article ( const QString& number,const QString& definite,const QString& gender );

            /**
             * Sets the articles for this identifier
             * @param art              article block
             */
//             void setArticle ( const KEduVocArticle& article );

            /**
             * Sets the article in the given @p number, definiteness (@p definite) and @p gender
             * @param article The article to set
             * @param number Article number. Values: "Singular", "Dual", "Plural"
             * @param definite Article definiteness. Values: "Definite", "Indefinite"
             * @param gender Article gender. Values: "Masculine", "Feminine", "Neutral"
             */
            void setArticle ( const QString& article, const QString& number,const QString& definite,const QString& gender );

            /**** Personal Pronoun Functions ****/

            /**
             * Get the personal pronouns for this identifier
             * @returns a KEduVocPersonalPronoun containing the personal pronouns
             */
//             KEduVocPersonalPronoun personalPronouns() const;

            QString personalPronoun ( const QString & number, const QString & person );


            /**
             * Sets personal pronouns
             * @param pronouns a KEduVocConjugation containing the personal pronouns
             */
//             void setPersonalPronouns ( const KEduVocPersonalPronoun &pronouns );

            void setPersonalPronoun ( const QString& personalPronoun, const QString & number, const QString & person );


        private:
            KEduVocIdentifier * m_identifier;
    };

}

#endif
