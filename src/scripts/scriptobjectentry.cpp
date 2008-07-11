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

namespace Scripting
{

    Expression::Expression() {
        m_expression = new KEduVocExpression();
    }

    Expression::Expression ( const QString & expression )
    {
        m_expression = new KEduVocExpression(expression);
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
}


