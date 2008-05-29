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

 /***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


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