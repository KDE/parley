/***************************************************************************
    Copyright 2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "audiobutton.h"

#include <kicon.h>
#include <klocale.h>

using namespace Practice;

AudioButton::AudioButton(QWidget *parent)
    : QToolButton(parent), m_player(0)
{
    setEnabled(false);
    setIcon(KIcon("media-playback-start"));
    setText(i18n("Play"));
    setToolTip(i18n("Play"));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(playAudio()));
}

void AudioButton::setSoundFile(KUrl soundFile)
{
    m_url = soundFile;
    setEnabled(!m_url.path().isEmpty() && m_url.isLocalFile());
}

void AudioButton::playAudio()
{
    if (!m_player) {
        m_player = Phonon::createPlayer(Phonon::NotificationCategory, m_url);
        m_player->setParent(this);
        connect(m_player, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(playerStateChanged(Phonon::State)));
    } else {
        if (m_player->state() == Phonon::PlayingState) {
            m_player->stop();
            return;
        }
        m_player->setCurrentSource(m_url);
    }
    m_player->play();
}

void AudioButton::playerStateChanged(Phonon::State newState)
{
    switch (newState) {
    case Phonon::LoadingState: case Phonon::BufferingState: case Phonon::PlayingState:
        setIcon(KIcon("media-playback-stop"));
        setText(i18n("Stop"));
        setToolTip(i18n("Stop"));
        break;
    case Phonon::StoppedState: case Phonon::PausedState: case Phonon::ErrorState:
        setIcon(KIcon("media-playback-start"));
        setToolTip(i18n("Play"));
        setText(i18n("Play"));
        break;
    }
}
