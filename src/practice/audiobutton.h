/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_AUDIOBUTTON_H
#define PRACTICE_AUDIOBUTTON_H

#include <QToolButton>
#include <QMediaPlayer>
#include <QUrl>


namespace Practice
{

class AudioButton : public QToolButton
{
    Q_OBJECT
public:
    explicit AudioButton(QWidget *parent);
    void setSoundFile(const QUrl &url);

private Q_SLOTS:
    void playAudio();
    void stopAudio();
    void playerStateChanged(QMediaPlayer::State newState);

private:
    QMediaPlayer* m_player;        ///< media object for the files
    QUrl m_url;
};

}

#endif // PRACTICE_AUDIOBUTTON_H
