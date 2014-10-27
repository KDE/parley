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

#ifndef PRACTICE_AUDIOBUTTON_H
#define PRACTICE_AUDIOBUTTON_H

#include <QtWidgets/QToolButton>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>

class QMediaPlayer;

namespace Practice
{

class AudioButton : public QToolButton
{
    Q_OBJECT
public:
    AudioButton(QWidget *parent);
    void setSoundFile(QUrl url);

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
