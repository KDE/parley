/***************************************************************************

    $Id$

                   language properties dialog page

    -----------------------------------------------------------------------

    begin                : Wed Oct 13 18:37:13 1999
                                           
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


#define Inherited LangPropPageData

#include <langset.h>
#include <kvoctraindoc.h>
#include <GrammerManager.h>

#include "LangPropPage.h"

#include <kapp.h>

#include <qkeycode.h>

struct CharSetRef {
          const char           *cs_str;
          const char           *expl;
          const QFont::CharSet  cs;
       };

#ifndef i18n_noop
# define i18n_noop(x) x
#endif

///////////////////////////////////////////////////////////////
// also update kvoctraindoc.cpp kvoctrainDoc::charSet2String
///////////////////////////////////////////////////////////////

static CharSetRef charset_list [] =
      {
        {i18n_noop("Any"), 0, QFont::AnyCharSet},
        {KES_8859_1, i18n_noop("(West Europe)"),                  QFont::ISO_8859_1},
        {KES_8859_2, i18n_noop("(East Europe, less common)"),     QFont::ISO_8859_2},
        {KES_8859_3, i18n_noop("(South Europe, less common)"),    QFont::ISO_8859_3},
        {KES_8859_4, i18n_noop("(North Europe, less common)"),    QFont::ISO_8859_4},
        {KES_8859_5, i18n_noop("(Cyrillic)"),                     QFont::ISO_8859_5},
        {KES_8859_6, i18n_noop("(Arabic)"),                       QFont::ISO_8859_6},
        {KES_8859_7, i18n_noop("(Greek)"),                        QFont::ISO_8859_7},
        {KES_8859_8, i18n_noop("(Hebrew)"),                       QFont::ISO_8859_8},
        {KES_8859_9, i18n_noop("(Western Europe, less common)"),  QFont::ISO_8859_9},
        {KES_KOI8_R, i18n_noop("(Cyrillic, RFC1489)"),            QFont::KOI8R},

        {KES_8859_10,   0,                                        QFont::ISO_8859_10},
        {KES_8859_11,   0,                                        QFont::ISO_8859_11},
        {KES_8859_12,   0,                                        QFont::ISO_8859_12},
        {KES_8859_13,   0,                                        QFont::ISO_8859_13},
        {KES_8859_14,   0,                                        QFont::ISO_8859_14},
        {KES_8859_15,   0,                                        QFont::ISO_8859_15},
        {KES_SET_JA,    0,                                        QFont::Set_Ja},
        {KES_SET_KO,    0,                                        QFont::Set_Ko},
        {KES_SET_TH_TH, 0,                                        QFont::Set_Th_TH},
        {KES_SET_ZH,    0,                                        QFont::Set_Zh},
        {KES_SET_ZH_TW, 0,                                        QFont::Set_Zh_TW},
        {KES_UNICODE,   i18n_noop("(independent)"),               QFont::Unicode},
        {KES_SET_BIG5,  0,                                        QFont::Set_Big5},

        {0, 0,            (QFont::CharSet) 0}
      };

#undef i18n_noop



LangPropPage::LangPropPage
(
        SpecFont_t        *font,
        kvoctrainDoc      *_doc,
        QString            curr_lang,
        const Conjugation &conjug,
        const Article     &art,
        const              QFont::CharSet _cs,
        QWidget           *parent,
        const char        *name
)
	:
	Inherited( parent, name ),
        doc(_doc),
        conjugations(conjug),
        articles(art),
        charset (_cs)
{
   connect( indef_female, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( def_female, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( def_male, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( indef_male, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( def_natural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( indef_natural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( first_singular, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( first_plural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( second_singular, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( second_plural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdF_singular, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdF_plural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdM_singular, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdM_plural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdN_singular, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( thirdN_plural, SIGNAL(returnPressed()), SLOT(accept()) );
   connect( charset_box, SIGNAL(highlighted(int)), SLOT(charsetChanged(int)) );

   connect( indef_female, SIGNAL(textChanged(const QString&)), SLOT(indefFemaleChanged(const QString&)) );
   connect( def_female, SIGNAL(textChanged(const QString&)), SLOT(defFemaleChanged(const QString&)) );
   connect( def_male, SIGNAL(textChanged(const QString&)), SLOT(defMaleChanged(const QString&)) );
   connect( indef_male, SIGNAL(textChanged(const QString&)), SLOT(indefMaleChanged(const QString&)) );
   connect( def_natural, SIGNAL(textChanged(const QString&)), SLOT(defNaturalChanged(const QString&)) );
   connect( indef_natural, SIGNAL(textChanged(const QString&)), SLOT(indefNaturalChanged(const QString&)) );
   connect( first_singular, SIGNAL(textChanged(const QString&)), SLOT(firstSingularChanged(const QString&)) );
   connect( first_plural, SIGNAL(textChanged(const QString&)), SLOT(firstPluralChanged(const QString&)) );
   connect( second_singular, SIGNAL(textChanged(const QString&)), SLOT(secondSingularChanged(const QString&)) );
   connect( second_plural, SIGNAL(textChanged(const QString&)), SLOT(secondPluralChanged(const QString&)) );
   connect( thirdF_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdFSingularChanged(const QString&)) );
   connect( thirdF_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdFPluralChanged(const QString&)) );
   connect( thirdM_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdMSingularChanged(const QString&)) );
   connect( thirdM_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdMPluralChanged(const QString&)) );
   connect( thirdN_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdNSingularChanged(const QString&)) );
   connect( thirdN_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdNPluralChanged(const QString&)) );

   connect( thirdS_common, SIGNAL(toggled(bool)), SLOT(slotThirdSCommonToggled(bool)) );
   connect( thirdP_common, SIGNAL(toggled(bool)), SLOT(slotThirdPCommonToggled(bool)) );

   setCaption(i18n("language properties" ));
   conjugations = conjug;

   first_plural->setText (conjugations.pers1Plural (CONJ_PREFIX));
   first_singular->setText (conjugations.pers1Singular (CONJ_PREFIX));
   second_singular->setText (conjugations.pers2Singular (CONJ_PREFIX));
   second_plural->setText (conjugations.pers2Plural (CONJ_PREFIX));
   thirdF_plural->setText (conjugations.pers3FemalePlural (CONJ_PREFIX));
   thirdF_singular->setText (conjugations.pers3FemaleSingular (CONJ_PREFIX));
   thirdN_plural->setText (conjugations.pers3NaturalPlural (CONJ_PREFIX));
   thirdN_singular->setText (conjugations.pers3NaturalSingular (CONJ_PREFIX));
   thirdM_plural->setText (conjugations.pers3MalePlural (CONJ_PREFIX));
   thirdM_singular->setText (conjugations.pers3MaleSingular (CONJ_PREFIX));

   if (font != 0 && font->specfont) {
     QFont specfont = font->limitedFont();
     first_plural->setFont (specfont);
     first_singular->setFont (specfont);
     second_singular->setFont (specfont);
     second_plural->setFont (specfont);
     thirdF_plural->setFont (specfont);
     thirdF_singular->setFont (specfont);
     thirdN_plural->setFont (specfont);
     thirdN_singular->setFont (specfont);
     thirdM_plural->setFont (specfont);
     thirdM_singular->setFont (specfont);

     def_female->setFont (specfont);
     indef_female->setFont (specfont);
     def_male->setFont (specfont);
     indef_male->setFont (specfont);
     def_natural->setFont (specfont);
     indef_natural->setFont (specfont);
   }

   bool common = conjugations.pers3SingularCommon(CONJ_PREFIX);
   thirdS_common->setChecked(common);
   thirdM_singular->setEnabled(!common);
   thirdN_singular->setEnabled(!common);

   common = conjugations.pers3PluralCommon(CONJ_PREFIX);
   thirdP_common->setChecked(common);
   thirdN_plural->setEnabled(!common);
   thirdM_plural->setEnabled(!common);

   female_label->setBuddy (def_female);
   male_label->setBuddy (def_male);
   natural_label->setBuddy (def_natural);

   female_c_label->setBuddy (thirdF_singular);
   male_c_label->setBuddy (thirdM_singular);
   natural_c_label->setBuddy (thirdN_singular);

   pers1_label->setBuddy(first_singular);
   pers2_label->setBuddy(second_singular);
   pers3_label->setBuddy(thirdF_singular);

   QString def, indef;
   articles.female(def, indef);
   def_female->setText (def);
   indef_female->setText (indef);

   articles.male(def, indef);
   def_male->setText (def);
   indef_male->setText (indef);

   articles.natural(def, indef);
   def_natural->setText (def);
   indef_natural->setText (indef);

   CharSetRef *ref = &charset_list[0];
   int index = 0;
   charset_box->clear();
   while (ref->cs_str != 0 ) {
     QString txt = i18n(ref->cs_str);
     if (ref->expl != 0) {
       txt += " ";
       txt += i18n(ref->expl);
     }
     charset_box->insertItem (txt);
     if (charset == ref->cs)
       index = ref - charset_list;
     ref++;
   }
   charset_box->setCurrentItem (index);
   charsetChanged (index);
   charset_label->setBuddy (charset_box);
}


void LangPropPage::initFocus() const
{
  def_female->setFocus();
}


Conjugation LangPropPage::getConjugation()
{
  conjugations.cleanUp();
  return conjugations;
}


void LangPropPage::firstPluralChanged(const QString& s)
{
   conjugations.setPers1Plural (CONJ_PREFIX, s);
}


void LangPropPage::firstSingularChanged(const QString& s)
{
   conjugations.setPers1Singular (CONJ_PREFIX, s);
}


void LangPropPage::secondSingularChanged(const QString& s)
{
   conjugations.setPers2Singular (CONJ_PREFIX, s);
}


void LangPropPage::secondPluralChanged(const QString& s)
{
   conjugations.setPers2Plural (CONJ_PREFIX, s);
}


void LangPropPage::thirdFPluralChanged(const QString& s)
{
   conjugations.setPers3FemalePlural (CONJ_PREFIX, s);
}


void LangPropPage::thirdFSingularChanged(const QString& s)
{
   conjugations.setPers3FemaleSingular (CONJ_PREFIX, s);
}


void LangPropPage::thirdMSingularChanged(const QString& s)
{
   conjugations.setPers3MaleSingular (CONJ_PREFIX, s);
}


void LangPropPage::thirdNSingularChanged(const QString& s)
{
   conjugations.setPers3NaturalSingular(CONJ_PREFIX, s);
}


void LangPropPage::thirdNPluralChanged(const QString& s)
{
   conjugations.setPers3NaturalPlural (CONJ_PREFIX, s);
}


void LangPropPage::thirdMPluralChanged(const QString& s)
{
   conjugations.setPers3MalePlural (CONJ_PREFIX, s);
}


void LangPropPage::slotThirdSCommonToggled(bool common)
{
  conjugations.setPers3SingularCommon(CONJ_PREFIX, common);
  thirdN_singular->setEnabled(!common);
  thirdM_singular->setEnabled(!common);
}


void LangPropPage::slotThirdPCommonToggled(bool common)
{
  conjugations.setPers3PluralCommon(CONJ_PREFIX, common);
  thirdN_plural->setEnabled(!common);
  thirdM_plural->setEnabled(!common);
}


void LangPropPage::defFemaleChanged(const QString& s)
{
   QString def, indef;
   articles.female(def, indef);
   articles.setFemale(s, indef);
}


void LangPropPage::indefFemaleChanged(const QString& s)
{
   QString def, indef;
   articles.female(def, indef);
   articles.setFemale(def, s);
}


void LangPropPage::defMaleChanged(const QString& s)
{
   QString def, indef;
   articles.male(def, indef);
   articles.setMale(s, indef);
}


void LangPropPage::indefMaleChanged(const QString& s)
{
   QString def, indef;
   articles.male(def, indef);
   articles.setMale(def, s);
}


void LangPropPage::defNaturalChanged(const QString& s)
{
   QString def, indef;
   articles.natural(def, indef);
   articles.setNatural(s, indef);
}


void LangPropPage::indefNaturalChanged(const QString& s)
{
   QString def, indef;
   articles.natural(def, indef);
   articles.setNatural(def, s);
}


void LangPropPage::charsetChanged(int idx)
{
   charset = charset_list[idx].cs;
}


void LangPropPage::keyPressEvent( QKeyEvent *e )
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
