/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "translation.h"

namespace Scripting
{
Translation::Translation(Expression * entry)
{
    m_translation = new KEduVocTranslation(entry->kEduVocExpression());
    m_text = m_translation;
}

Translation::Translation(Expression * entry, const QString & translation)
{
    m_translation = new KEduVocTranslation(entry->kEduVocExpression(), translation);
    m_text = m_translation;
}

Translation::Translation(KEduVocTranslation * translation)
    : Text(translation), m_translation(translation)
{
}

Translation::~Translation()
{
}

QString Translation::wordType() const
{
    if (m_translation->wordType())
        return m_translation->wordType()->name();
    return QString();
}

void Translation::setConjugationText(const QString& conjugation, const QString& tense, const KEduVocWordFlags& flags)
{
    KEduVocConjugation conjug = m_translation->getConjugation(tense);
    conjug.setConjugation(conjugation, flags);
    m_translation->setConjugation(tense, conjug);
}

QObject * Translation::conjugation(const QString & tense, const KEduVocWordFlags& flags)
{
    KEduVocConjugation conjug = m_translation->getConjugation(tense);
    return new Text(conjug.conjugation(flags));
}

void Translation::setConjugation(QObject * conjugation, const QString& tense, const KEduVocWordFlags& flags)
{
    Text * txt = dynamic_cast<Text*>(conjugation);
    if (txt) {
        KEduVocConjugation conjug = m_translation->getConjugation(tense);
        conjug.setConjugation(*(txt->kEduVocText()), flags);
        m_translation->setConjugation(tense, conjug);
    }
}


QStringList Translation::conjugationTexts(const QString& tense)
{
    Q_UNUSED(tense)
//         KEduVocConjugation conjug = m_translation->conjugation ( tense );

    QStringList list;

//         for ( int n = 0; n <= KEduVocConjugation::Plural; n++ )
//             for ( int p = 0; p <= KEduVocConjugation::ThirdNeutralCommon; p++ )
//                 list << conjug.conjugation ( ( KEduVocConjugation::ConjugationPerson ) p, ( KEduVocConjugation::ConjugationNumber ) n ).text();

    return list;
}

QStringList Translation::conjugationTexts(const QString& tense, const KEduVocWordFlags& flags)
{
    Q_UNUSED(tense)
    Q_UNUSED(flags)
//         KEduVocConjugation conjug = m_translation->conjugation ( tense );

    QStringList list;

//         for ( int p = 0; p <= KEduVocConjugation::ThirdNeutralCommon; p++ )
//             list << conjug.conjugation ( ( KEduVocConjugation::ConjugationPerson ) p, number ).text();

    return list;
}


//     QObject * Translation::declension ( KEduVocWordFlag::DeclensionNumber number, KEduVocWordFlag::DeclensionCase decCase )
//     {
//         qDebug() << number << decCase;
//         qDebug() << m_translation->declension();
//         if ( m_translation->declension() ) {
//             qDebug() << "in here";
//             //crashes here!
//             m_translation->declension()->declension ( number,decCase );
//             qDebug() << "in here";
//             return new Text ( m_translation->declension()->declension ( number,decCase ) );
//         }
//         else
//             return new Text ( QString() );
//     }

//     void Translation::setDeclension ( const QString& declension, KEduVocWordFlag::DeclensionNumber number, KEduVocWordFlag::DeclensionCase decCase )
//     {
//         qDebug() << number << decCase;
//         KEduVocDeclension * dec = m_translation->declension();
//         qDebug() << dec;
//         if ( !dec )
//             dec = new KEduVocDeclension();
//         qDebug() << "here";
//         dec->setDeclension ( KEduVocText ( declension ),number,decCase );
//         m_translation->setDeclension ( dec );
//     }

}
