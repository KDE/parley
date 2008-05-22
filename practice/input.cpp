//
// C++ Implementation: input
//
// Description: Implementation of the input-gathering class
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "input.h"
#include "statistics.h"

#include <KDebug>

TextualInput::TextualInput(QWidget* parent)
        : QLineEdit(parent), Input()
{}

Input::~Input() {}

void TextualInput::slotShowSolutionFinished()
{
    setText(m_answer);
}

void TextualInput::slotCheckAnswer()
{
    kDebug() << "checking answer";

    if (text() == m_answer)
    {
        kDebug() << "correct";
        emit signalCorrect();
    }
    else
    {
        kDebug() << "incorrect";
        emit signalIncorrect(Statistics::UnknownMistake); // TODO: do this logic
    }
}

void TextualInput::slotSetAnswer(const QString& answer)
{
    m_answer = answer;
    setText("");
    //slotShowSolutionFinished();
}

#include "input.moc"
