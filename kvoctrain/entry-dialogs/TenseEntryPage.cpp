/***************************************************************************

    $Id$

                     tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin                : Sat Nov 27 20:20:34 1999
                                           
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


#define Inherited TenseEntryPageData

#include <qkeycode.h>

#include <kapp.h>

#include <eadebug.h>
#include <langset.h>

#include "TenseEntryPage.h"


TenseEntryPage::TenseEntryPage
(
        SpecFont_t        *font,
        QDialog           *dlgbook,
        bool               multi_sel,
        const Conjugation &con_prefix,
        const Conjugation &conjug,
        QWidget           *parent,
        const char        *name
)                       
	:
	Inherited( parent, name ),
        multi_mode(multi_sel)
{

	connect( thirdN_plural, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( thirdN_singular, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( thirdM_plural, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( thirdM_singular, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( thirdF_plural, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( thirdF_singular, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( third_p_common, SIGNAL(toggled(bool)), SLOT(slotThirdPCommonToggled(bool)) );
	connect( third_s_common, SIGNAL(toggled(bool)), SLOT(slotThirdSCommonToggled(bool)) );
	connect( second_plural, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( second_singular, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( first_plural, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( first_singular, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( b_next, SIGNAL(clicked()), SLOT(slotNextConj()) );
	connect( tensebox, SIGNAL(highlighted(int)), SLOT(slotTenseSelected(int)) );

	connect( thirdN_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdNPluralChanged(const QString&)) );
	connect( thirdN_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdNSingularChanged(const QString&)) );
	connect( thirdM_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdMPluralChanged(const QString&)) );
	connect( thirdM_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdMSingularChanged(const QString&)) );
	connect( thirdF_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdFPluralChanged(const QString&)) );
	connect( thirdF_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdFSingularChanged(const QString&)) );
	connect( second_plural, SIGNAL(textChanged(const QString&)), SLOT(secondPluralChanged(const QString&)) );
	connect( second_singular, SIGNAL(textChanged(const QString&)), SLOT(secondSingularChanged(const QString&)) );
	connect( first_plural, SIGNAL(textChanged(const QString&)), SLOT(firstPluralChanged(const QString&)) );
	connect( first_singular, SIGNAL(textChanged(const QString&)), SLOT(firstSingularChanged(const QString&)) );

   prefix = con_prefix;
   conjugations = conjug;
   selection = "";
   if (multi_mode)
     tensebox->setEnabled(false);
   else  {
     for (int i = 0; i <  Conjugation::numTenses(); i++)
       tensebox->insertItem (Conjugation::getName(i) );
   }

   slotTenseSelected(0);
/*
   // FIXME: fill labels with prefixes ?

   label_first_plural->setText (con_prefix.pers1Plural (CONJ_PREFIX));
   label_first_singular->setText (con_prefix.pers1Singular (CONJ_PREFIX));
   label_second_singular->setText (con_prefix.pers2Singular (CONJ_PREFIX));
   label_second_plural->setText (con_prefix.pers2Plural (CONJ_PREFIX));
   label_thirdF_plural->setText (con_prefix.pers3FemalePlural (CONJ_PREFIX));
   label_thirdF_singular->setText (con_prefix.pers3FemaleSingular (CONJ_PREFIX));
   label_thirdM_singular->setText (con_prefix.pers3MaleSingular (CONJ_PREFIX));
   label_thirdN_singular->setText (con_prefix.pers3NaturalSingular (CONJ_PREFIX));
   label_thirdN_plural->setText (con_prefix.pers3NaturalPlural (CONJ_PREFIX));
   label_thirdM_plural->setText (con_prefix.pers3MalePlural (CONJ_PREFIX));
*/

   tenselabel->setBuddy(tensebox);

   pers1_label->setBuddy(first_singular);
   pers2_label->setBuddy(second_singular);
   pers3_label->setBuddy(thirdF_singular);

   female_label->setBuddy(thirdF_singular);
   male_label->setBuddy(thirdM_singular);
   natural_label->setBuddy(thirdN_singular);

   if (font != 0 && font->specfont) {
     QFont specfont = font->limitedFont();
     first_plural->setFont (specfont);
     first_singular->setFont (specfont);
     second_singular->setFont (specfont);
     second_plural->setFont (specfont);
     thirdF_plural->setFont (specfont);
     thirdF_singular->setFont (specfont);
     thirdM_singular->setFont (specfont);
     thirdN_singular->setFont (specfont);
     thirdN_plural->setFont (specfont);
     thirdM_plural->setFont (specfont);
   }
   updateFields();
}


void TenseEntryPage::initFocus() const
{
  first_singular->setFocus();
}


void TenseEntryPage::firstPluralChanged(const QString& s)
{
   conjugations.setPers1Plural (selection, s);
   updateFields();
}


void TenseEntryPage::firstSingularChanged(const QString& s)
{
   conjugations.setPers1Singular (selection, s);
   updateFields();
}


void TenseEntryPage::secondSingularChanged(const QString& s)
{
   conjugations.setPers2Singular (selection, s);
   updateFields();
}


void TenseEntryPage::secondPluralChanged(const QString& s)
{
   conjugations.setPers2Plural (selection, s);
   updateFields();
}


void TenseEntryPage::thirdFPluralChanged(const QString& s)
{
   conjugations.setPers3FemalePlural (selection, s);
   updateFields();
}


void TenseEntryPage::thirdFSingularChanged(const QString& s)
{
   conjugations.setPers3FemaleSingular (selection, s);
   updateFields();
}


void TenseEntryPage::thirdMSingularChanged(const QString& s)
{
   conjugations.setPers3MaleSingular (selection, s);
   updateFields();
}


void TenseEntryPage::thirdNSingularChanged(const QString& s)
{
   conjugations.setPers3NaturalSingular(selection, s);
   updateFields();
}


void TenseEntryPage::thirdNPluralChanged(const QString& s)
{
   conjugations.setPers3NaturalPlural (selection, s);
   updateFields();
}


void TenseEntryPage::thirdMPluralChanged(const QString& s)
{
   conjugations.setPers3MalePlural (selection, s);
   updateFields();
}


void TenseEntryPage::slotTenseSelected(int sel)
{
   if (multi_mode) {
     b_next->setEnabled(false);
     third_s_common->setEnabled(false);
     third_p_common->setEnabled(false);
     first_plural->setEnabled(false);
     first_singular->setEnabled(false);
     second_singular->setEnabled(false);
     second_plural->setEnabled(false);
     thirdF_plural->setEnabled(false);
     thirdF_singular->setEnabled(false);
     thirdM_singular->setEnabled(false);
     thirdN_singular->setEnabled(false);
     thirdN_plural->setEnabled(false);
     thirdM_plural->setEnabled(false);
   }
   else {
     selection = Conjugation::getAbbrev(sel);
     first_plural->setText (conjugations.pers1Plural (selection ));
     first_singular->setText (conjugations.pers1Singular (selection ));
     second_plural->setText (conjugations.pers2Plural (selection ));
     second_singular->setText (conjugations.pers2Singular (selection ));
     thirdF_plural->setText (conjugations.pers3FemalePlural (selection ));
     thirdF_singular->setText (conjugations.pers3FemaleSingular (selection ));
     thirdM_plural->setText (conjugations.pers3MalePlural (selection));
     thirdM_singular->setText (conjugations.pers3MaleSingular (selection ));
     thirdN_plural->setText (conjugations.pers3NaturalPlural (selection)); 
     thirdN_singular->setText (conjugations.pers3NaturalSingular (selection ));
  
     bool common = conjugations.pers3SingularCommon(selection);
     third_s_common->setChecked(common);
     thirdM_singular->setEnabled(!common);
     thirdN_singular->setEnabled(!common);
  
     common = conjugations.pers3PluralCommon(selection);
     third_p_common->setChecked(common);
     thirdN_plural->setEnabled(!common);
     thirdM_plural->setEnabled(!common);
   }
}


void TenseEntryPage::slotThirdSCommonToggled(bool common)
{
  conjugations.setPers3SingularCommon(selection, common);
  thirdM_singular->setEnabled(!common);
  thirdN_singular->setEnabled(!common);
}


void TenseEntryPage::slotThirdPCommonToggled(bool common)
{
  conjugations.setPers3PluralCommon(selection, common);
  thirdN_plural->setEnabled(!common);
  thirdM_plural->setEnabled(!common);
}


void TenseEntryPage::slotNextConj()
{
   int j;
   for (int i = tensebox->currentItem()+1; i < tensebox->count(); i++) {

     for (j = 0; j < conjugations.numEntries(); j++ ) {
       if (Conjugation::getAbbrev(i) == conjugations.getType(j)) {
         tensebox->setCurrentItem (i);
         slotTenseSelected(i);
         return;
       }
     }
   }

   for (int i = 0; i < tensebox->currentItem()-1; i++) {
     for (j = 0; j < conjugations.numEntries(); j++ ) {
       if (Conjugation::getAbbrev(i) == conjugations.getType(j)) {
         tensebox->setCurrentItem (i);
         slotTenseSelected(i);
         return;
       }
     }
   }
}


Conjugation TenseEntryPage::getConjugation()
{
  conjugations.cleanUp();
  return conjugations;
}


void TenseEntryPage::updateFields()
{
   b_next->setEnabled(conjugations.numEntries() > 1); // next button
}


void TenseEntryPage::keyPressEvent( QKeyEvent *e )
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
