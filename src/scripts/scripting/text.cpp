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
#include "text.h"

namespace Scripting
{

Text::Text(const QString& text)
    : QObject()
{
    m_text = new KEduVocText(text);
}

Text::Text(KEduVocText * text)
    : m_text(text)
{
}

Text::Text(const KEduVocText & text)
{
    m_text = new KEduVocText(text);
}


Text::~Text()
{
}


}

