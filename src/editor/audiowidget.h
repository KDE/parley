/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include "ui_audiowidget.h"

class QMediaPlayer;

class KEduVocExpression;

namespace Editor
{

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

    QMediaPlayer* m_player;        ///< media object for the files
};

}

#endif
