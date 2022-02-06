/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include "translateshelladapter.h"
#include "ui_audiowidget.h"
#include <QFutureWatcher>
#include <QMediaPlayer>

class KEduVocDocument;
class KEduVocExpression;

namespace Editor
{
class AudioWidget : public QWidget, public Ui::AudioWidget
{
    Q_OBJECT
public:
    explicit AudioWidget(QWidget *parent = nullptr);
    void setDocument(KEduVocDocument *doc);

public Q_SLOTS:
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation(KEduVocExpression *entry, int translation);

private Q_SLOTS:
    void slotAudioFileChanged(const QString &url);
    void startPlayback();
    void slotPlaybackFinished(QMediaPlayer::State state);
    void startRecordAudio();
    void downloadWebserviceAudio();
    void handleDownloadWebserviceFinished();

private:
    QString defaultOutputDirectory() const;

    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int m_currentTranslation{-1};
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression *m_entry{nullptr};
    QMediaPlayer m_player; ///< media object for the files
    TranslateShellAdapter m_translateShell;
    KEduVocDocument *m_doc{nullptr};
    QFutureWatcher<bool> m_webserviceDownloadWatcher;
};

}

#endif
