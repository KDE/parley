/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "audiowidget.h"
#include <KEduVocDocument>
#include <KEduVocExpression>
#include <QDebug>
#include <QMediaPlayer>

using namespace Editor;

AudioWidget::AudioWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(audioUrlRequester, &KUrlRequester::textChanged, this, &AudioWidget::slotAudioFileChanged);
    connect(playButton, &QPushButton::clicked, this, &AudioWidget::startPlayback);
    connect(recordButton, &QPushButton::clicked, this, &AudioWidget::startRecordAudio);
    connect(downloadButton, &QPushButton::clicked, this, &AudioWidget::downloadWebserviceAudio);
    connect(&m_player, &QMediaPlayer::stateChanged, this, &AudioWidget::slotPlaybackFinished);
    connect(&m_webserviceDownloadWatcher, &QFutureWatcher<bool>::finished, this, &AudioWidget::handleDownloadWebserviceFinished);

    playButton->setEnabled(false);
    playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    recordButton->setVisible(false); // TODO enable when recorder is implemented
    recordButton->setIcon(QIcon::fromTheme(QStringLiteral("media-record")));
    downloadButton->setEnabled(m_translateShell.isTranslateShellAvailable());

    audioUrlRequester->setEnabled(false);
}

void AudioWidget::setDocument(KEduVocDocument *doc)
{
    m_doc = doc;
}

QString AudioWidget::defaultOutputDirectory() const
{
    if (m_doc) {
        return m_doc->url().path().remove(".kvtml") + "/media";
    } else {
        return QString();
    }
}

void AudioWidget::setTranslation(KEduVocExpression *entry, int translation)
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

        if (m_player.state() == QMediaPlayer::PlayingState) {
            playButton->setEnabled(true);
        } else {
            playButton->setEnabled(false);
        }

        audioUrlRequester->clear();
    }
}

void AudioWidget::slotAudioFileChanged(const QString &url)
{
    if (m_entry) {
        m_entry->translation(m_currentTranslation)->setSoundUrl(QUrl::fromLocalFile(url));
    }
    playButton->setEnabled(!url.isEmpty());
}

void AudioWidget::startPlayback()
{
    QUrl soundFile = m_entry->translation(m_currentTranslation)->soundUrl();

    if (m_player.state() == QMediaPlayer::PlayingState) {
        m_player.stop();
    }
    m_player.setMedia(soundFile);
    m_player.play();
}

void AudioWidget::startRecordAudio()
{
    /// FIXME: When Phonon gains the ability to record sound, implement me :)
}

void AudioWidget::slotPlaybackFinished(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        playButton->setEnabled(!audioUrlRequester->url().isEmpty());
        break;

    case QMediaPlayer::PlayingState:
        playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
        playButton->setEnabled(false);
        break;

    case QMediaPlayer::PausedState:
        playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        playButton->setEnabled(!audioUrlRequester->url().isEmpty());
        break;
    }
}

void AudioWidget::downloadWebserviceAudio()
{
    if (!m_entry || !m_doc) {
        return;
    }

    QDir dir(defaultOutputDirectory());
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }

    QString soundFileName = m_entry->translation(m_currentTranslation)->soundUrl().toLocalFile();
    if (soundFileName.isEmpty()) {
        const QString fileName = m_entry->translation(m_currentTranslation)->text().replace(QRegExp("[^a-zA-Z\\s]"), QString());
        int index{0};
        do {
            soundFileName = dir.path() + '/' + fileName + '_' + QString::number(index) + ".ts";
            ++index;
            qDebug() << "test" << soundFileName;
        } while (QFile::exists(soundFileName));
        m_entry->translation(m_currentTranslation)->setSoundUrl(QUrl::fromLocalFile(soundFileName));
    }
    QFuture<bool> result = TranslateShellAdapter::downloadSoundFile(m_entry->translation(m_currentTranslation)->text(),
                                                                    m_doc->identifier(m_currentTranslation).locale(),
                                                                    soundFileName);
    m_webserviceDownloadWatcher.setFuture(result);
}

void AudioWidget::handleDownloadWebserviceFinished()
{
    if (!m_entry) {
        return;
    }
    if (QFile::exists(m_entry->translation(m_currentTranslation)->soundUrl().toLocalFile())) {
        playButton->setEnabled(true);
        playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        audioUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->soundUrl());
    }
}
