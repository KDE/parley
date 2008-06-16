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

SoundPrompt::SoundPrompt(KSvgRenderer * renderer, QWidget * parent) : QLabel(parent), m_renderer(renderer) {};
ImagePrompt::ImagePrompt(KSvgRenderer * renderer, QWidget * parent) : QLabel(parent), m_pic(QPicture()), m_renderer(renderer) {};

void ImagePrompt::slotSetImage(const KUrl& image)
{
    if (!image.isEmpty())
        m_pic.load(image.url());
    else
        m_pic = QPicture();
    setPicture(m_pic);
}
