/***************************************************************************

    $Id$

                   dialog page for adjectives (comparison)

    -----------------------------------------------------------------------

    begin                : Sat Dec 4 18:09:29 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include "AdjEntryPage.h"

#include <langset.h>

#define Inherited AdjEntryPageData

#include <qkeycode.h>

#include <kapp.h>

AdjEntryPage::AdjEntryPage
(
        SpecFont_t        *font,
        QDialog           *dlgbook,
        bool               multi_sel,
        const Comparison  &comp,
        QWidget           *parent,
        const char        *name
)
	:
	Inherited( parent, name )
{
   comparisons = comp;

   connect( lev1Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev3Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev2Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );

   connect( lev1Field, SIGNAL(textChanged(const char*)), SLOT(lev1Changed(const char*)) );
   connect( lev2Field, SIGNAL(textChanged(const char*)), SLOT(lev2Changed(const char*)) );
   connect( lev3Field, SIGNAL(textChanged(const char*)), SLOT(lev3Changed(const char*)) );

   lev1Label->setBuddy(lev1Field);
   lev2Label->setBuddy(lev2Field);
   lev3Label->setBuddy(lev3Field);

   if (multi_sel) {
     lev1Field->setEnabled(false);
     lev2Field->setEnabled(false);
     lev3Field->setEnabled(false);
   }
   else {
     lev1Field->setText (comp.l1());
     lev2Field->setText (comp.l2());
     lev3Field->setText (comp.l3());
   }

   if (font != 0 && font->specfont) {
     QFont specfont = font->limitedFont();
     lev1Field->setFont (specfont);
     lev2Field->setFont (specfont);
     lev3Field->setFont (specfont);
   }
}


void AdjEntryPage::initFocus() const
{
  lev1Field->setFocus();
}


void AdjEntryPage::returnPressed()
{
   emit accept();
}


void AdjEntryPage::lev1Changed(const QString& s)
{
    comparisons.setL1 (s);
}


void AdjEntryPage::lev2Changed(const QString& s)
{
    comparisons.setL2 (s);
}


void AdjEntryPage::lev3Changed(const QString& s)
{
    comparisons.setL3 (s);
}


void AdjEntryPage::keyPressEvent( QKeyEvent *e )
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
