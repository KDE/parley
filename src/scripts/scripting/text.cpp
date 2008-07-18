//
// C++ Implementation: text
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "text.h"

namespace Scripting
{

    Text::Text ( const QString& text )
            : QObject()
    {
        m_text = new KEduVocText ( text );
    }

    Text::Text ( KEduVocText * text )
            : m_text ( text )
    {
    }

    Text::~Text()
    {
    }


}

