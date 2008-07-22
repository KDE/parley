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
#include "identifier.h"

#include "translation.h"

#include <QMap>

#include <KDebug>

namespace Scripting
{

    Identifier::Identifier()
            : QObject()
    {
        m_identifier = new KEduVocIdentifier();
    }

    Identifier::Identifier ( KEduVocIdentifier * identifier )
            : QObject(), m_identifier ( identifier )
    {
    }

    Identifier::Identifier ( KEduVocIdentifier & identifier )
            : QObject()
    {
        m_identifier = new KEduVocIdentifier ( identifier );
    }


    Identifier::~Identifier()
    {
    }

    QMap<QString,KEduVocArticle::ArticleNumber> & getArticleNumberMap ()
    {
        static QMap<QString,KEduVocArticle::ArticleNumber> numberMap;
        if ( numberMap.isEmpty() )
        {
            //do this only the first time (cause numberMap is static and keeps its value)
            numberMap["singular"] = KEduVocArticle::Singular;
            numberMap["dual"] = KEduVocArticle::Dual;
            numberMap["plural"] = KEduVocArticle::Plural;
        }
        return numberMap;
    }

    QMap<QString,KEduVocArticle::ArticleGender> & getArticleGenderMap ()
    {
        static QMap<QString,KEduVocArticle::ArticleGender> genderMap;
        if ( genderMap.isEmpty() )
        {
            //do this only the first time (cause genderMap is static and keeps its value)
            genderMap["masculine"] = KEduVocArticle::Masculine;
            genderMap["feminine"] = KEduVocArticle::Feminine;
            genderMap["neutral"] = KEduVocArticle::Neutral;
        }
        return genderMap;
    }

    QMap<QString,KEduVocArticle::ArticleDefiniteness> & getArticleDefinitenessMap ()
    {
        static QMap<QString,KEduVocArticle::ArticleDefiniteness> definitenessMap;
        if ( definitenessMap.isEmpty() )
        {
            //do this only the first time (cause genderMap is static and keeps its value)
            definitenessMap["definite"] = KEduVocArticle::Definite;
            definitenessMap["indefinite"] = KEduVocArticle::Indefinite;
        }
        return definitenessMap;
    }

    QString Identifier::article ( const QString& number,const QString& definite,const QString& gender )
    {
        QMap<QString,KEduVocArticle::ArticleNumber> numberMap = getArticleNumberMap();
        QMap<QString,KEduVocArticle::ArticleGender> genderMap = getArticleGenderMap();
        QMap<QString,KEduVocArticle::ArticleDefiniteness> definitenessMap = getArticleDefinitenessMap();

        if ( numberMap.contains ( number.toLower() ) && definitenessMap.contains ( definite.toLower() ), genderMap.contains ( gender.toLower() ) )
            return m_identifier->article().article ( numberMap[number.toLower() ],definitenessMap[definite.toLower() ],genderMap[gender.toLower() ] );
        else
            kDebug() << "Invalid number, definite or gender";
    }

    void Identifier::setArticle ( const QString& article, const QString& number,const QString& definite,const QString& gender )
    {
        QMap<QString,KEduVocArticle::ArticleNumber> numberMap = getArticleNumberMap();
        QMap<QString,KEduVocArticle::ArticleGender> genderMap = getArticleGenderMap();
        QMap<QString,KEduVocArticle::ArticleDefiniteness> definitenessMap = getArticleDefinitenessMap();

        if ( numberMap.contains ( number.toLower() ) && definitenessMap.contains ( definite.toLower() ), genderMap.contains ( gender.toLower() ) )
        {
            KEduVocArticle karticle = m_identifier->article();
            karticle.setArticle ( article, numberMap[number.toLower() ],definitenessMap[definite.toLower() ],genderMap[gender.toLower() ] );
            m_identifier->setArticle ( karticle );
        }
        else
            kDebug() << "Invalid number, definite or gender";
    }

    QString Identifier::personalPronoun ( const QString & number, const QString & person )
    {
        QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap = Translation::getConjNumberMap();
        QMap<QString,KEduVocConjugation::ConjugationPerson> personMap = Translation::getConjPersonMap();


        if ( numberMap.contains ( number.toLower() ) && personMap.contains ( person.toLower() ) )
        {
            return m_identifier->personalPronouns().personalPronoun ( personMap[person.toLower() ],numberMap[number.toLower() ] );
        }
        else kDebug() << "Invalid number or person";
    }

    void Identifier::setPersonalPronoun ( const QString& personalPronoun, const QString & number, const QString & person )
    {
        QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap = Translation::getConjNumberMap();
        QMap<QString,KEduVocConjugation::ConjugationPerson> personMap = Translation::getConjPersonMap();


        if ( numberMap.contains ( number.toLower() ) && personMap.contains ( person.toLower() ) )
        {
            KEduVocPersonalPronoun ppronouns = m_identifier->personalPronouns();
            ppronouns.setPersonalPronoun ( personalPronoun,personMap[person.toLower() ],numberMap[number.toLower() ] );
            m_identifier->setPersonalPronouns(ppronouns);
        }
        else kDebug() << "Invalid number or person";
    }

}
