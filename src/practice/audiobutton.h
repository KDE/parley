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

#include <QtGui/QToolButton>
#include <kurl.h>

#include <Phonon/MediaObject>

namespace Practice
{

class AudioButton : public QToolButton
{
    Q_OBJECT
public:
    AudioButton(QWidget *parent);
    void setSoundFile(KUrl url);

private Q_SLOTS:
    void playAudio();
    void playerStateChanged(Phonon::State newState);

private:
    Phonon::MediaObject *m_player;
    KUrl m_url;
};

}

#endif // PRACTICE_AUDIOBUTTON_H
