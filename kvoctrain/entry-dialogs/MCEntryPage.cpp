/***************************************************************************

    $Id$

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin                : Mon Oct 29 18:09:29 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.3  2001/12/26 15:11:29  mueller
    CVSSILINT: fixincludes

    Revision 1.2  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.1  2001/10/30 14:53:34  arnold
    added property 'multiple choice'


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include "MCEntryPage.h"
#include "EntryDlg.h"

#include <langset.h>

#include <qkeycode.h>
#include <qlineedit.h>
#include <qlabel.h>

#include <kapplication.h>

MCEntryPage::MCEntryPage
(
        QDialog                *dlgbook,
        bool                    multi_sel,
        const MultipleChoice   &mc,
        QWidget                *parent,
        const char             *name
)
	:
	MCEntryPageForm( parent, name )
{
   multiplechoice = mc;

   connect( mc1Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( mc2Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( mc3Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( mc4Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( mc5Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );

   connect( mc1Field, SIGNAL(textChanged(const QString&)), SLOT(mc1Changed(const QString&)) );
   connect( mc2Field, SIGNAL(textChanged(const QString&)), SLOT(mc2Changed(const QString&)) );
   connect( mc3Field, SIGNAL(textChanged(const QString&)), SLOT(mc3Changed(const QString&)) );
   connect( mc4Field, SIGNAL(textChanged(const QString&)), SLOT(mc4Changed(const QString&)) );
   connect( mc5Field, SIGNAL(textChanged(const QString&)), SLOT(mc5Changed(const QString&)) );

   setData(multi_sel, mc);
}


void MCEntryPage::setData(bool multi_sel,
                          const MultipleChoice &mc)
{
     mc1Field->setText (mc.mc1());
     mc2Field->setText (mc.mc2());
     mc3Field->setText (mc.mc3());
     mc4Field->setText (mc.mc4());
     mc5Field->setText (mc.mc5());

     if (multi_sel) {
       mc1Field->setEnabled(false);
       mc2Field->setEnabled(false);
       mc3Field->setEnabled(false);
       mc4Field->setEnabled(false);
       mc5Field->setEnabled(false);
     }

     setModified(false);
}


void MCEntryPage::initFocus() const
{
  mc1Field->setFocus();
}


void MCEntryPage::returnPressed()
{
   emit accept();
}


void MCEntryPage::mc1Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC1 (s);
}


void MCEntryPage::mc2Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC2 (s);
}


void MCEntryPage::mc3Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC3 (s);
}


void MCEntryPage::mc4Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC4 (s);
}


void MCEntryPage::mc5Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC5 (s);
}


void MCEntryPage::keyPressEvent( QKeyEvent *e )
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


bool MCEntryPage::isModified()
{
  return modified;
}


void MCEntryPage::setModified(bool mod)
{
  modified = mod;
  if (mod)
    emit sigModified();
}


void MCEntryPage::setEnabled(int enable)
{
  bool ena = enable == EntryDlg::EnableAll;

  mc1Field->setEnabled (ena);
  mc2Field->setEnabled (ena);
  mc3Field->setEnabled (ena);
  mc4Field->setEnabled (ena);
  mc5Field->setEnabled (ena);
}


#include "MCEntryPage.moc"
