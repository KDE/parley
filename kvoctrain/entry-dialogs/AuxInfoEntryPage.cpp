/***************************************************************************

    $Id$

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 17:29:44 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.5  2001/12/26 15:11:29  mueller
    CVSSILINT: fixincludes

    Revision 1.4  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.3  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:40:37  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include <qkeycode.h>
#include <qmultilineedit.h>
#include <qlabel.h>
#include <qlistbox.h>

#include <kapplication.h>

#include "AuxInfoEntryPage.h"
#include "EntryDlg.h"

#include <langset.h>

#include <LineList.h>

AuxInfoEntryPage::AuxInfoEntryPage
(
        QDialog    *dlgbook,
        bool        multi_sel,
        QString     syno,
        QString     anto,
        QString     exam,
        QString     rem,
        QString     para,
	QWidget    *parent,
	const char *name
)
	:
	AuxInfoEntryPageForm( parent, name )
{
        QFontMetrics fm (synonym_line->font());
        int sz = fm.lineSpacing();
	
	synonym_line->setMaximumHeight(sz*3);
	antonym_line->setMaximumHeight(sz*3);
	para_line->setMaximumHeight(sz*3);
	remark_line->setMaximumHeight(sz*3);
	examp_line->setMaximumHeight(sz*3);
/*
	connect( para_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( remark_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( examp_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( synonym_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( antonym_line, SIGNAL(returnPressed()), SLOT(accept()) );
*/
        connect( para_line, SIGNAL(textChanged()), SLOT(slotParaSelected()) );
	connect( remark_line, SIGNAL(textChanged()), SLOT(slotRemarkSelected()) );
	connect( examp_line, SIGNAL(textChanged()), SLOT(slotExampSelected()) );
	connect( antonym_line, SIGNAL(textChanged()), SLOT(slotAntonymSelected()) );
	connect( synonym_line, SIGNAL(textChanged()), SLOT(slotSynonymSelected()) );

	setData(multi_sel,
                syno,
                anto,
                exam,
                rem,
                para);
}


void AuxInfoEntryPage::setData(
        bool        multi_sel,
        QString     syno,
        QString     anto,
        QString     example,
        QString     remark,
        QString     para)
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


void AuxInfoEntryPage::initFocus() const
{
  synonym_line->setFocus();
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


void AuxInfoEntryPage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
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
