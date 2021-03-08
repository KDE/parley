/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "identifier.h"

#include <QMap>

#include <QDebug>

namespace Scripting
{

Identifier::Identifier()
    : QObject()
{
    m_identifier = new KEduVocIdentifier();
}

Identifier::Identifier(KEduVocIdentifier * identifier)
    : QObject(), m_identifier(identifier)
{
}

Identifier::Identifier(KEduVocIdentifier & identifier)
    : QObject()
{
    m_identifier = new KEduVocIdentifier(identifier);
}


Identifier::~Identifier()
{
}

QString Identifier::article(const KEduVocWordFlags& flags)
{
    return m_identifier->article().article(flags);
}


void Identifier::setArticle(const QString& article, const KEduVocWordFlags& flags)
{
    KEduVocArticle karticle = m_identifier->article();
    karticle.setArticle(article, flags);
    m_identifier->setArticle(karticle);
}

QString Identifier::personalPronoun(const KEduVocWordFlags& flags) const
{
    return m_identifier->personalPronouns().personalPronoun(flags);
}

void Identifier::setPersonalPronoun(const QString& conjugation, const KEduVocWordFlags& flags)
{
    KEduVocPersonalPronoun ppronouns = m_identifier->personalPronouns();
    ppronouns.setPersonalPronoun(conjugation, flags);
    m_identifier->setPersonalPronouns(ppronouns);
}

QStringList Identifier::personalPronouns()
{
//         KEduVocPersonalPronoun ppronouns = m_identifier->personalPronouns();

    QStringList list;

//         for ( int n = 0; n <= KEduVocConjugation::Plural; n++ )
//             for ( int p = 0; p <= KEduVocConjugation::ThirdNeutralCommon; p++ )
//             {
//                 list << ppronouns.personalPronoun ( ( KEduVocConjugation::ConjugationPerson ) p, ( KEduVocConjugation::ConjugationNumber ) n );
//             }

    return list;
}

}
