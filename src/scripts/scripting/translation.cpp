/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "translation.h"

namespace Scripting
{
    Translation::Translation (Expression * entry)
    {
        m_translation = new KEduVocTranslation(entry->kEduVocEntry());
        m_text = m_translation;
    }

    Translation::Translation ( Expression * entry, const QString & translation ) {
        m_translation = new KEduVocTranslation(entry->kEduVocEntry(),translation);
        m_text = m_translation;
    }

    Translation::Translation ( KEduVocTranslation * translation )
            : Text (translation), m_translation ( translation )
    {
    }

    Translation::~Translation()
    {
    }

}
