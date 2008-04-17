//
// C++ Interface: timer
//
// Description: Keeps track of the timer for practices that are timed.
//
///\file timer.h
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

#ifndef EDU_TIMER_H
#define EDU_TIMER_H

/**
* @class Timer
* @author David Capel <wot.narg@gmail.com>
* @brief This class keeps track of the timer for each question.
* This may have a visible widget or be invisible, depending on the practide mode.
* Some modes (and settings configurations) may not use timers.
*/
class Timer : public EduWidget
{

    Q_OBJECT

    public:
        /// A typedef for clarity in the Timer functions.
        typedef int Milliseconds;

    public slots:
        /// Started a timer with a length of ms milliseconds.
        void slotStartAnswerTimer(Milliseconds ms);
        /// Stops the timer prematurely (called when the user inputs an answer)
        void slotStopAnswerTimer();

    signals:
        /// Emitted when the timer times out.
        void signalTimeoutReached();

    private:
        QTimer * timer;
};

#endif
