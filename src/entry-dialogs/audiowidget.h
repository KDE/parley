/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H


#include "ui_audiowidget.h"

namespace Phonon
{
    class MediaObject;
}

class KEduVocExpression;

class AudioWidget : public QWidget, public Ui::AudioWidget
{
    Q_OBJECT
public:
    explicit AudioWidget(QWidget* parent = 0);

public slots:
    /**
     * Update the word
     * @param entry 
     * @param translation 
     */
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void slotAudioFileChanged(const QString & url);
    void playAudio();
//     void recordAudio();
    void slotPlaybackFinished();

private:
    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression* m_entry;

    Phonon::MediaObject* m_player;
};
#endif
