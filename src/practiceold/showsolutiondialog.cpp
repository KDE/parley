/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "showsolutiondialog.h"
#include "prefs.h"

#include "testentry.h"
#include <keduvocexpression.h>
#include <QTimer>
#include <Phonon/MediaObject>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
#include <phononnamespace.h>

ShowSolutionDialog::ShowSolutionDialog(TestEntry* entry, QWidget* parent)
    :KDialog(parent)
{
    m_entry = entry;
    m_player = 0;

    setupUi(mainWidget());
    setButtons(KDialog::Ok);

    if ( Prefs::showSolutionAfterAnswer() ) {
        QTimer::singleShot(Prefs::showSolutionAfterAnswerTime() * 1000, this, SLOT(accept()));
    }

    QFont font = questionLabel->font();
    font.setPointSize(font.pointSize()*2);

    questionLabel->setAutoFillBackground(true);
    solutionLabel->setAutoFillBackground(true);
    questionLabel->setFont(font);
    dividerLabel->setFont(font);
    solutionLabel->setFont(font);

    QPalette qp = QPalette(questionLabel->palette());
    qp.setColor(QPalette::Active, QPalette::Window, Qt::white);
    questionLabel->setPalette(qp);
    solutionLabel->setPalette(qp);

    questionLabel->setText(entry->entry()->translation(Prefs::questionLanguage())->text());
    solutionLabel->setText(entry->entry()->translation(Prefs::solutionLanguage())->text());

    if ( !m_entry->entry()->translation(Prefs::questionLanguage())->soundUrl().url().isEmpty()  ) {
        playQuestionSoundButton->setIcon(KIcon("media-playback-start"));
        connect(playQuestionSoundButton, SIGNAL(clicked()), SLOT(audioPlayFromIdentifier()));
    } else {
        playQuestionSoundButton->setVisible(false);
    }

    if ( !m_entry->entry()->translation(Prefs::solutionLanguage())->soundUrl().url().isEmpty()  ) {
        playSolutionSoundButton->setIcon(KIcon("media-playback-start"));
        connect(playSolutionSoundButton, SIGNAL(clicked()), SLOT(audioPlayToIdentifier()));
    } else {
        playSolutionSoundButton->setVisible(false);
    }

    KConfigGroup cg(KGlobal::config(), "ShowSolutionDialog");
    restoreDialogSize(cg);
}


ShowSolutionDialog::~ShowSolutionDialog()
{
    m_player->deleteLater();

    KConfigGroup cg(KGlobal::config(), "ShowSolutionDialog");
    KDialog::saveDialogSize(cg);
}


void ShowSolutionDialog::audioPlayFromIdentifier()
{
    QString file = m_entry->entry()->translation(Prefs::questionLanguage())->soundUrl().url();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void ShowSolutionDialog::audioPlayToIdentifier()
{
    QString file = m_entry->entry()->translation(Prefs::solutionLanguage())->soundUrl().url();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void ShowSolutionDialog::audioPlayFile(const QString & soundFile)
{
    kDebug() << "Attempting to play sound: " << soundFile;
    if (!m_player)
    {
        kDebug() << "Creating Phonon player...";
        m_player = new Phonon::MediaObject(this);
        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::NoCategory, this);
        createPath(m_player, audioOutput);
    }
    m_player->stop();
    m_player->setCurrentSource(soundFile);
    m_player->play();
}

#include "showsolutiondialog.moc"

