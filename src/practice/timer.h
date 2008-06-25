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

#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include "prefs.h"

/**
* @class Timer
* @author David Capel <wot.narg@gmail.com>
* @brief This class keeps track of the timer for each question.
* This may have a visible widget or be invisible, depending on the practide mode.
* Some modes (and settings configurations) may not use timers.
*/
class InvisibleTimer : public QObject
{
    Q_OBJECT

    public:
        InvisibleTimer(QObject * parent = 0);
        ~InvisibleTimer();

        /// A typedef for clarity in the Timer functions.
        typedef int Milliseconds;

        /// Sets the length of the timer
        void setLength(Milliseconds value) { m_length = value; }
        /// Returns the length of the timer
        Milliseconds length() const { return m_length; }


    public slots:
        /// Start a timer with a length of ms milliseconds and set m_length to ms.
        void slotStart(Milliseconds ms);
        /// Start a timer with a length of m_length
        void slotStart();
        /// Stops the timer prematurely (called when the user inputs an answer)
        void slotStop();
        void slotTimeout();
    signals:
        /// Emitted when the timer times out.
        void signalTimeout(Prefs::EnumPracticeTimeoutMode::type);

    private:
        QTimer * m_timer;
        Milliseconds m_length;
};

#endif
