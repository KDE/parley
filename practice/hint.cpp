//
// C++ Implementation: hint
//
// Description: 
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hint.h"

Hint::Hint(QObject* parent)
    : QObject(parent)
{
}

void Hint::slotShowHint()
{
    emit signalAnswerTainted(Statistics::HintShown);
    emit signalShowHint();
}

void Hint::slotShowAnswer()
{
    emit signalShowAnswer();
}
