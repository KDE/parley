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
#include "translation.h"
#include <keduvocconjugation.h>

namespace Scripting
{
    Translation::Translation ( Expression * entry )
    {
        m_translation = new KEduVocTranslation ( entry->kEduVocEntry() );
        m_text = m_translation;
    }

    Translation::Translation ( Expression * entry, const QString & translation )
    {
        m_translation = new KEduVocTranslation ( entry->kEduVocEntry(),translation );
        m_text = m_translation;
    }

    Translation::Translation ( KEduVocTranslation * translation )
            : Text ( translation ), m_translation ( translation )
    {
    }

    Translation::~Translation()
    {
    }

    QString Translation::wordType() const
    {
        if ( m_translation->wordType() )
            return m_translation->wordType()->name();
        return QString();
    }

    QMap<QString,KEduVocConjugation::ConjugationNumber> & Translation::getConjNumberMap ()
    {
        static QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap;
        if ( numberMap.isEmpty() )
        {
            //do this only the first time (cause numberMap is static and keeps its value)
            numberMap["singular"] = KEduVocConjugation::Singular;
            numberMap["dual"] = KEduVocConjugation::Dual;
            numberMap["plural"] = KEduVocConjugation::Plural;
        }
        return numberMap;
//         if ( numberMap.contains ( number.toLower() ) )
//             return numberMap[number.toLower() ];
    }

    QMap<QString,KEduVocConjugation::ConjugationPerson> & Translation::getConjPersonMap ()
    {
        static QMap<QString,KEduVocConjugation::ConjugationPerson> personMap;
        if ( personMap.isEmpty() )
        {
            //do this only the first time (cause personMap is static and keeps its value)
            personMap["first"] = KEduVocConjugation::First;
            personMap["second"] = KEduVocConjugation::Second;
            personMap["thirdmale"] = KEduVocConjugation::ThirdMale;
            personMap["thirdfemale"] = KEduVocConjugation::ThirdFemale;
            personMap["thirdneutral"] = KEduVocConjugation::ThirdNeutralCommon;
            personMap["third"] = KEduVocConjugation::ThirdNeutralCommon;
        }
        return personMap;
//         if (personMap.contains(person.toLower())) return personMap[person.toLower()];
    }

    void Translation::setConjugation ( const QString& conjugation, const QString& tense, const QString & number, const QString & person )
    {
        static QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap = getConjNumberMap();
        static QMap<QString,KEduVocConjugation::ConjugationPerson> personMap = getConjPersonMap();

        if ( numberMap.contains ( number.toLower() ) && personMap.contains ( person.toLower() ) )
        {
            //set conjugation
            KEduVocConjugation & conjug = m_translation->conjugation ( tense );
            conjug.setConjugation ( conjugation,personMap[person.toLower() ],numberMap[number.toLower() ] );
            m_translation->setConjugation ( tense, conjug );
        }
        else kDebug() << "Invalid number or person";
    }

    QString Translation::conjugation ( const QString & tense, const QString & number, const QString & person )
    {
        static QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap = getConjNumberMap();
        static QMap<QString,KEduVocConjugation::ConjugationPerson> personMap = getConjPersonMap();

        if ( numberMap.contains ( number.toLower() ) && personMap.contains ( person.toLower() ) )
        {
            //get conjugation text
            KEduVocConjugation conjug = m_translation->conjugation ( tense );
            return conjug.conjugation ( personMap[person.toLower() ],numberMap[number.toLower() ] ).text();
        }
        else kDebug() << "Invalid number or person";
    }

    QStringList Translation::conjugation ( const QString& tense )
    {
        KEduVocConjugation conjug = m_translation->conjugation ( tense );

        QStringList list;

        QSet<KEduVocConjugation::ConjugationNumber> numberList = getConjNumberMap().values().toSet();
        QSet<KEduVocConjugation::ConjugationPerson> personList = getConjPersonMap().values().toSet();

        foreach ( KEduVocConjugation::ConjugationNumber number, numberList )
        {
            foreach ( KEduVocConjugation::ConjugationPerson person, personList )
            {
                list << conjug.conjugation ( person,number ).text();
            }
        }

        return list;
    }

    QStringList Translation::conjugation ( const QString& tense, const QString& number )
    {
        KEduVocConjugation conjug = m_translation->conjugation ( tense );
        static QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap = getConjNumberMap();

        QStringList list;

        if ( numberMap.contains ( number.toLower() ) )
        {
            QSet<KEduVocConjugation::ConjugationPerson> personList = getConjPersonMap().values().toSet();
            foreach ( KEduVocConjugation::ConjugationPerson person, personList )
            list << conjug.conjugation ( person, numberMap[number.toLower() ] ).text();
            return list;
        }
        kDebug() << "Invalid conjugation number given";
        return list;
    }

}
