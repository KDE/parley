//
// C++ Implementation: scriptobjectentry
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectentry.h"
#include "scriptobjecttranslation.h"

namespace Scripting
{

    Expression::Expression()
    {
        m_expression = new KEduVocExpression();
    }

    Expression::Expression ( const QString & expression )
    {
        m_expression = new KEduVocExpression ( expression );
    }

    Expression::Expression ( const QStringList & translations )
    {
        m_expression = new KEduVocExpression ( translations );
    }

    Expression::Expression ( KEduVocExpression * expression )
            : QObject(), m_expression ( expression )
    {
    }

    Expression::Expression ( const Expression & other ) : QObject()
    {
        m_expression = other.kEduVocEntry();
    }

    Expression::~Expression()
    {
    }

    QVariantList Expression::translationIndices() const
    {
        QVariantList vlist;
        foreach (int k, m_expression->translationIndices()) {
            vlist.push_back(QVariant(k));
        }
        return vlist;
    }

    Expression & Expression::operator= ( const Expression &other )
    {
        m_expression = other.kEduVocEntry();
        return ( *this );
    }

    bool Expression::operator== ( const Expression &other ) const
    {
        return m_expression == other.kEduVocEntry();
    }

    QString Scripting::Expression::getTranslation() const
    {
        /// @note KEduVocTranslation inherits from KEduVocText
        return m_expression->translation ( 0 )->text();
    }

//     void Expression::setTranslation( int index, QObject* translation ) {
//         Translation * t = dynamic_cast<Translation*>(translation);
//         m_expression->setTranslation(index,t->kEduVocTranslation());
//     }

    Translation* Expression::translation ( int index )
    {
        return new Translation ( m_expression->translation ( index ) );
    }

    Translation* Expression::translation ( int index ) const
    {
        return new Translation ( m_expression->translation ( index ) );
    }
}


