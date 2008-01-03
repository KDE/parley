/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 17:29:44 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "additionaleditpage.h"
#include "EntryDlg.h"
#include <QTextEdit>
#include <Phonon/MediaObject>

AdditionalEditPage::AdditionalEditPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_player = 0;

    setupUi(this);

    connect(paraphraseLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(commentLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(exampleLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(antonymLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(synonymLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(audioUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged()));
    connect(imageUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotImageChanged(const QString&)));

    connect(audioPlayButton, SIGNAL(clicked()), this, SLOT(playAudio()) );
}


bool AdditionalEditPage::isModified()
{
    if ( m_currentRow < 0 || m_currentTranslation < 0 ) {
        return false;
    }

    bool modified = false;

//     if (synonymLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym()) {
//         modified = true;
//     }
//     if (antonymLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym()) {
//         modified = true;
//     }
//     if (exampleLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).example()) {
//         modified = true;
//     }
//     if (commentLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment()) {
//         modified = true;
//     }
//     if (paraphraseLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase()) {
//         modified = true;
//     }
    return modified;
}


void AdditionalEditPage::setData(int row, int col)
{
//     m_currentRow = row;
//     m_currentTranslation = col;
// 
//     synonymLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym());
//     antonymLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym());
//     exampleLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).example());
//     commentLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment());
//     paraphraseLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase());
// 
//     if ( !m_doc->entry(m_currentRow)->translation(m_currentTranslation).soundUrl().isEmpty() ) {
//         audioUrlRequester->setUrl( m_doc->entry(
//                     m_currentRow)->translation(m_currentTranslation).soundUrl() );
//     } else {
//         audioUrlRequester->clear();
//     }
// 
//     imagePreviewLabel->setText(i18nc("@label image preview is empty", "No Image"));
// 
//     if ( !m_doc->entry(m_currentRow)->translation(m_currentTranslation).imageUrl().isEmpty() ) {
//         imageUrlRequester->setUrl( m_doc->entry(
//                     m_currentRow)->translation(m_currentTranslation).imageUrl() );
//     } else {
//         imageUrlRequester->clear();
//     }
}


void AdditionalEditPage::commitData()
{
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setComment(commentLineEdit->text());
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setSynonym(synonymLineEdit->text());
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setAntonym(antonymLineEdit->text());
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setExample(exampleLineEdit->text());
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setParaphrase(paraphraseLineEdit->text());
// 
//     // sound and image
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setSoundUrl( audioUrlRequester->url() );
// 
//     m_doc->entry(m_currentRow)->translation(m_currentTranslation).setImageUrl( imageUrlRequester->url() );
//     foreach (int j, m_doc->entry(m_currentRow)->translationIndices()) {
//         if ( m_doc->entry(m_currentRow)->translation(j).imageUrl().isEmpty() ) {
//             m_doc->entry(m_currentRow)->translation(j).setImageUrl( imageUrlRequester->url() );
//         }
//     }
}


void AdditionalEditPage::clear()
{
    synonymLineEdit->setText(QString());
    antonymLineEdit->setText(QString());
    exampleLineEdit->setText(QString());
    commentLineEdit->setText(QString());
    paraphraseLineEdit->setText(QString());
}

void AdditionalEditPage::slotDataChanged()
{
    emit sigModified();
}


void AdditionalEditPage::playAudio()
{
    KUrl soundFile = audioUrlRequester->url();
    kDebug() << "sound file: " << soundFile.url();

    if (!m_player)
    {
        m_player = Phonon::createPlayer(Phonon::NoCategory, soundFile);
        m_player->setParent(this);
    } else {
        m_player->setCurrentSource(soundFile);
    }
    m_player->play();
}

void AdditionalEditPage::slotImageChanged(const QString & url)
{
    imagePreviewLabel->setPixmap(QPixmap(url).scaled(imagePreviewLabel->size(), Qt::KeepAspectRatio));
    slotDataChanged();
}


#include "additionaleditpage.moc"
