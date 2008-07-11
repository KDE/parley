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

    ScriptObjectEntry::ScriptObjectEntry ( ScriptObjectEntry & other ) : QObject()
    {
        m_entry = other.kEduVocEntry();
    }


    ScriptObjectEntry::~ScriptObjectEntry()
    {
    }

    QString Scripting::ScriptObjectEntry::getTranslation()
    {
        /// @note KEduVocTranslation inherits from KEduVocText
        return m_entry->translation ( 0 )->text();
    }
}


