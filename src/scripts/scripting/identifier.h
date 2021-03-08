/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTINGIDENTIFIER_H
#define SCRIPTINGIDENTIFIER_H

#include <KEduVocIdentifier>
#include <KEduVocWordFlags>

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
newid.setArticle("der",Parley.Singular|Parley.Definite|Parley.Masculine)
newid.setArticle("die",Parley.Singular|Parley.Definite|Parley.Feminine)
newid.setArticle("das",Parley.Singular|Parley.Definite|Parley.Neuter)
newid.setArticle("ein",Parley.Singular|Parley.Indefinite|Parley.Masculine)
newid.setArticle("eine",Parley.Singular|Parley.Indefinite|Parley.Feminine)
newid.setArticle("ein",Parley.Singular|Parley.Indefinite|Parley.Neuter)
newid.setPersonalPronoun("ich",Parley.Singular|Parley.First)
newid.setPersonalPronoun("du",Parley.Singular|Parley.Second)
newid.setPersonalPronoun("er",Parley.Singular|Parley.Third|Parley.Masculine)
newid.setPersonalPronoun("sie",Parley.Singular|Parley.Third|Parley.Feminine)
newid.setPersonalPronoun("es",Parley.Singular|Parley.Third|Parley.Neuter)
newid.setPersonalPronoun("wir",Parley.Plural|Parley.First)
newid.setPersonalPronoun("ihr",Parley.Plural|Parley.Second)
newid.setPersonalPronoun("sie",Parley.Plural|Parley.Third)
index = Parley.doc.appendIdentifier(newid)
@endcode
 *
 *   @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Identifier : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString locale READ locale WRITE setLocale)
public:

    Identifier();

    explicit Identifier(KEduVocIdentifier * identifier);

    explicit Identifier(KEduVocIdentifier & identifier);

    ~Identifier();

    KEduVocIdentifier * kEduVocIdentifier() {
        return m_identifier;
    }

    /**
        * Name of this identifier. (English, Anatomy, Fruit salad...)
        * @return name
        */
    QString name() const {
        return m_identifier->name();
    }
    /**
     * Set the name
     * @param name
     */
    void setName(const QString& name) {
        m_identifier->setName(name);
    }

    /**
     * The locale of the contents: en, de, es, ...
     * @return locale
     */
    QString locale() const {
        return m_identifier->locale();
    }
    /**
     * Set the locale
     * @param name
     */
    void setLocale(const QString& name) {
        m_identifier->setLocale(name);
    }

public Q_SLOTS:

    /**** Article Functions ****/

    /**
     * Articles (a, the in English, el, la,... in Spanish)
     * @returns articles
     */
//             KEduVocArticle article() const;

    /**
     * Returns the article in the given number, definiteness and gender flags:
     *
     * - Number flags: Parley.Singular, Parley.Dual, Parley.Plural
     * - Definiteness flags: Parley.Definite, Parley.Indefinite
     * - Gender flags: Parley.Masculine, Parley.Feninine, Parley.Neuter
     *
     * @param flags Flags to indicate which article to return
     * @return A string containing the requested article. Empty string if does not exist
     */
    QString article(const KEduVocWordFlags& flags);

    /**
     * Sets the @p article in the given number, definiteness and gender flags:
     *
     * - Number flags: Parley.Singular, Parley.Dual, Parley.Plural
     * - Definiteness flags: Parley.Definite, Parley.Indefinite
     * - Gender flags: Parley.Masculine, Parley.Feninine, Parley.Neuter
     *
     * @param article The article to set
     * @param flags Flags to indicate which article to set
     */
    void setArticle(const QString& article, const KEduVocWordFlags& flags);

    /**** Personal Pronoun Functions ****/

    /**
     * Get the personal pronouns for this identifier
     * @returns a KEduVocPersonalPronoun containing the personal pronouns
     */
//             KEduVocPersonalPronoun personalPronouns() const;

//              QString personalPronoun ( const QString & number, const QString & person );
    QString personalPronoun(const KEduVocWordFlags& flags) const;


    /**
     * Sets personal pronouns
     * @param conjugation a conjugation of the personal pronoun
     * @param flags the KEduVocWordFlags flags
     */
//             void setPersonalPronouns ( const KEduVocPersonalPronoun &pronouns );

//             void setPersonalPronoun ( const QString& personalPronoun, const QString & number, const QString & person );
    void setPersonalPronoun(const QString& conjugation, const KEduVocWordFlags& flags);

    QStringList personalPronouns();

    /**
    * Sets the document tenses
    * @code
    * #how to add new tenses to a language
    * import Parley
    * for ident in Parley.doc.identifiers():
    *    T = ident.tenses()
    *    print T
    *    T.append("Present Perfect")
    *    T.append("Past Simple")
    *    T.append("Past Perfect")
    *    ident.setTenses(T)
    *    print ident.tenses()
    * @endcode
    * @param names A string list of the document tenses we want to be using
    */
    void setTenses(const QStringList &names) {
        m_identifier->setTenseList(names);
    }

    /**
     * Gets the language tenses (see example in Identifier::setTenses())
     * @return A string list of all the language tenses
     */
    QStringList tenses() const {
        return m_identifier->tenseList();
    }


private:
    KEduVocIdentifier * m_identifier;
};

}

#endif
