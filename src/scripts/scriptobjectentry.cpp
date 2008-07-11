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

    ScriptObjectEntry::ScriptObjectEntry ( KEduVocExpression * entry )
            : QObject(), m_entry ( entry )
    {
    }

    ScriptObjectEntry::ScriptObjectEntry ( const ScriptObjectEntry & other ) : QObject()
    {
        m_entry = other.kEduVocEntry();
    }


    ScriptObjectEntry::~ScriptObjectEntry()
    {
    }

    ScriptObjectEntry & ScriptObjectEntry::operator= ( const ScriptObjectEntry &other )
    {
        m_entry = other.kEduVocEntry();
        return ( *this );
    }

    bool ScriptObjectEntry::operator== ( const ScriptObjectEntry &other ) const {
        return m_entry == other.kEduVocEntry();
    }

    QString Scripting::ScriptObjectEntry::getTranslation() const
    {
        /// @note KEduVocTranslation inherits from KEduVocText
        return m_entry->translation ( 0 )->text();
    }
}


