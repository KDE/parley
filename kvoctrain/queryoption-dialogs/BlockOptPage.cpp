/***************************************************************************

    $Id$

                  blocking options page class

    -----------------------------------------------------------------------

    begin                : Sat Oct 30 13:29:00 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "BlockOptPage.h"

#include <QueryManager.h>
#include <prefs.h>

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <qcombobox.h>
#include <qlabel.h>
#include <qcheckbox.h>

struct ListRef {
          const char *text;
          long int num;
       };

static ListRef date_itemlist [] =
      { {I18N_NOOP("Don`t Care"),    0},

        {I18N_NOOP("30 Min"),        30*60   },
        {I18N_NOOP("1 Hour"),    1 * 60*60   },
        {I18N_NOOP("2 Hours"),   2 * 60*60   },
        {I18N_NOOP("4 Hours"),   4 * 60*60   },
        {I18N_NOOP("8 Hours"),   8 * 60*60   },
        {I18N_NOOP("12 Hours"), 12 * 60*60   },
        {I18N_NOOP("18 Hours"), 18 * 60*60   },

        {I18N_NOOP("1 Day"),    1 * 60*60*24},
        {I18N_NOOP("2 Days"),   2 * 60*60*24},
        {I18N_NOOP("3 Days"),   3 * 60*60*24},
        {I18N_NOOP("4 Days"),   4 * 60*60*24},
        {I18N_NOOP("5 Days"),   5 * 60*60*24},
        {I18N_NOOP("6 Days"),   6 * 60*60*24},

        {I18N_NOOP("1 Week"),   1 * 60*60*24*7},
        {I18N_NOOP("2 Weeks"),  2 * 60*60*24*7},
        {I18N_NOOP("3 Weeks"),  3 * 60*60*24*7},
        {I18N_NOOP("4 Weeks"),  4 * 60*60*24*7},

        {I18N_NOOP("1 Month"),  1 * 60*60*24*30},
        {I18N_NOOP("2 Months"), 2 * 60*60*24*30},
        {I18N_NOOP("3 Months"), 3 * 60*60*24*30},
        {I18N_NOOP("4 Months"), 4 * 60*60*24*30},
        {I18N_NOOP("5 Months"), 5 * 60*60*24*30},
        {I18N_NOOP("6 Months"), 6 * 60*60*24*30},
        {I18N_NOOP("10 Months"), 10 * 60*60*24*30},
        {I18N_NOOP("12 Months"), 12 * 60*60*24*30},
        {0 ,  0}
      };


BlockOptPage::BlockOptPage
(
        QueryManager *_manager,
        bool          _block,
        bool          _expire,
        QWidget      *parent,
        const char   *name
)
	:
	BlockOptPageForm( parent, name )
{

   connect( expire1, SIGNAL(activated(int)), SLOT(slotSetExpire1(int)) );
   connect( block1, SIGNAL(activated(int)), SLOT(slotSetSetBlock1(int)) );
   connect( expire2, SIGNAL(activated(int)), SLOT(slotSetExpire2(int)) );
   connect( block2, SIGNAL(activated(int)), SLOT(slotSetSetBlock2(int)) );
   connect( expire3, SIGNAL(activated(int)), SLOT(slotSetExpire3(int)) );
   connect( block3, SIGNAL(activated(int)), SLOT(slotSetBlock3(int)) );
   connect( expire4, SIGNAL(activated(int)), SLOT(slotSetExpire4(int)) );
   connect( block4, SIGNAL(activated(int)), SLOT(slotSetSetBlock4(int)) );
   connect( expire5, SIGNAL(activated(int)), SLOT(slotSetExpire5(int)) );
   connect( block5, SIGNAL(activated(int)), SLOT(slotSetSetBlock5(int)) );
   connect( expire6, SIGNAL(activated(int)), SLOT(slotSetExpire6(int)) );
   connect( block6, SIGNAL(activated(int)), SLOT(slotSetSetBlock6(int)) );
   connect( expire7, SIGNAL(activated(int)), SLOT(slotSetExpire7(int)) );
   connect( block7, SIGNAL(activated(int)), SLOT(slotSetBlock7(int)) );
   connect( kcfg_expire, SIGNAL(toggled(bool)), SLOT(slotCheckExpire(bool)) );
   connect( kcfg_block, SIGNAL(toggled(bool)), SLOT(slotCheckBlock(bool)) );

   l_lev7->setBuddy(block7);
   l_lev6->setBuddy(block6);
   l_lev5->setBuddy(block5);
   l_lev4->setBuddy(block4);
   l_lev3->setBuddy(block3);
   l_lev2->setBuddy(block2);
   l_lev1->setBuddy(block1);

   setStates (_manager, _block, _expire);
   checkValidity();
}


void BlockOptPage::setStates(QueryManager *_manager, bool _block, bool _expire)
{
   manager = _manager;
   block = _block;
   expire = _expire;
   kcfg_block->setChecked(block);
   kcfg_expire->setChecked(expire);

   setBlockBox (Prefs::blockItem(KV_LEV1_GRADE), block1);
   setBlockBox (Prefs::blockItem(KV_LEV2_GRADE), block2);
   setBlockBox (Prefs::blockItem(KV_LEV3_GRADE), block3);
   setBlockBox (Prefs::blockItem(KV_LEV4_GRADE), block4);
   setBlockBox (Prefs::blockItem(KV_LEV5_GRADE), block5);
   setBlockBox (Prefs::blockItem(KV_LEV6_GRADE), block6);
   setBlockBox (Prefs::blockItem(KV_LEV7_GRADE), block7);

   setExpireBox (Prefs::expireItem(KV_LEV1_GRADE), expire1);
   setExpireBox (Prefs::expireItem(KV_LEV2_GRADE), expire2);
   setExpireBox (Prefs::expireItem(KV_LEV3_GRADE), expire3);
   setExpireBox (Prefs::expireItem(KV_LEV4_GRADE), expire4);
   setExpireBox (Prefs::expireItem(KV_LEV5_GRADE), expire5);
   setExpireBox (Prefs::expireItem(KV_LEV6_GRADE), expire6);
   setExpireBox (Prefs::expireItem(KV_LEV7_GRADE), expire7);

   block1->setEnabled(block);
   block2->setEnabled(block);
   block3->setEnabled(block);
   block4->setEnabled(block);
   block5->setEnabled(block);
   block6->setEnabled(block);
   block7->setEnabled(block);

   expire1->setEnabled(expire);
   expire2->setEnabled(expire);
   expire3->setEnabled(expire);
   expire4->setEnabled(expire);
   expire5->setEnabled(expire);
   expire6->setEnabled(expire);
   expire7->setEnabled(expire);

   emit blockExpire (kcfg_block->isChecked(),
                     kcfg_expire->isChecked() );
}


void BlockOptPage::initFocus() const
{
  kcfg_block->setFocus();
}


void BlockOptPage::setExpireBox(int no, QComboBox *box)
{
   ListRef *ref = date_itemlist;
   int index = 0;

   box->clear();
   while (ref->text != 0 ) {
     box->insertItem (i18n(ref->text));
     if (no == ref->num)
       index = ref - date_itemlist;
     ref++;
   }
   box->setCurrentItem (index);
}


void BlockOptPage::setBlockBox(int no, QComboBox *box)
{
   ListRef *ref = date_itemlist;
   int index = 0;

   box->clear();
   while (ref->text != 0 ) {
     box->insertItem (i18n(ref->text));
     if (no == ref->num)
       index = ref - date_itemlist;
     ref++;
   }
   box->setCurrentItem (index);
}


void BlockOptPage::slotCheckBlock(bool state)
{
  block = state;
  setStates (manager, block, expire);
  emit modifySetting();
//  checkValidity();
}


void BlockOptPage::slotCheckExpire(bool state)
{
  expire = state;
  setStates (manager, block, expire);
  emit modifySetting();
//  checkValidity();
}


void BlockOptPage::slotSetBlock7(int idx)
{
  Prefs::setBlockItem(KV_LEV7_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire7(int idx)
{
  Prefs::setExpireItem(KV_LEV7_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock6(int idx)
{
  Prefs::setBlockItem(KV_LEV6_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire6(int idx)
{
  Prefs::setExpireItem(KV_LEV6_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock5(int idx)
{
  Prefs::setBlockItem(KV_LEV5_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire5(int idx)
{
  Prefs::setExpireItem(KV_LEV5_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock4(int idx)
{
  Prefs::setBlockItem(KV_LEV4_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire4(int idx)
{
  Prefs::setExpireItem(KV_LEV4_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetBlock3(int idx)
{
  Prefs::setBlockItem(KV_LEV3_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire3(int idx)
{
  Prefs::setExpireItem(KV_LEV3_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock2(int idx)
{
  Prefs::setBlockItem(KV_LEV2_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire2(int idx)
{
  Prefs::setExpireItem(KV_LEV2_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock1(int idx)
{
  Prefs::setBlockItem(KV_LEV1_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire1(int idx)
{
  Prefs::setExpireItem(KV_LEV1_GRADE, date_itemlist[idx].num);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::checkValidity()
{
  QString all_msg;
  bool found = false;
  for (int i = KV_LEV2_GRADE; i <= KV_MAX_GRADE; i++) {

    if (  block
        && Prefs::blockItem(i-1) != 0
        && Prefs::blockItem(i) != 0
        && Prefs::blockItem(i-1) >= Prefs::blockItem(i)) {
      QString format;
      if (!found)
        format = i18n("\nNonsensical blocking times.\n");
      found = true;

      format += i18n("Time for level %1 is not lower than time for level %2.\n");
      QString msg = format.arg(i-1).arg(i);
      all_msg += msg;
    }
  }

  for (int i = KV_LEV2_GRADE; i <= KV_MAX_GRADE; i++) {
    found = false;
    if (   expire
        && Prefs::expireItem(i-1) != 0
        && Prefs::expireItem(i) != 0
        && Prefs::expireItem(i-1) >= Prefs::expireItem(i)) {
      QString format;
      if (!found)
        format = i18n("\nNonsensical expiration times.\n");
      found = true;

      format += i18n("Time for level %1 is not lower than time for level %2.\n");
      QString msg = format.arg(i-1).arg(i);
      all_msg += msg;
    }
  }

  if (all_msg.length() == 0) {
    found = false;
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
      if (   block
          && expire
          && Prefs::expireItem(i) != 0
          && Prefs::blockItem(i) != 0
          && Prefs::blockItem(i) >= Prefs::expireItem(i)) {
        QString format;
        if (!found)
          format = i18n("\nNonsensical blocking vs. expiration times.\n");
        found = true;

        format += i18n("At level %1 blocking time is not lower than expiration time.\n");
        QString msg = format.arg(i);
        all_msg += msg;
      }
    }
  }


  if (all_msg.length() != 0)
    KMessageBox::sorry(this, all_msg,
          kapp->makeStdCaption(i18n("Nonsensical Values")));
}

void BlockOptPage::keyPressEvent( QKeyEvent *e )
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

#include "BlockOptPage.moc"

