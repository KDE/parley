//
// C++ Interface: multiplechoicemcinput
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef MULTIPLECHOICEMCIINPUT_H
#define MULTIPLECHOICEMCIINPUT_H

#include "mcinput.h"

class PracticeEntry;

class MultipleChoiceMCInput : public MCInput
{
    Q_OBJECT
    public:
        MultipleChoiceMCInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);

    public slots:
        void slotSetAnswers(PracticeEntry*, QList<PracticeEntry*>);
};


#endif
