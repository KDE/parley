///\file stdbuttons.h
// C++ Interface: stdbuttons
//
// Description: The class the manages the standard buttons of a practice dialog (continue, skip, quit, etc).
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef EDU_STD_BUTTONS_H
#define EDU_STD_BUTTONS_H

#include <QDialogButtonBox>
#include <QWidget>

#include <KDialogButtonBox>
#include <QAbstractButton>

#include "statistics.h"

/// @class StdButtons
/// @author David Capel <wot.narg@gmail.com>
/// @brief The standard set of control button on a practice dialog (contiue, skip, quit, etc)

class StdButtons : public KDialogButtonBox
{
        Q_OBJECT

    public:
        StdButtons(QWidget * parent = 0);

    public slots:
        void slotButtonClicked(QAbstractButton * button);
        void slotReturnPressed();
        void slotAnswerShown();
//        virtual void slotQuit();

    signals:
        /// Emitted when a question is skipped.
        void signalSkipped(Statistics::SkipReason);
        /// Emitted when the user wants to continue.
        /// The user should only be able to use the continue function
        /// after the question was been answered and corrected.
        void signalContinue();
        void signalCheckAnswer();
        void signalQuit();
        // void clicked( QAbstractButton * button );
};



#endif
