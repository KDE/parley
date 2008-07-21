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

    void Translation::setConjugation ( const QString& conjugation, const QString& tense, const QString & number, const QString & person )
    {
        static QMap<QString,KEduVocConjugation::ConjugationNumber> numberMap;
        if ( numberMap.isEmpty() )
        {
            //do this only the first time (cause numberMap is static and keeps its value)
            numberMap["singular"] = KEduVocConjugation::Singular;
            numberMap["dual"] = KEduVocConjugation::Dual;
            numberMap["plural"] = KEduVocConjugation::Plural;
        }

        static QMap<QString,KEduVocConjugation::ConjugationPerson> personMap;
        if ( personMap.isEmpty() )
        {
            //do this only the first time (cause personMap is static and keeps its value)
            personMap["first"] = KEduVocConjugation::First;
            personMap["second"] = KEduVocConjugation::Second;
            personMap["thirdmale"] = KEduVocConjugation::ThirdMale;
            personMap["thirdfemale"] = KEduVocConjugation::ThirdFemale;
            personMap["thirdneutralcommon"] = KEduVocConjugation::ThirdNeutralCommon;
            personMap["third"] = KEduVocConjugation::ThirdNeutralCommon;
        }

        if ( numberMap.contains ( number ) && personMap.contains ( person ) )
        {
            kDebug() << "Here";
            KEduVocConjugation & conjug = m_translation->conjugation ( tense );
            conjug.setConjugation ( conjugation,personMap[person],numberMap[number] );
            m_translation->setConjugation ( tense, conjug );
        }
    }

}
