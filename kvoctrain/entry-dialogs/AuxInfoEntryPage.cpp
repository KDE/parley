/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 17:29:44 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

    connect(para_line, SIGNAL(textChanged()), SLOT(slotParaSelected()));
    connect(remark_line, SIGNAL(textChanged()), SLOT(slotRemarkSelected()));
    connect(examp_line, SIGNAL(textChanged()), SLOT(slotExampSelected()));
    connect(antonym_line, SIGNAL(textChanged()), SLOT(slotAntonymSelected()));
    connect(synonym_line, SIGNAL(textChanged()), SLOT(slotSynonymSelected()));
}


void AuxInfoEntryPage::setData(const QString &syno, const QString &anto, const QString &example, const QString &remark, const QString &para)
{
    synonym_line->setText(syno);
    antonym_line->setText(anto);
    examp_line->setText(example);
    remark_line->setText(remark);
    para_line->setText(para);

    setModified(false);
}


void AuxInfoEntryPage::slotSynonymSelected()
{
    setModified(true);
    synonym = synonym_line->toPlainText();
}


void AuxInfoEntryPage::slotAntonymSelected()
{
    setModified(true);
    antonym = antonym_line->toPlainText();
}


void AuxInfoEntryPage::slotRemarkSelected()
{
    setModified(true);
    remark = remark_line->toPlainText();
}


void AuxInfoEntryPage::slotExampSelected()
{
    setModified(true);
    example = examp_line->toPlainText();
}


void AuxInfoEntryPage::slotParaSelected()
{
    setModified(true);
    paraphrase = para_line->toPlainText();
}


QString AuxInfoEntryPage::getSynonym()
{
    return synonym.simplified();
}


QString AuxInfoEntryPage::getAntonym()
{
    return antonym.simplified();
}


QString AuxInfoEntryPage::getExample()
{
    return example.simplified();
}


QString AuxInfoEntryPage::getRemark()
{
    return remark.simplified();
}


QString AuxInfoEntryPage::getParaphrase()
{
    return paraphrase.simplified();
}


bool AuxInfoEntryPage::isModified()
{
    return modified;
}


void AuxInfoEntryPage::setModified(bool mod)
{
    modified = mod;
    if (mod)
        emit sigModified();
}

#include "AuxInfoEntryPage.moc"
