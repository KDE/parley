/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

void AudioButton::setSoundFile(const QUrl &soundFile)
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
