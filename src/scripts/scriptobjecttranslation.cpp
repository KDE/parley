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
    Translation::Translation (Expression * entry)
            : QObject()
    {
         m_translation = new KEduVocTranslation(entry->kEduVocEntry());
    }

    Translation::Translation ( Expression * entry, const QString & translation ) {
        m_translation = new KEduVocTranslation(entry->kEduVocEntry(),translation);
    }

    Translation::Translation ( KEduVocTranslation * translation )
            : QObject(), m_translation ( translation )
    {
    }

    Translation::~Translation()
    {
    }

}
