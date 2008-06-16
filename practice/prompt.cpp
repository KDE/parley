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

TextualPrompt::TextualPrompt(KSvgRenderer * renderer, const QString& elementId) :
         m_renderer(renderer)
{
    m_backgroundRect = renderer->boundsOnElement(elementId);
    setPos(m_backgroundRect.x() + m_backgroundRect.width() / 20.0, m_backgroundRect.y() + m_backgroundRect.height() / 4.0);
    adjustSize();
};

void TextualPrompt::slotSetText(const QString& text) {setPlainText(text); };

ImagePrompt::ImagePrompt(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent) :
    QLabel(parent),
    m_pic(QPicture()),
    m_renderer(renderer)
{
     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());

     slotSetImage(KUrl());
};

SoundPrompt::SoundPrompt(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent) :
    QPushButton(parent),
    m_renderer(renderer)
{
    if (!m_media)
    {
        m_media = new Phonon::MediaObject(this);
        Phonon::AudioOutput * audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
        createPath(m_media, audioOutput);
    }

    setText("Play Sound");
    QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
    setGeometry(view->mapToScene(bounds).boundingRect().toRect());

    connect(this, SIGNAL(clicked()), this, SLOT(slotPlay()));

    slotSetSound(KUrl());
};

void SoundPrompt::slotSetSound(const KUrl& sound)
{
    m_sound = sound;
    if (!sound.isEmpty())
        setVisible(true);
    else
        setVisible(false);
};


void SoundPrompt::slotPlay()
{
    m_media->setCurrentSource(m_sound);
    m_media->play();
}

void ImagePrompt::slotSetImage(const KUrl& image)
{
    if (!image.isEmpty())
    {
        m_pic.load(image.url());
        setVisible(true);
    }
    else
    {
        m_pic = QPicture();
        setVisible(false);
    }
    setPicture(m_pic);
}
