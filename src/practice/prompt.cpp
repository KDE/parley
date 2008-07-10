//
// C++ Implementation: prompt
//
// Description: Implementation of the prompt (or question) displaying class
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

#include "prompt.h"

#include <KDebug>
#include <kio/netaccess.h>
#include <KRandomSequence>
#include <KRandom>

TextualPrompt::TextualPrompt ( KSvgRenderer * renderer, const QString& elementId ) :
        m_renderer ( renderer )
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    m_backgroundRect = renderer->boundsOnElement ( elementId );
    setPos ( m_backgroundRect.x() + m_backgroundRect.width() / 20.0, m_backgroundRect.y() + m_backgroundRect.height() / 4.0 );
    adjustSize();
};

void TextualPrompt::slotSetText ( const QString& text ) {setPlainText ( text ); };

ImagePrompt::ImagePrompt ( KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent ) :
        QLabel ( parent ),
        m_pic ( QPixmap() ),
        m_renderer ( renderer )
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    QRect bounds = m_renderer->boundsOnElement ( elementId ).toRect();
    setGeometry ( view->mapToScene ( bounds ).boundingRect().toRect() );

    slotSetImage ( KUrl() );
};

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
};

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

void ImagePrompt::slotSetImage ( const KUrl& image )
{
    kDebug() << image;
    if ( !image.isEmpty() )
    {
        QString tmpFile;
        if ( KIO::NetAccess::download ( image, tmpFile, this ) )
        {
            m_pic.load ( tmpFile );
            KIO::NetAccess::removeTempFile ( tmpFile );
            setVisible ( true );
        }
        else
        {
            //KMessageBox::error(this, KIO::NetAccess::lastErrorString());
            kDebug() << KIO::NetAccess::lastErrorString();
        }
    }
    else
    {
        m_pic = QPixmap();
        setVisible ( false );
    }
    setPixmap ( m_pic );
}


MixedLettersPrompt::MixedLettersPrompt(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent)
: QWidget(parent), m_renderer(renderer), m_scene(view->scene()), m_view(view)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    m_backgroundRect = renderer->boundsOnElement ( elementId );
};

void MixedLettersPrompt::slotSetText (const QString& solution )
{
    m_solution = solution;

    // remove old items
    foreach ( QGraphicsItem* item, m_letters) {
        m_scene->removeItem(item);
        delete item;
    }
    m_letters.clear();


    QList<int> positions;
    for ( int i = 0; i < solution.length(); i++ )
    {
        positions.append((m_backgroundRect.width()-20) * i/solution.length()  + 10 );
    }

    KRandomSequence random;
    for ( int i = 0; i < solution.length(); i++ )
    {
        QGraphicsTextItem* letter = new QGraphicsTextItem( QString(solution[i]) );
        letter->translate(positions.takeAt( random.getLong(positions.size())),
        m_backgroundRect.height()/4 - 5 + random.getLong( m_backgroundRect.height()/2 ) );
        m_letters.append(letter);
        m_scene->addItem(letter);
        letter->setPos(m_view->mapToScene(letter->x(), letter->y()));
    }
}

void MixedLettersPrompt::slotAnswerChanged(const QString& answer)
{
    for ( int i = 0; i < m_solution.length(); i++ ) {
        if ( answer[i] == m_solution[i] ) {
            m_letters[i]->setHtml("<b><font color=\"#188C18\">" + m_solution[i] + "</font></b>");
        } else {
//             if ( i >= answerLineEdit->text().length() ) {
                // no input yet
            m_letters[i]->setHtml(QString(m_solution[i]));
//             } else {
                // wrong
//                 m_answerTextItems[i]->setHtml("<b><font color=\"#FF0000\">" + solution[i] + "</font></b>");
//             }
        }
    }
}

