/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "audiobutton.h"

#include <KLocalizedString>
#include <QAudioOutput>

using namespace Practice;

AudioButton::AudioButton(QWidget *parent)
    : QToolButton(parent)
    , m_player(nullptr)
{
    setEnabled(false);
    setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
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
        m_player = new QMediaPlayer(this);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        connect(m_player, &QMediaPlayer::stateChanged, this, &AudioButton::playerStateChanged);
    } else {
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->stop();
        }
    }
    m_player->setMedia(m_url);
    m_player->setVolume(50);
#else
        connect(m_player, &QMediaPlayer::playbackStateChanged, this, &AudioButton::playerStateChanged);
        m_player->setAudioOutput(new QAudioOutput);
    } else {
        if (m_player->playbackState() == QMediaPlayer::PlayingState) {
            m_player->stop();
        }
    }
    m_player->setSource(m_url);
    m_player->audioOutput()->setVolume(50);
#endif
    m_player->play();
}

void AudioButton::stopAudio()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (m_player && m_player->state() == QMediaPlayer::PlayingState) {
#else
    if (m_player && m_player->playbackState() == QMediaPlayer::PlayingState) {
#endif
        m_player->stop();
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void AudioButton::playerStateChanged(QMediaPlayer::State newState)
#else
void AudioButton::playerStateChanged(QMediaPlayer::PlaybackState newState)
#endif
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

#include "moc_audiobutton.cpp"
