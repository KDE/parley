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
        : QLineEdit(parent)
{
    connect(this, SIGNAL(returnPressed()), this, SLOT(slotEmitInput()));
}

void TextualInput::slotEmitCurrentInput()
{
    emit signalInput(text());
}

void TextualInput::slotChangeAnswerColor(bool correct)
{
    QPalette pal;
    if (correct)
        pal.setColor(QPalette::Text, Qt::green);
    else
        pal.setColor(QPalette::Text, Qt::red);
    
    setPalette(pal);
    setText(text());
}


void TextualInput::slotShowSolution(const QString& solution)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    setPalette(pal);
    setText(solution);
}

void TextualInput::slotClear()
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);
    setText("");
}
    

#include "input.moc"
