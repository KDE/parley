/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "audiowidget.h"

#include <Phonon/MediaObject>

#include <keduvocexpression.h>

#include <KIcon>
#include <kdebug.h>


AudioWidget::AudioWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;
    m_player = 0;

    connect(audioUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotAudioFileChanged(const QString&)));
    connect(playButton, SIGNAL(clicked()), SLOT(playAudio()));
    connect(recordButton, SIGNAL(clicked()), SLOT(recordAudio()));

    playButton->setIcon(KIcon("media-playback-start"));
    recordButton->setEnabled(false);
    recordButton->setIcon(KIcon("media-record"));
}


void AudioWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (m_entry) {
        recordButton->setEnabled(true);
        audioUrlRequester->setEnabled(true);
        audioUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->soundUrl().toLocalFile());
    } else {
        recordButton->setEnabled(false);
        audioUrlRequester->setEnabled(false);
        if (m_player) {
            if (m_player->state() == Phonon::PlayingState) {
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
    kDebug() << "Setting sound " << url;
    m_entry->translation(m_currentTranslation)->setSoundUrl( KUrl(url) );
    playButton->setEnabled(!url.isEmpty());
}


void AudioWidget::playAudio()
{
    KUrl soundFile = m_entry->translation(m_currentTranslation)->soundUrl();
    kDebug() << "sound file: " << soundFile.url();

    if (!m_player)
    {
        m_player = Phonon::createPlayer(Phonon::NotificationCategory, soundFile);
        m_player->setParent(this);
        connect(m_player, SIGNAL(finished()), SLOT(slotPlaybackFinished()));
    } else {
        if (m_player->state() == Phonon::PlayingState) {
            m_player->stop();
            slotPlaybackFinished();
            return;
        }
        m_player->setCurrentSource(soundFile);
    }
    playButton->setIcon(KIcon("media-playback-stop"));
    m_player->play();
}

void AudioWidget::recordAudio()
{
}

void AudioWidget::slotPlaybackFinished()
{
    playButton->setIcon(KIcon("media-playback-start"));
    playButton->setEnabled(!audioUrlRequester->url().isEmpty());
}

#include "audiowidget.moc"
