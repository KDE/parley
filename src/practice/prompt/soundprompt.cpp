//
// C++ Implementation: soundprompt
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

#include "soundprompt.h"
#include "../activearea.h"


#include <KDebug>
#include <kio/netaccess.h>
#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <KUrl>

SoundPrompt::SoundPrompt ( KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent ) :
        QPushButton ( parent ),
        m_renderer ( renderer ),
        m_media(0),
        m_area(area)
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);

    setText ( "Play Sound" );

    QRectF bounds = m_renderer->boundsOnElement(tId);
     bounds.translate(area->offset(tId));
     setGeometry(bounds.toRect());

    connect ( this, SIGNAL ( clicked() ), this, SLOT ( slotPlay() ) );
}

void SoundPrompt::slotSetSound ( const KUrl& sound )
{
    m_sound = sound;
    if ( !sound.isEmpty() ) {
        setVisible ( true );
    } else {
        setVisible ( false );
    }
}

SoundPrompt::~SoundPrompt()
{
    slotSetSound(KUrl()); // removes any lingering tmp files.
}

void SoundPrompt::slotPlay()
{
    if (m_sound.isEmpty()) {
        return;
    }

    if (!m_media ) {
        m_media = Phonon::createPlayer(Phonon::NotificationCategory, m_sound);
        m_media->setParent(this);
    } else {
        if (m_media->state() == Phonon::PlayingState) {
            m_media->stop();
        }
        m_media->setCurrentSource ( m_sound );
    }
    m_media->play();
}

