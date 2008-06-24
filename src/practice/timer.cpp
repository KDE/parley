//
// C++ Implementation: timer
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "timer.h"

#include <KDebug>

InvisibleTimer::InvisibleTimer(QObject * parent)
    : QObject(parent)
{
    kDebug() << "ctor'ed";
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

InvisibleTimer::~InvisibleTimer()
{
    delete m_timer;
}

void InvisibleTimer::slotStart(Milliseconds ms)
{
    kDebug() << "hit with " << ms;
    setLength(ms);
    slotStart();
}

void InvisibleTimer::slotStart()
{
    kDebug() << "...continues with " << m_length;
    m_timer->setSingleShot(true);
    m_timer->start(m_length);
}

void InvisibleTimer::slotStop()
{
    kDebug() << "stopped";
    m_timer->stop();
}

void InvisibleTimer::slotTimeout()
{
    kDebug() << "hit";
 //   emit signalTimeout((Prefs::EnumPracticeTimeoutMode)(Prefs::practiceTimeoutMode())); // TODO check on this
}
