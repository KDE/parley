/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
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

