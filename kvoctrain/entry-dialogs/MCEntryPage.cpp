/***************************************************************************

    $Id: $

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin                : Mon Oct 29 18:09:29 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log: $

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

#include <langset.h>

#include <qkeycode.h>
#include <qlineedit.h>
#include <qlabel.h>

#include <kapp.h>

MCEntryPage::MCEntryPage
(
        SpecFont_t             *font,
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

   mc1Label->setBuddy(mc1Field);
   mc2Label->setBuddy(mc2Field);
   mc3Label->setBuddy(mc3Field);
   mc4Label->setBuddy(mc4Field);
   mc5Label->setBuddy(mc5Field);

   if (multi_sel) {
     mc1Field->setEnabled(false);
     mc2Field->setEnabled(false);
     mc3Field->setEnabled(false);
     mc4Field->setEnabled(false);
     mc5Field->setEnabled(false);
   }
   else {
     mc1Field->setText (mc.mc1());
     mc2Field->setText (mc.mc2());
     mc3Field->setText (mc.mc3());
     mc4Field->setText (mc.mc4());
     mc5Field->setText (mc.mc5());
   }

   if (font != 0 && font->specfont) {
     QFont specfont = font->limitedFont();
     mc1Field->setFont (specfont);
     mc2Field->setFont (specfont);
     mc3Field->setFont (specfont);
     mc4Field->setFont (specfont);
     mc5Field->setFont (specfont);
   }
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
    multiplechoice.setMC1 (s);
}


void MCEntryPage::mc2Changed(const QString& s)
{
    multiplechoice.setMC2 (s);
}


void MCEntryPage::mc3Changed(const QString& s)
{
    multiplechoice.setMC3 (s);
}


void MCEntryPage::mc4Changed(const QString& s)
{
    multiplechoice.setMC4 (s);
}


void MCEntryPage::mc5Changed(const QString& s)
{
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

#include "MCEntryPage.moc"
