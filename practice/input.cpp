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

Input::Input() {}

Input::~Input()
{
    delete m_answer;
}

void TextualInput::slotShowSolutionFinished()
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);
    setText(m_answer->text);
}

void TextualInput::slotCheckAnswer()
{
    if (text() == m_answer->text)
    {
        kDebug() << "correct";
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::green);
        setPalette(pal);
        emit signalCorrect();
    }
    else
    {
        kDebug() << "incorrect";
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::red);
        setPalette(pal);
        emit signalIncorrect(Statistics::UnknownMistake); // TODO: do this logic
    }
}

void TextualInput::slotSetAnswer(EduAnswer* answer)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);
    m_answer = answer;
    setText("");
}

#include "input.moc"
