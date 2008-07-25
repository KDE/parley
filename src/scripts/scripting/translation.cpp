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

    void Translation::setConjugationText ( const QString& conjugation, const QString& tense, KEduVocConjugation::ConjugationNumber number, KEduVocConjugation::ConjugationPerson person )
    {
        KEduVocConjugation & conjug = m_translation->conjugation ( tense );
        conjug.setConjugation ( conjugation, person, number );
        m_translation->setConjugation ( tense, conjug );
    }

    QObject * Translation::conjugation ( const QString & tense, KEduVocConjugation::ConjugationNumber number, KEduVocConjugation::ConjugationPerson person )
    {
        KEduVocConjugation conjug = m_translation->conjugation ( tense );
        return new Text ( conjug.conjugation ( person , number ) );
    }

    void Translation::setConjugation ( QObject * conjugation, const QString& tense, KEduVocConjugation::ConjugationNumber number, KEduVocConjugation::ConjugationPerson person )
    {
        Text * txt = dynamic_cast<Text*>(conjugation);
        if (txt) {
            KEduVocConjugation & conjug = m_translation->conjugation ( tense );
            conjug.setConjugation ( *(txt->kEduVocText()), person, number );
            m_translation->setConjugation ( tense, conjug );
        }
    }


    QStringList Translation::conjugationTexts ( const QString& tense )
    {
        KEduVocConjugation conjug = m_translation->conjugation ( tense );

        QStringList list;

        for ( int n = 0; n <= KEduVocConjugation::Plural; n++ )
            for ( int p = 0; p <= KEduVocConjugation::ThirdNeutralCommon; p++ )
                list << conjug.conjugation ( ( KEduVocConjugation::ConjugationPerson ) p, ( KEduVocConjugation::ConjugationNumber ) n ).text();

        return list;
    }

    QStringList Translation::conjugationTexts ( const QString& tense, KEduVocConjugation::ConjugationNumber number )
    {
        KEduVocConjugation conjug = m_translation->conjugation ( tense );

        QStringList list;

        for ( int p = 0; p <= KEduVocConjugation::ThirdNeutralCommon; p++ )
            list << conjug.conjugation ( ( KEduVocConjugation::ConjugationPerson ) p, number ).text();

        return list;
    }


//     QObject * Translation::declension ( KEduVocDeclension::DeclensionNumber number, KEduVocDeclension::DeclensionCase decCase )
//     {
//         kDebug() << number << decCase;
//         kDebug() << m_translation->declension();
//         if ( m_translation->declension() ) {
//             kDebug() << "in here";
//             //crashes here!
//             m_translation->declension()->declension ( number,decCase );
//             kDebug() << "in here";
//             return new Text ( m_translation->declension()->declension ( number,decCase ) );
//         }
//         else
//             return new Text ( QString() );
//     }

//     void Translation::setDeclension ( const QString& declension, KEduVocDeclension::DeclensionNumber number, KEduVocDeclension::DeclensionCase decCase )
//     {
//         kDebug() << number << decCase;
//         KEduVocDeclension * dec = m_translation->declension();
//         kDebug() << dec;
//         if ( !dec )
//             dec = new KEduVocDeclension();
//         kDebug() << "here";
//         dec->setDeclension ( KEduVocText ( declension ),number,decCase );
//         m_translation->setDeclension ( dec );
//     }

}
