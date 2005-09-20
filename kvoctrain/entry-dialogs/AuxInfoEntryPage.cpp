/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 17:29:44 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#include <q3multilineedit.h>

#include "AuxInfoEntryPage.h"
#include "EntryDlg.h"

#include <langset.h>

#include <LineList.h>

AuxInfoEntryPage::AuxInfoEntryPage
(
  EntryDlg   *_dlgbook,
  bool        multi_sel,
  QString     syno,
  QString     anto,
  QString     exam,
  QString     rem,
  QString     para,
  QWidget    *parent,
  const char *name
)
 : AuxInfoEntryPageForm( parent, name ), dlgbook(_dlgbook)

{
  QFontMetrics fm (synonym_line->font());
  int sz = fm.lineSpacing();

  synonym_line->setMaximumHeight(sz*3);
  antonym_line->setMaximumHeight(sz*3);
  para_line->setMaximumHeight(sz*3);
  remark_line->setMaximumHeight(sz*3);
  examp_line->setMaximumHeight(sz*3);

  connect( para_line, SIGNAL(textChanged()), SLOT(slotParaSelected()) );
  connect( remark_line, SIGNAL(textChanged()), SLOT(slotRemarkSelected()) );
  connect( examp_line, SIGNAL(textChanged()), SLOT(slotExampSelected()) );
  connect( antonym_line, SIGNAL(textChanged()), SLOT(slotAntonymSelected()) );
  connect( synonym_line, SIGNAL(textChanged()), SLOT(slotSynonymSelected()) );

  setData(multi_sel, syno, anto, exam, rem, para);
}


void AuxInfoEntryPage::setData(bool multi_sel, QString syno, QString anto, QString  example, QString remark, QString para)
{
  synonym_line->setText(syno);
  antonym_line->setText(anto);
  examp_line->setText(example);
  remark_line->setText(remark);
  para_line->setText(para);

  if (multi_sel) {
    synonym_line ->setEnabled(false);
    antonym_line ->setEnabled(false);
    remark_line ->setEnabled(false);
    examp_line ->setEnabled(false);
    para_line ->setEnabled(false);
  }

  setModified(false);
}


void AuxInfoEntryPage::slotSynonymSelected()
{
  setModified(true);
  synonym = synonym_line->text();
}


void AuxInfoEntryPage::slotAntonymSelected()
{
  setModified(true);
  antonym = antonym_line->text();
}


void AuxInfoEntryPage::slotRemarkSelected ()
{
  setModified(true);
  remark = remark_line->text();
}


void AuxInfoEntryPage::slotExampSelected ()
{
  setModified(true);
  example = examp_line->text();
}


void AuxInfoEntryPage::slotParaSelected ()
{
  setModified(true);
  paraphrase = para_line->text();
}


QString AuxInfoEntryPage::getSynonym ()
{
  normalize(synonym);
  return synonym;
}


QString AuxInfoEntryPage::getAntonym ()
{
  normalize(antonym);
  return antonym;
}


QString AuxInfoEntryPage::getExample ()
{
  normalize(example);
  return example;
}


QString AuxInfoEntryPage::getRemark  ()
{
  normalize(remark);
  return remark;
}


QString AuxInfoEntryPage::getParaphrase  ()
{
  normalize(paraphrase);
  return paraphrase;
}


void AuxInfoEntryPage::normalize (QString &str)
{
  LineList ll (str);
  ll.normalizeWS();
  str = ll.allLines();
}


bool AuxInfoEntryPage::isModified()
{
  return modified;
}


void AuxInfoEntryPage::setEnabled(int enable)
{
  bool ena = enable == EntryDlg::EnableAll;

  synonym_line->setEnabled (ena);
  antonym_line->setEnabled (ena);
  para_line->setEnabled (ena);
  remark_line->setEnabled (ena);
  examp_line->setEnabled (ena);
}


void AuxInfoEntryPage::setModified(bool mod)
{
  modified = mod;
  if (mod)
    emit sigModified();
}

#include "AuxInfoEntryPage.moc"
