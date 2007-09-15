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

#include "AuxInfoEntryPage.h"

#include <QTextEdit>

#include "EntryDlg.h"

AuxInfoEntryPage::AuxInfoEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;

    setupUi(this);
    QFontMetrics fm(synonymLineEdit->font());
    int sz = fm.lineSpacing();

    synonymLineEdit->setMaximumHeight(sz*3);
    antonymLineEdit->setMaximumHeight(sz*3);
    paraphraseLineEdit->setMaximumHeight(sz*3);
    commentLineEdit->setMaximumHeight(sz*3);
    exampleLineEdit->setMaximumHeight(sz*3);

    connect(paraphraseLineEdit, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(commentLineEdit, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(exampleLineEdit, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(antonymLineEdit, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(synonymLineEdit, SIGNAL(textChanged()), SLOT(slotDataChanged()));
}


bool AuxInfoEntryPage::isModified()
{
    if ( m_currentRow < 0 || m_currentTranslation < 0 ) {
        return false;
    }

    bool modified = false;

    if (synonymLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym()) {
        modified = true;
    }
    if (antonymLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym()) {
        modified = true;
    }
    if (exampleLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).example()) {
        modified = true;
    }
    if (commentLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment()) {
        modified = true;
    }
    if (paraphraseLineEdit->text() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase()) {
        modified = true;
    }
    return modified;
}


void AuxInfoEntryPage::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;

    synonymLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym());
    antonymLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym());
    exampleLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).example());
    commentLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment());
    paraphraseLineEdit->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase());
}


void AuxInfoEntryPage::commitData()
{
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setComment(commentLineEdit->text());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setSynonym(synonymLineEdit->text());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setAntonym(antonymLineEdit->text());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setExample(exampleLineEdit->text());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setParaphrase(paraphraseLineEdit->text());
}


void AuxInfoEntryPage::clear()
{
    synonymLineEdit->setText(QString());
    antonymLineEdit->setText(QString());
    exampleLineEdit->setText(QString());
    commentLineEdit->setText(QString());
    paraphraseLineEdit->setText(QString());
}

void AuxInfoEntryPage::slotDataChanged()
{
    emit sigModified();
}


#include "AuxInfoEntryPage.moc"
