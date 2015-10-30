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

#include <KLocalizedString>

using namespace Practice;

AudioButton::AudioButton(QWidget *parent)
    : QToolButton(parent), m_player(0)
{
    setEnabled(false);
    QIcon setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    setText(i18n("Play"));
    setToolTip(i18n("Play"));
    connect(this, &AudioButton::clicked, this, &AudioButton::playAudio);
    connect(parent, SIGNAL(stopAudio()), this, SLOT(stopAudio()));
}

void AudioButton::setSoundFile(QUrl soundFile)
{
    m_url = soundFile;
    setEnabled(!m_url.isEmpty() && m_url.isLocalFile());
}

void AudioButton::playAudio()
{
    if (!m_player) {
        m_player = new QMediaPlayer( this );
        connect(m_player, &QMediaPlayer::stateChanged, this, &AudioButton::playerStateChanged);
    } else {
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->stop();
        }
    }
    m_player->setMedia(m_url);
    m_player->setVolume(50);
    m_player->play();
}

void AudioButton::stopAudio()
{
    if (m_player && m_player->state() == QMediaPlayer::PlayingState) {
        m_player->stop();
    }
}

void AudioButton::playerStateChanged(QMediaPlayer::State newState)
{
    switch (newState) {
    case QMediaPlayer::PlayingState:
        setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
        setText(i18n("Stop"));
        setToolTip(i18n("Stop"));
        break;
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        setToolTip(i18n("Play"));
        setText(i18n("Play"));
        break;
    }
}
