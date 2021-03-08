/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "audiowidget.h"

#include <KEduVocExpression>

#include <QMediaPlayer>
#include <QDebug>

using namespace Editor;

AudioWidget::AudioWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;
    m_player = 0;

    connect(audioUrlRequester, &KUrlRequester::textChanged, this, &AudioWidget::slotAudioFileChanged);
    connect(playButton, &QPushButton::clicked, this, &AudioWidget::playAudio);
//     connect(recordButton, SIGNAL(clicked()), SLOT(recordAudio()));

    playButton->setEnabled(false);
    playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    recordButton->setVisible(false);
//     recordButton->setEnabled(false);
//     recordButton->setIcon(QIcon::fromTheme("media-record"));
    audioUrlRequester->setEnabled(false);
}

void AudioWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (m_entry) {
        recordButton->setEnabled(true);
        audioUrlRequester->setEnabled(true);
        audioUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->soundUrl());
    } else {
        recordButton->setEnabled(false);
        audioUrlRequester->setEnabled(false);
        if (m_player) {
            if (m_player->state() == QMediaPlayer::PlayingState) {
                playButton->setEnabled(true);
            } else {
                playButton->setEnabled(false);
            }
        }
        audioUrlRequester->clear();
    }
}

void AudioWidget::slotAudioFileChanged(const QString & url)
{
    if (m_entry) {
        m_entry->translation(m_currentTranslation)->setSoundUrl( QUrl::fromLocalFile(url));
    }
    playButton->setEnabled(!url.isEmpty());
}

void AudioWidget::playAudio()
{
    QUrl soundFile = m_entry->translation(m_currentTranslation)->soundUrl();

    if (!m_player) {
        m_player = new QMediaPlayer( this );
        connect(m_player, SIGNAL(finished()), SLOT(slotPlaybackFinished()));
    } else {
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->stop();
            slotPlaybackFinished();
        }
    }
    m_player->setMedia(soundFile);
    m_player->setVolume(50);

    playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    m_player->play();
}

/*
void AudioWidget::recordAudio()
{
    /// FIXME: When Phonon gains the ability to record sound, implement me :)
}
*/

void AudioWidget::slotPlaybackFinished()
{
    playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    playButton->setEnabled(!audioUrlRequester->url().isEmpty());
}
