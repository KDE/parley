//
// C++ Implementation: timer
//
// Description:
//
//
// Copyright 2008 David Capel <wot.narg@gmail.com>
//
//

#include "timer.h"
#include "activearea.h"

#include <KSvgRenderer>
#include <QGraphicsScene>
#include <KDebug>

Timer::Timer(QObject * parent)
    : QObject(parent)
{
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTick()));
    m_enabled = true;
}

Timer::Timer(bool enable)
: m_enabled(enable)
{}

Timer::~Timer()
{
    delete m_timer;
    delete m_gtimer;
}

void Timer::slotStart(Milliseconds ms)
{
    setLength(ms);
    slotStart();
}

void Timer::slotStart()
{
    m_timer->start(1000);
    m_remaining = m_length;
    m_gtimer->update(m_remaining, m_length);
}

void Timer::slotStop()
{
    if (!m_enabled) return;
    kDebug() << "stopped";
    m_timer->stop();
    m_gtimer->stop();
}

void Timer::slotTick()
{
    m_remaining -= 1000;
    if (m_remaining)
        m_gtimer->update(m_remaining, m_length);
    else
    {
        m_gtimer->timeout();
        emit signalTimeout(Prefs::practiceTimeoutMode());
    }
}


void Timer::makeGUI(KSvgRenderer * renderer, ActiveArea* area)
{
    // TODO do this
    m_gtimer = new DummyTimerWidget();
}
