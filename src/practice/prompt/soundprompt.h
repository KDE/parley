//
// C++ Interface: soundprompt
//
// Description:
//
//
// Copyright 2008 David Capel <wot.narg@gmail.com>
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef SOUNDPROMPT_H
#define SOUNDPROMPT_H

#include <QPushButton>

#include <Phonon/MediaObject>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
#include <Phonon/Global>
#include <KUrl>



class KSvgRenderer;
class QString;
class QGraphicsView;
class QWidget;
class ActiveArea;

/**
* @class SoundPrompt
* @author David Capel <wot.narg@gmail.com>
* @brief This widget provides a button that, when pushed, will play a sound.
*/
class SoundPrompt : public QPushButton
{
        Q_OBJECT


    public:
        SoundPrompt(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);
        ~SoundPrompt();
    public slots:
        /// Sets the sound to play. Pass an empty KUrl to clear it.
        void slotSetSound(const KUrl& sound);
        /// Plays the previously set sound.
        void slotPlay();
    private:
        KUrl m_sound;
        KSvgRenderer * m_renderer;
        class QRectF m_backgroundRect;
        class Phonon::MediaObject * m_media;
        QString m_tmpFile;
        ActiveArea * m_area;
};

#endif
