//
// C++ Implementation: scriptobjecttranslation
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjecttranslation.h"

namespace Scripting
{
    ScriptObjectTranslation::ScriptObjectTranslation (ScriptObjectEntry * entry)
            : QObject()
    {
         m_translation = new KEduVocTranslation(entry->kEduVocEntry());
    }

    ScriptObjectTranslation::ScriptObjectTranslation ( ScriptObjectEntry * entry, const QString & translation ) {
        m_translation = new KEduVocTranslation(entry->kEduVocEntry(),translation);
    }

    ScriptObjectTranslation::ScriptObjectTranslation ( KEduVocTranslation * translation )
            : QObject(), m_translation ( translation )
    {
    }

    ScriptObjectTranslation::~ScriptObjectTranslation()
    {
    }

}
