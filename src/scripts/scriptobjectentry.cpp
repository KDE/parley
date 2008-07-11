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

    Entry::Entry ( KEduVocExpression * entry )
            : QObject(), m_entry ( entry )
    {
    }

    Entry::Entry ( const Entry & other ) : QObject()
    {
        m_entry = other.kEduVocEntry();
    }


    Entry::~Entry()
    {
    }

    Entry & Entry::operator= ( const Entry &other )
    {
        m_entry = other.kEduVocEntry();
        return ( *this );
    }

    bool Entry::operator== ( const Entry &other ) const {
        return m_entry == other.kEduVocEntry();
    }

    QString Scripting::Entry::getTranslation() const
    {
        /// @note KEduVocTranslation inherits from KEduVocText
        return m_entry->translation ( 0 )->text();
    }
}


