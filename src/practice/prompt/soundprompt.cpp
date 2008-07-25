//
// C++ Implementation: soundprompt
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
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

#include <KDebug>
#include <kio/netaccess.h>
#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <KUrl>

SoundPrompt::SoundPrompt ( KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent ) :
        QPushButton ( parent ),
        m_renderer ( renderer )
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    if ( !m_media )
    {
        m_media = new Phonon::MediaObject ( this );
        Phonon::AudioOutput * audioOutput = new Phonon::AudioOutput ( Phonon::MusicCategory, this );
        createPath ( m_media, audioOutput );
    }

    setText ( "Play Sound" );
    QRect bounds = m_renderer->boundsOnElement ( elementId ).toRect();
    setGeometry ( view->mapToScene ( bounds ).boundingRect().toRect() );

    connect ( this, SIGNAL ( clicked() ), this, SLOT ( slotPlay() ) );

    slotSetSound ( KUrl() );
}

void SoundPrompt::slotSetSound ( const KUrl& sound )
{
    // remove the temp file from the last call
    // I'll make sure this function is called at destruction to remove any lingering temp files.
    KIO::NetAccess::removeTempFile ( m_tmpFile );

    //kDebug() << sound;
    m_sound = sound;
    if ( !sound.isEmpty() )
    {
        QString tmpFile;
        if ( KIO::NetAccess::download ( sound, m_tmpFile, this ) )
        {
            m_media->setCurrentSource ( tmpFile );
            setVisible ( true );
        }
        else
        {
            //KMessageBox::error(this, KIO::NetAccess::lastErrorString());
            kDebug() << KIO::NetAccess::lastErrorString();
        }
    }
        else
            setVisible ( false );
}

SoundPrompt::~SoundPrompt()
{
    slotSetSound(KUrl()); // removes any lingering tmp files.
}

void SoundPrompt::slotPlay()
{
    m_media->setCurrentSource ( m_sound );
    m_media->play();
}

