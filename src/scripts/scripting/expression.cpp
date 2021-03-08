/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "expression.h"
#include "translation.h"

namespace Scripting
{

Expression::Expression()
{
    m_expression = new KEduVocExpression();
}

Expression::Expression(const QString & expression)
{
    m_expression = new KEduVocExpression(expression);
}

Expression::Expression(const QStringList & translations)
{
    m_expression = new KEduVocExpression(translations);
}

Expression::Expression(KEduVocExpression * expression)
    : QObject(), m_expression(expression)
{
}

Expression::Expression(const Expression & other) : QObject()
{
    m_expression = other.kEduVocExpression();
}

Expression::~Expression()
{
}

QVariantList Expression::translationIndices() const
{
    QVariantList vlist;
    foreach(int k, m_expression->translationIndices()) {
        vlist.push_back(QVariant(k));
    }
    return vlist;
}

QVariantList Expression::translations() const
{
    //build a list of all the translations
    QList<KEduVocTranslation *> translations;
    foreach(int k, m_expression->translationIndices()) {
        if (m_expression->translation(k))
            translations.push_back(m_expression->translation(k));
    }
    //convert it to QVariantList and return it
    return toVariantList<KEduVocTranslation, Translation> (translations);
}

QStringList Expression::translationTexts() const
{
    //build a list of all the translation texts
    QStringList list;
    foreach(int k, m_expression->translationIndices()) {
        if (m_expression->translation(k))
            list << m_expression->translation(k)->text();
    }
    return list;
}

//     Expression & Expression::operator= ( const Expression &other )
//     {
//         m_expression = other.kEduVocEntry();
//         return ( *this );
//     }

//     bool Expression::operator== ( const Expression &other ) const
//     {
//         return m_expression == other.kEduVocEntry();
//     }

//     void Expression::setTranslation( int index, QObject* translation ) {
//         Translation * t = dynamic_cast<Translation*>(translation);
//         m_expression->setTranslation(index,t->kEduVocTranslation());
//     }

void Expression::setTranslations(const QStringList &translations)
{
    delete m_expression;
    m_expression = new KEduVocExpression(translations);
}

QObject* Expression::translation(int index) const
{
    return new Translation(m_expression->translation(index));
}
}


