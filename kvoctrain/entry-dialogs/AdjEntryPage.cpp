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
    Revision 1.6  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.4  2001/10/13 21:15:49  arnold
    tested and fixed changes from previous cvs updatecd

    Revision 1.3  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.2  2001/10/05 17:33:17  arnold
    FIX: slot signature changed to KDE2

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


#include "AdjEntryPage.h"

#include <langset.h>

#include <qkeycode.h>
#include <qlineedit.h>
#include <qlabel.h>

#include <kapplication.h>

AdjEntryPage::AdjEntryPage
(
        QDialog           *dlgbook,
        bool               multi_sel,
        const Comparison  &comp,
        QWidget           *parent,
        const char        *name
)
	:
	AdjEntryPageForm( parent, name )
{
   comparisons = comp;

   connect( lev1Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev3Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );
   connect( lev2Field, SIGNAL(returnPressed()), SLOT(returnPressed()) );

   connect( lev1Field, SIGNAL(textChanged(const QString&)), SLOT(lev1Changed(const QString&)) );
   connect( lev2Field, SIGNAL(textChanged(const QString&)), SLOT(lev2Changed(const QString&)) );
   connect( lev3Field, SIGNAL(textChanged(const QString&)), SLOT(lev3Changed(const QString&)) );

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

#include "AdjEntryPage.moc"
