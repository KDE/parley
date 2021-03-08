/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTINGSCRIPTOBJECTTRANSLATION_H
#define SCRIPTINGSCRIPTOBJECTTRANSLATION_H

#include <KEduVocDeclension>
#include <KEduVocConjugation>

#include <KEduVocTranslation>
#include <KEduVocWordtype>
#include "expression.h"
#include "text.h"

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
    /// Word pronunciation
    Q_PROPERTY(QString pronunciation READ pronunciation WRITE setPronunciation)
    /// Comment on this translation
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    /// Example sentence
    Q_PROPERTY(QString example READ example WRITE setExample)
    /// Paraphrase
    Q_PROPERTY(QString paraphrase READ paraphrase WRITE setParaphrase)
//             Q_PROPERTY ( QString antonym READ antonym WRITE setAntonym )
//             Q_PROPERTY ( QString irregularPlural READ irregularPlural WRITE setIrregularPlural )
    /// Comparative form
    Q_PROPERTY(QString comparative READ comparative WRITE setComparative)
    /// Superlative form
    Q_PROPERTY(QString superlative READ superlative WRITE setSuperlative)
    /// Url of the sound file
    Q_PROPERTY(QString soundUrl READ soundUrl WRITE setSoundUrl)
    /// Url of the image file
    Q_PROPERTY(QString imageUrl READ soundUrl WRITE setImageUrl)
public:
    /*
     * Default constructor for an empty translation.
     */
    explicit Translation(Expression * entry);

    /*
     * Constructor
     * @param translation is used as translation
     */
    Translation(Expression * entry, const QString & translation);

    /*
     * Constructor from KEduVocTranslation (not used by scripts)
     * @param translation KEduVocTranslation to initialize Scripting::Translation
     */
    explicit Translation(KEduVocTranslation * translation);

    ~Translation();

    KEduVocTranslation* kEduVocTranslation() {
        return m_translation;
    }

    template <class T, class S>
    QVariantList toVariantList(QList<T*> objList) const;

//             static QMap<QString,KEduVocConjugation::ConjugationNumber> & getConjNumberMap ();

//             static QMap<QString,KEduVocConjugation::ConjugationPerson> & getConjPersonMap ();

    /* returns the pronunciation of this translation */
    QString pronunciation() const {
        return m_translation->pronunciation();
    }
    /* sets the pronunciation of this expression */
    void setPronunciation(const QString & expression) {
        m_translation->setPronunciation(expression);
    }

    /* returns the translation comment */
    QString comment() const {
        return m_translation->comment();
    }
    /* sets the translation comment */
    void setComment(const QString & expr) {
        m_translation->setComment(expr);
    }

    /* returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const {
        return m_translation->example();
    }

    /* sets example of this expression */
    void setExample(const QString & expression) {
        m_translation->setExample(expression);
    }

    /* sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase(const QString & expression) {
        m_translation->setParaphrase(expression);
    }

    /* returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const {
        return m_translation->paraphrase();
    }

    /* sets antonym this expression
    * @param expression       antonym of this index
    */
//             void setAntonym ( const QString & expression ) { m_translation->setAntonym ( expression ); }

    /* returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
//             QString antonym() const { return m_translation->antonym(); }

    /*
     * Set a irregular plural form.
     * @param plural plural of the word
     */
//             void setIrregularPlural ( const QString& plural ) { m_translation->setIrregularPlural( plural ); }

    /*
     * Get a irregular plural form.
     * @return plural of the word
     */
//             QString irregularPlural() const { return m_translation->irregularPlural(); }

    /* sets the word type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setWordType(KEduVocWordType* wordType) {
        m_translation->setWordType(wordType);
    }

    /* returns the leitner box of this translation
     * @return the box
     */
//             KEduVocLeitnerBox* leitnerBox() const;

    /* sets the leitner box of this translation
     * @param leitnerBox the box
     */
//             void setLeitnerBox ( KEduVocLeitnerBox* leitnerBox );

    /*
     * Bad, only used for tense entry page, will be deleted later. Deprecated.
     * @param conjugation
     */
//             void setConjugations ( const QMap<QString, KEduVocConjugation>& conjugations );



    /*
     * Bad, only compatibility. Deprecated.
     * @return
     */
//             QMap <QString, KEduVocConjugation> conjugations() const;

    /*
     * Comparison forms of adjectives/adverbs.
     */
    QString comparative() const {
        return m_translation->comparativeForm().text();
    }
    void setComparative(const QString& comparative) {
        m_translation->setComparativeForm(comparative);
    }
    QString superlative() const {
        return m_translation->superlativeForm().text();
    }
    void setSuperlative(const QString& superlative) {
        m_translation->setSuperlativeForm(superlative);
    }

    /* sets multiple choice
     * @param mc               multiple choice block
      */
//@note commented out in keduvoctranslation.h
//     void setMultipleChoice( const QStringList &mc );

    /* get the sound url for this translation if it exists */
    QString soundUrl() {
        return m_translation->soundUrl().toString();
    }

    /* set the sound url for this translation
     * @param url               url of the sound file */
    void setSoundUrl(const QString &url) {
        m_translation->setSoundUrl(QUrl(url));
    }

    /* get the image url for this translation if it exists */
    QString imageUrl() {
        return m_translation->soundUrl().toString();
    }

    /* set the image url for this translation
     * @param url               url of the image
     */
    void setImageUrl(const QString &url) {
        m_translation->setImageUrl(QUrl(url));
    }

    /*
     * Equal operator to assing a translation to another one.
     * @param translation translation to be copied
     * @return reference to the new translation
     */
//             KEduVocTranslation& operator= ( const KEduVocTranslation &translation );

    /*
     * Compare two translations, including word type etc.
     * @param translation
     * @return true if equal
     */
//             bool operator== ( const KEduVocTranslation &translation ) const;

//             void fromKVTML2 ( QDomElement& parent );
//             void toKVTML2 ( QDomElement& parent );
public slots:
    /**
     * Add a false friend
     * @param falseFriend false friend of this index
     */
    void addFalseFriend(Translation* falseFriend) {
        m_translation->addFalseFriend(falseFriend->kEduVocTranslation());
    }

    /**
     * Remove a false friend
     * @param falseFriend false friend of this index
     */
    void removeFalseFriend(Translation* falseFriend) {
        m_translation->removeFalseFriend(falseFriend->kEduVocTranslation());
    }

    /**
     * Add a synonym
     * @param synonym An existing translation object that contains the synonym
     */
    void addSynonym(Translation* synonym) {
        m_translation->addSynonym(synonym->kEduVocTranslation());
    }

    /**
     * Remove a synonym
     * @param synonym An existing translation object that contains the synonym
     */
    void removeSynonym(Translation* synonym) {
        m_translation->removeSynonym(synonym->kEduVocTranslation());
    }

    /**
     * Add a antonym
     * @param antonym An existing translation object that contains the antonym
         */
    void addAntonym(Translation* antonym) {
        m_translation->addAntonym(antonym->kEduVocTranslation());
    }

    /**
     * Remove a antonym
     * @param antonym An existing translation object that contains the synonym
     */
    void removeAntonym(Translation* antonym) {
        m_translation->removeAntonym(antonym->kEduVocTranslation());
    }

    /**
     * Returns false friends of this expression
     * @return list of false friends (list of Translation objects)
     */
    QVariantList falseFriends() const {
        return toVariantList<KEduVocTranslation, Translation> (m_translation->falseFriends());
    }

    /**
     * Returns synonyms of this expression
     * @return synonyms (list of Translation objects)
     */
    QVariantList synonyms() const {
        return toVariantList<KEduVocTranslation, Translation> (m_translation->synonyms());
    }

    /**
     * Returns antonyms of this expression
     * @return antonyms (list of Translation objects)
     */
    QVariantList antonyms() const {
        return toVariantList<KEduVocTranslation, Translation> (m_translation->antonyms());
    }

    /** returns the word type of this expression */
    QString wordType() const;

    /** returns multiple choice if available */
    QStringList multipleChoice() {
        return m_translation->getMultipleChoice();
    }

    /** returns a string list with the available conjugation tenses */
    QStringList conjugationTenses() const {
        return m_translation->conjugationTenses();
    }

    /**
     * Sets the @p conjugation of the verb in the given @p tense, @p number and @p person
     * @code
     * #how to add a conjugation for a verb
     * import Parley
     * #set the tenses of this document (if not previously done)
     * Parley.doc.setTenses(["present","past simple"])
     * #iterate through all the word translations and find word "play"
     * for entry in Parley.doc.rootLesson.entries(True):
     *    for tr in entry.translations():
     *        if tr.text == "play":
     *            #make sure it's marked as a verb
     *            Parley.doc.setWordType(tr,"Verb")
     *            #add conjugations
     *            tr.setConjugationText("plays","present",Parley.Singular,Parley.Third)
     *            tr.setConjugationText("play","present",Parley.Plural,Parley.First)
     *            tr.setConjugationText("played","past simple",Parley.Singular,Parley.Second)
     * @endcode
     * @param conjugation The conjugation in @p tense, @p number, @p person
     * @param tense The tense that @p conjugation belongs to (see Document::tenses())
     * @param flags The KEduVocWordFlags flags
     */
    void setConjugationText(const QString& conjugation, const QString& tense, const KEduVocWordFlags& flags);

    /**
     * Sets the @p conjugation of the verb in the given @p tense, @p number and @p person. Use this method instead of setConjugationText when the grades, practice count etc are important and want them to be set in this conjugation
     * @param conjugation Scripting::Text object as the conjugation
     * @param tense The tense that @p conjugation belongs to (see Document::tenses())
     * @param flags The KEduVocWordFlags flags
     */
    void setConjugation(QObject * conjugation, const QString& tense, const KEduVocWordFlags& flags);


    /**
     * returns the conjugation of the verb in the given @p tense, @p number and @p person
     * @param tense The conjugation tense (see Document::tenses())
     * @param flags The KEduVocWordFlags flags
     * @return A Text object that represents the conjugation
     */
    QObject * conjugation(const QString & tense, const KEduVocWordFlags& flags);

    /**
     * Returns all the possible conjugations of the given @p tense
     * @param tense The tense of the conjugation
     * @return A String list with 15 entries. First 5 are for Singular [First, Second, ThirdMale, ThirdFemale, ThirdNeutral/Third], next 5 for Dual (in a similar way) and last 5 for plural.
     */
    QStringList conjugationTexts(const QString& tense);

    /**
     * Returns the conjugations of the given @p tense and @p number
     * @param tense The tense of conjugation
     * @param flags The KEduVocWordFlags flags
     * @return A String list with 5 conjugations in the order of [First, Second, ThirdMale, ThirdFemale, Third]
     */
    QStringList conjugationTexts(const QString& tense, const KEduVocWordFlags& flags);


    /*
     * Returns a pointer to the declension object of this translation.
     * Returns 0 if no declension object exists!
     * @return the declension
     */
//             KEduVocDeclension* declension();
//weird bug with QMap with this function
//             QObject * declension(KEduVocWordFlag::DeclensionNumber number, KEduVocWordFlag::DeclensionCase decCase);

    /*
     * Set a new declension for a translation
     * @param declension
     */
//             void setDeclension ( KEduVocDeclension* declension );
//             void setDeclension(const QString& declension, KEduVocWordFlag::DeclensionNumber number, KEduVocWordFlag::DeclensionCase decCase);



private:
    KEduVocTranslation * m_translation;

};

//Template functions should not be separated from their definition (must be in the header file)
template <class T, class S>
QVariantList Translation::toVariantList(QList<T*> objList) const
{
    QVariantList list;
    foreach(T * t, objList) {
        QObject * obj = new S(t);
        list.push_back(QVariant::fromValue(obj));
    }
    return list;
}


}

#endif
