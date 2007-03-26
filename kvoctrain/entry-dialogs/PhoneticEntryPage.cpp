 /***************************************************************************

              dialog page for characters from the phonetic alphabet

    -----------------------------------------------------------------------

    begin          : Sun Dec 9 2001

    copyright      : (C) 2001-2002 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001-2002 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QLayout>
#include <QFrame>
#include <QGridLayout>
#include <QKeyEvent>

#include <kdebug.h>
#include <klocale.h>

#include "PhoneticEntryPage.h"

#define KV_MAX_HORIZ  20
#define KV_FONTSIZE   14

#define I18N_NOOP_maybe(x) x

// provide Unicode chars U0250 - U02AF

struct KV_Unicode_Ref {
  wchar_t     code;
  const char *unicodename;
  const char *audible;
};

KV_Unicode_Ref kv_unicode_ref[] = {

   {0x00E6, "LATIN SMALL LETTER AE", I18N_NOOP_maybe("")},
   {0x00E7, "LATIN SMALL LETTER C WITH CEDILLA", I18N_NOOP_maybe("")},
   {0x00F0, "LATIN SMALL LETTER ETH", I18N_NOOP_maybe("")},
   {0x00F8, "LATIN SMALL LETTER O WITH STROKE", I18N_NOOP_maybe("")},
   {0x0127, "LATIN SMALL LETTER H WITH STROKE", I18N_NOOP_maybe("")},
   {0x014B, "LATIN SMALL LETTER ENG", I18N_NOOP_maybe("")},
   {0x0153, "LATIN SMALL LIGATURE OE", I18N_NOOP_maybe("")},
   {0x01F0, "LATIN SMALL LETTER J WITH CARON", I18N_NOOP_maybe("")},

   {0x03B2, "GREEK SMALL LETTER BETA", I18N_NOOP_maybe("")},
   {0x03B8, "GREEK SMALL LETTER THETA", I18N_NOOP_maybe("")},
   {0x03BB, "GREEK SMALL LETTER LAMDA", I18N_NOOP_maybe("")},
   {0x03C7, "GREEK SMALL LETTER CHI", I18N_NOOP_maybe("")},

   {0x0250, "LATIN SMALL LETTER TURNED A",     I18N_NOOP_maybe("low central unrounded vowel")},
   {0x0251, "LATIN SMALL LETTER ALPHA",     I18N_NOOP_maybe("low back unrounded vowel")},
   {0x0252, "LATIN SMALL LETTER TURNED ALPHA",     I18N_NOOP_maybe("low back rounded vowel")},
   {0x0253, "LATIN SMALL LETTER B WITH HOOK",     I18N_NOOP_maybe("implosive bilabial stop")},
   {0x0254, "LATIN SMALL LETTER OPEN O",     I18N_NOOP_maybe("lower-mid back rounded vowel")},
   {0x0255, "LATIN SMALL LETTER C WITH CURL",     I18N_NOOP_maybe("voiceless alveolo-palatal laminal fricative")},
   {0x0256, "LATIN SMALL LETTER D WITH TAIL",     I18N_NOOP_maybe("voiced retroflex stop")},
   {0x0257, "LATIN SMALL LETTER D WITH HOOK",     I18N_NOOP_maybe("implosive dental or alveolar stop")},
   {0x0258, "LATIN SMALL LETTER REVERSED E",     I18N_NOOP_maybe("upper-mid central unrounded vowel")},
   {0x0259, "LATIN SMALL LETTER SCHWA",     I18N_NOOP_maybe("mid-central unrounded vowel")},
   {0x025A, "LATIN SMALL LETTER SCHWA WITH HOOK",     I18N_NOOP_maybe("rhotacized schwa")},
   {0x025B, "LATIN SMALL LETTER OPEN E",     I18N_NOOP_maybe("lower-mid front unrounded vowel")},
   {0x025C, "LATIN SMALL LETTER REVERSED OPEN E",     I18N_NOOP_maybe("lower-mid central unrounded vowel")},
   {0x025D, "LATIN SMALL LETTER REVERSED OPEN E WITH HOOK",     I18N_NOOP_maybe("rhotacized lower-mid central vowel")},
   {0x025E, "LATIN SMALL LETTER CLOSED REVERSED OPEN E",     I18N_NOOP_maybe("lower-mid central rounded vowel")},
   {0x025F, "LATIN SMALL LETTER DOTLESS J WITH STROKE",     I18N_NOOP_maybe("voiced palatal stop")},
   {0x0260, "LATIN SMALL LETTER G WITH HOOK",     I18N_NOOP_maybe("implosive velar stop")},
   {0x0261, "LATIN SMALL LETTER SCRIPT G",     I18N_NOOP_maybe("voiced velar stop")},
   {0x0262, "LATIN LETTER SMALL CAPITAL G",     I18N_NOOP_maybe("voiced uvular stop")},
   {0x0263, "LATIN SMALL LETTER GAMMA",     I18N_NOOP_maybe("voiced velar fricative")},
   {0x0264, "LATIN SMALL LETTER RAMS HORN",     I18N_NOOP_maybe("")},
   {0x0265, "LATIN SMALL LETTER TURNED H",     I18N_NOOP_maybe("voiced rounded palatal approximant")},
   {0x0266, "LATIN SMALL LETTER H WITH HOOK",     I18N_NOOP_maybe("breathy-voiced glottal fricative")},
   {0x0267, "LATIN SMALL LETTER HENG WITH HOOK",     I18N_NOOP_maybe("voiceless coarticulated velar and palatoalveolar fricative")},
   {0x0268, "LATIN SMALL LETTER I WITH STROKE",     I18N_NOOP_maybe("high central unrounded vowel")},
   {0x0269, "LATIN SMALL LETTER IOTA",     I18N_NOOP_maybe("semi-high front unrounded vowel")},
   {0x026A, "LATIN LETTER SMALL CAPITAL I",     I18N_NOOP_maybe("semi-high front unrounded vowel")},
   {0x026B, "LATIN SMALL LETTER L WITH MIDDLE TILDE",     I18N_NOOP_maybe("velarized voiced alveolar lateral approximant")},
   {0x026C, "LATIN SMALL LETTER L WITH BELT",     I18N_NOOP_maybe("voiceless alveolar lateral fricative")},
   {0x026D, "LATIN SMALL LETTER L WITH RETROFLEX HOOK",     I18N_NOOP_maybe("voiced retroflex lateral")},
   {0x026E, "LATIN SMALL LETTER LEZH",     I18N_NOOP_maybe("voiced lateral fricative")},
   {0x026F, "LATIN SMALL LETTER TURNED M",     I18N_NOOP_maybe("high back unrounded vowel")},
   {0x0270, "LATIN SMALL LETTER TURNED M WITH LONG LEG",     I18N_NOOP_maybe("voiced velar approximant")},
   {0x0271, "LATIN SMALL LETTER M WITH HOOK",     I18N_NOOP_maybe("voiced labiodental nasal")},
   {0x0272, "LATIN SMALL LETTER N WITH LEFT HOOK",     I18N_NOOP_maybe("voiced palatal nasal")},
   {0x0273, "LATIN SMALL LETTER N WITH RETROFLEX HOOK",     I18N_NOOP_maybe("voiced retroflex nasal")},
   {0x0274, "LATIN LETTER SMALL CAPITAL N",     I18N_NOOP_maybe("voiced uvular nasal")},
   {0x0275, "LATIN SMALL LETTER BARRED O",     I18N_NOOP_maybe("rounded mid-central vowel, i.e. rounded schwa")},
   {0x0276, "LATIN LETTER SMALL CAPITAL OE",     I18N_NOOP_maybe("low front rounded vowel")},
   {0x0277, "LATIN SMALL LETTER CLOSED OMEGA",     I18N_NOOP_maybe("semi-high back rounded vowel")},
   {0x0278, "LATIN SMALL LETTER PHI",     I18N_NOOP_maybe("voiceless bilabial fricative")},
   {0x0279, "LATIN SMALL LETTER TURNED R",     I18N_NOOP_maybe("voiced alveolar approximant")},
   {0x027A, "LATIN SMALL LETTER TURNED R WITH LONG LEG",     I18N_NOOP_maybe("voiced lateral flap")},
   {0x027B, "LATIN SMALL LETTER TURNED R WITH HOOK",     I18N_NOOP_maybe("voiced retroflex approximant")},
   {0x027C, "LATIN SMALL LETTER R WITH LONG LEG",     I18N_NOOP_maybe("voiced strident apico-alveolar trill")},
   {0x027D, "LATIN SMALL LETTER R WITH TAIL",     I18N_NOOP_maybe("voiced retroflex flap")},
   {0x027E, "LATIN SMALL LETTER R WITH FISHHOOK",     I18N_NOOP_maybe("voiced alveolar flap or tap")},
   {0x027F, "LATIN SMALL LETTER REVERSED R WITH FISHHOOK",     I18N_NOOP_maybe("apical dental vowel")},
   {0x0280, "LATIN LETTER SMALL CAPITAL R",     I18N_NOOP_maybe("voiced uvular trill")},
   {0x0281, "LATIN LETTER SMALL CAPITAL INVERTED R",     I18N_NOOP_maybe("voiced uvular fricative or approximant")},
   {0x0282, "LATIN SMALL LETTER S WITH HOOK",     I18N_NOOP_maybe("voiceless retroflex fricative")},
   {0x0283, "LATIN SMALL LETTER ESH",     I18N_NOOP_maybe("voiceless postalveolar fricative")},
   {0x0284, "LATIN SMALL LETTER DOTLESS J WITH STROKE AND HOOK",     I18N_NOOP_maybe("implosive palatal stop")},
   {0x0285, "LATIN SMALL LETTER SQUAT REVERSED ESH",     I18N_NOOP_maybe("apical retroflex vowel")},
   {0x0286, "LATIN SMALL LETTER ESH WITH CURL",     I18N_NOOP_maybe("palatalized voiceless postalveolar fricative")},
   {0x0287, "LATIN SMALL LETTER TURNED T",     I18N_NOOP_maybe("dental click")},
   {0x0288, "LATIN SMALL LETTER T WITH RETROFLEX HOOK",     I18N_NOOP_maybe("voiceless retroflex stop")},
   {0x0289, "LATIN SMALL LETTER U BAR",     I18N_NOOP_maybe("high central rounded vowel")},
   {0x028A, "LATIN SMALL LETTER UPSILON",     I18N_NOOP_maybe("semi-high back rounded vowel")},
   {0x028B, "LATIN SMALL LETTER V WITH HOOK",     I18N_NOOP_maybe("voiced labiodental approximant")},
   {0x028C, "LATIN SMALL LETTER TURNED V",     I18N_NOOP_maybe("lower-mid back unrounded vowel")},
   {0x028D, "LATIN SMALL LETTER TURNED W",     I18N_NOOP_maybe("voiceless rounded labiovelar approximant")},
   {0x028E, "LATIN SMALL LETTER TURNED Y",     I18N_NOOP_maybe("voiced lateral approximant")},
   {0x028F, "LATIN LETTER SMALL CAPITAL Y",     I18N_NOOP_maybe("semi-high front rounded vowel")},
   {0x0290, "LATIN SMALL LETTER Z WITH RETROFLEX HOOK",     I18N_NOOP_maybe("voiced retroflex fricative")},
   {0x0291, "LATIN SMALL LETTER Z WITH CURL",     I18N_NOOP_maybe("voiced alveolo-palatal laminal fricative")},
   {0x0292, "LATIN SMALL LETTER EZH",     I18N_NOOP_maybe("voiced postalveolar fricative")},
   {0x0293, "LATIN SMALL LETTER EZH WITH CURL",     I18N_NOOP_maybe("palatalized voiced postalveolar fricative")},
   {0x0294, "LATIN LETTER GLOTTAL STOP",     I18N_NOOP_maybe("")},
   {0x0295, "LATIN LETTER PHARYNGEAL VOICED FRICATIVE",     I18N_NOOP_maybe("voiced pharyngeal fricative")},
   {0x0296, "LATIN LETTER INVERTED GLOTTAL STOP",     I18N_NOOP_maybe("lateral click")},
   {0x0297, "LATIN LETTER STRETCHED C",     I18N_NOOP_maybe("palatal (or alveolar) click")},
   {0x0298, "LATIN LETTER BILABIAL CLICK",     I18N_NOOP_maybe("")},
   {0x0299, "LATIN LETTER SMALL CAPITAL B",     I18N_NOOP_maybe("bilabial trill")},
   {0x029A, "LATIN SMALL LETTER CLOSED OPEN E",     I18N_NOOP_maybe("lower-mid front rounded vowel")},
   {0x029B, "LATIN LETTER SMALL CAPITAL G WITH HOOK",     I18N_NOOP_maybe("voiced uvular implosive")},
   {0x029C, "LATIN LETTER SMALL CAPITAL H",     I18N_NOOP_maybe("voiceless epiglottal fricative")},
   {0x029D, "LATIN SMALL LETTER J WITH CROSSED-TAIL",     I18N_NOOP_maybe("voiced palatal fricative")},
   {0x029E, "LATIN SMALL LETTER TURNED K",     I18N_NOOP_maybe("proposed for velar click")},
   {0x029F, "LATIN LETTER SMALL CAPITAL L",     I18N_NOOP_maybe("velar lateral approximant")},
   {0x02A0, "LATIN SMALL LETTER Q WITH HOOK",     I18N_NOOP_maybe("voiceless uvular implosive")},
   {0x02A1, "LATIN LETTER GLOTTAL STOP WITH STROKE",     I18N_NOOP_maybe("voiced epiglottal stop")},
   {0x02A2, "LATIN LETTER REVERSED GLOTTAL STOP WITH STROKE",     I18N_NOOP_maybe("voiced epiglottal fricative")},
   {0x02A3, "LATIN SMALL LETTER DZ DIGRAPH",     I18N_NOOP_maybe("voiced dental affricate")},
   {0x02A4, "LATIN SMALL LETTER DEZH DIGRAPH",     I18N_NOOP_maybe("voiced postalveolar affricate")},
   {0x02A5, "LATIN SMALL LETTER DZ DIGRAPH WITH CURL",     I18N_NOOP_maybe("voiced alveolo-palatal affricate")},
   {0x02A6, "LATIN SMALL LETTER TS DIGRAPH",     I18N_NOOP_maybe("voiceless dental affricate")},
   {0x02A7, "LATIN SMALL LETTER TESH DIGRAPH",     I18N_NOOP_maybe("voiceless postalveolar affricate")},
   {0x02A8, "LATIN SMALL LETTER TC DIGRAPH WITH CURL",     I18N_NOOP_maybe("voiceless alveolo-palatal affricate")},
   {0x02A9, "LATIN SMALL LETTER FENG DIGRAPH",     I18N_NOOP_maybe("velopharyngeal fricative")},
   {0x02AA, "LATIN SMALL LETTER LS DIGRAPH",     I18N_NOOP_maybe("lateral alveolar fricative (lisp)")},
   {0x02AB, "LATIN SMALL LETTER LZ DIGRAPH",     I18N_NOOP_maybe("voiced lateral alveolar fricative")},
   {0x02AC, "LATIN LETTER BILABIAL PERCUSSIVE",     I18N_NOOP_maybe("audible lip smack")},
   {0x02AD, "LATIN LETTER BIDENTAL PERCUSSIVE",     I18N_NOOP_maybe("audible teeth gnashing")},

   {0x02CA, "MODIFIER LETTER ACUTE ACCENT (Mandarin Chinese second tone)",     I18N_NOOP_maybe("high-rising tone")},
   {0x02CB, "MODIFIER LETTER GRAVE ACCENT (Mandarin Chinese fourth tone)",     I18N_NOOP_maybe("high-falling tone")},

   {0x2191, "UPWARDS ARROW",     I18N_NOOP_maybe("egressive airflow")},
   {0x2193, "DOWNWARDS ARROW",     I18N_NOOP_maybe("ingressive airflow")},

   {0, 0, 0}
};


void PhoneticButton::slotClicked()
{
  if (text().length() != 0)
    emit page->charSelected(text()[0].unicode());
}


PhoneticEntryPage::PhoneticEntryPage(const QFont &ipafont, QWidget *parent) : KDialog(parent)
{
  setButtons(Close);
  setDefaultButton(Close);
  setCaption(i18n("Select Characters From Phonetic Alphabet"));
  setModal(false);
  int num = sizeof(kv_unicode_ref) / sizeof(kv_unicode_ref[0]);
  QFrame * dialogFrame = new QFrame(this);
  setMainWidget(dialogFrame);
  QGridLayout *gridLayoutTop = new QGridLayout(dialogFrame);
  gridLayoutTop->setMargin(0);
  QSpacerItem *spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  gridLayoutTop->addItem(spacerItem, 1, 0, 1, 1);
  QSpacerItem *spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayoutTop->addItem(spacerItem1, 0, 1, 1, 1);

  QGridLayout *gridLayoutButtons = new QGridLayout();
  gridLayoutButtons->setMargin(0);
  KV_Unicode_Ref *uni_ref = kv_unicode_ref;
  int vert = 0;
  int horiz = 0;
  while (uni_ref->code != 0) {
    QChar qc = uni_ref->code;
    QString text(qc);
    PhoneticButton *phoneticButton = new PhoneticButton(text, dialogFrame, this);
    connect (phoneticButton, SIGNAL(clicked()), phoneticButton, SLOT(slotClicked()) );
    QString tip = i18n("Unicode name: ");
    tip += QString::fromLatin1(uni_ref->unicodename);
    tip += '\n';
    tip += i18nc("Describing the sound of the character", "Sound: ");
    tip += i18n(uni_ref->audible);
    phoneticButton->setFont(ipafont);
    phoneticButton->setSizePolicy(QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
    int sz = qMax(14, int(1.7*ipafont.pointSize()));
    phoneticButton->setMaximumSize(QSize (sz, sz));
    phoneticButton->setToolTip(tip);
    gridLayoutButtons->addWidget( phoneticButton, vert, horiz, 0 /*Qt::AlignCenter*/ );

    if (++horiz >= KV_MAX_HORIZ) {
      ++vert;
      horiz = 0;
    }
    ++uni_ref;
  }

  gridLayoutTop->addLayout(gridLayoutButtons, 0, 0, 1, 1);
}


void PhoneticEntryPage::keyPressEvent( QKeyEvent *e )
{
  ///@todo Why check the modifiers? Why use keyPressEvent at all?
  if ((e->state() & (Qt::ControlModifier | Qt::AltModifier)) == 0) {
    QString s = e->text();
    for (int i = 0; i < s.length(); ++i) {
      emit charSelected(s[i].unicode());
    }
    e->accept();
  }
  else
    e->ignore();
}


#include "PhoneticEntryPage.moc"
