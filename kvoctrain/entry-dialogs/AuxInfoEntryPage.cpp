/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 17:29:44 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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
#include <kvtlanguages.h>

AuxInfoEntryPage::AuxInfoEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;

    setupUi(this);
    QFontMetrics fm(synonym_line->font());
    int sz = fm.lineSpacing();

    synonym_line->setMaximumHeight(sz*3);
    antonym_line->setMaximumHeight(sz*3);
    para_line->setMaximumHeight(sz*3);
    remark_line->setMaximumHeight(sz*3);
    examp_line->setMaximumHeight(sz*3);

    connect(para_line, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(remark_line, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(examp_line, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(antonym_line, SIGNAL(textChanged()), SLOT(slotDataChanged()));
    connect(synonym_line, SIGNAL(textChanged()), SLOT(slotDataChanged()));
}


bool AuxInfoEntryPage::isModified()
{
    if ( m_currentRow < 0 || m_currentTranslation < 0 ) {
        return false;
    }

    bool modified = false;

    if (synonym_line->toPlainText() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym()) {
        modified = true;
    }
    if (antonym_line->toPlainText() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym()) {
        modified = true;
    }
    if (examp_line->toPlainText() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).example()) {
        modified = true;
    }
    if (remark_line->toPlainText() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment()) {
        modified = true;
    }
    if (para_line->toPlainText() != m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase()) {
        modified = true;
    }
    return modified;
}


void AuxInfoEntryPage::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;

    synonym_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).synonym());
    antonym_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).antonym());
    examp_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).example());
    remark_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).comment());
    para_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).paraphrase());
}


void AuxInfoEntryPage::commitData()
{
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setComment(remark_line->toPlainText());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setSynonym(synonym_line->toPlainText());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setAntonym(antonym_line->toPlainText());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setExample(examp_line->toPlainText());
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setParaphrase(para_line->toPlainText());
}


void AuxInfoEntryPage::clear()
{
    synonym_line->setText(QString());
    antonym_line->setText(QString());
    examp_line->setText(QString());
    remark_line->setText(QString());
    para_line->setText(QString());
}

void AuxInfoEntryPage::slotDataChanged()
{
    emit sigModified();
}


#include "AuxInfoEntryPage.moc"
