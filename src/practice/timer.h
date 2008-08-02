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
class Timer : public QObject
{
    Q_OBJECT

    public:
        Timer(QObject * parent = 0);
        explicit Timer(bool enable);
        ~Timer();

        /// A typedef for clarity in the Timer functions.
        typedef int Milliseconds;

        /// Sets the length of the timer
        void setLength(Milliseconds value) { m_length = value; }
        /// Returns the length of the timer
        Milliseconds length() const { return m_length; }

        void makeGUI(class KSvgRenderer* renderer, class ActiveArea * scene);

    public slots:
        /// Start a timer with a length of ms milliseconds and set m_length to ms.
        void slotStart(Milliseconds ms);
        /// Start a timer with a length of m_length
        void slotStart();
        /// Stops the timer prematurely (called when the user inputs an answer)
        void slotStop();
        /// Called each second; it checks the time remaining and updates the GUI.
        void slotTick();
    signals:
        /// Emitted when the timer times out.
        void signalTimeout(Prefs::EnumPracticeTimeoutMode::type);

    private:
        QTimer * m_timer;
        Milliseconds m_length;
        class TimerWidget * m_gtimer;
        Milliseconds m_remaining;
        bool m_enabled;
};

class TimerWidget
{
    public:
        virtual void update(Timer::Milliseconds remaining, Timer::Milliseconds original) = 0;
        virtual void stop() = 0;
        virtual void timeout() = 0;
        virtual ~TimerWidget() {}
};

class DummyTimerWidget : public TimerWidget
{
       void update(Timer::Milliseconds remaining, Timer::Milliseconds original) {};
       void stop() {};
       void timeout() {};
};

#endif
