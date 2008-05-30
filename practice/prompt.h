//
// C++ Interface: prompt
//
// Description: The prompt displays the question portion or original translation of an entry
///\file prompt.h
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


#ifndef PROMPT_H
#define PROMPT_H

#include <QString>
#include <QLabel>
#include <QPicture>

#include <KUrl>

class TextualPrompt : public QLabel
{
        Q_OBJECT

    public:
        TextualPrompt(QWidget * parent = 0);

    public slots:
        void slotSetText(const QString& text) {setText(text); };
};

class SoundPrompt : public QLabel
{
        Q_OBJECT


    public:
        SoundPrompt(QWidget * parent = 0);

    public slots:
        void slotSetSound(const KUrl& sound) { m_sound = sound; };
    private:
        KUrl m_sound;
};

class ImagePrompt : public QLabel
{
        Q_OBJECT


    public:
        ImagePrompt(QWidget * parent = 0);

    public slots:

        void slotSetImage(const KUrl& image);

    private:
        QPicture m_pic;
};


#endif

