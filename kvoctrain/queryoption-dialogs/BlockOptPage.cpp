/***************************************************************************

    $Id$

                  blocking options page class

    -----------------------------------------------------------------------

    begin                : Sat Oct 30 13:29:00 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/10/17 21:41:16  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.3  2001/10/13 21:11:02  arnold
    tested and fixed changes from previous cvs update

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:46:01  arnold
    import of version 0.7.0pre8 to kde-edu


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

#include <kapp.h>
#include <kmessagebox.h>

#include <qkeycode.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qcheckbox.h>

#ifndef i18n_noop
# define i18n_noop(x) (x)
#endif


struct ListRef {
          const char *text;
          long int num;
       };

static ListRef date_itemlist [] =
      { {i18n_noop("don`t care"),    0},

        {i18n_noop("30 min"),        30*60   },
        {i18n_noop("1 hour"),    1 * 60*60   },
        {i18n_noop("2 hours"),   2 * 60*60   },
        {i18n_noop("4 hours"),   4 * 60*60   },
        {i18n_noop("8 hours"),   8 * 60*60   },
        {i18n_noop("12 hours"), 12 * 60*60   },
        {i18n_noop("18 hours"), 18 * 60*60   },

        {i18n_noop("1 day"),    1 * 60*60*24},
        {i18n_noop("2 days"),   2 * 60*60*24},
        {i18n_noop("3 days"),   3 * 60*60*24},
        {i18n_noop("4 days"),   4 * 60*60*24},
        {i18n_noop("5 days"),   5 * 60*60*24},
        {i18n_noop("6 days"),   6 * 60*60*24},

        {i18n_noop("1 week"),   1 * 60*60*24*7},
        {i18n_noop("2 weeks"),  2 * 60*60*24*7},
        {i18n_noop("3 weeks"),  3 * 60*60*24*7},
        {i18n_noop("4 weeks"),  4 * 60*60*24*7},

        {i18n_noop("1 month"),  1 * 60*60*24*7*30},
        {i18n_noop("2 months"), 2 * 60*60*24*7*30},
        {i18n_noop("3 months"), 3 * 60*60*24*7*30},
        {i18n_noop("4 months"), 4 * 60*60*24*7*30},
        {i18n_noop("5 months"), 5 * 60*60*24*7*30},
        {i18n_noop("6 months"), 6 * 60*60*24*7*30},
        {i18n_noop("10 months"), 10 * 60*60*24*7*30},
        {i18n_noop("12 months"), 12 * 60*60*24*7*30},
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
   connect( c_expire, SIGNAL(toggled(bool)), SLOT(slotCheckExpire(bool)) );
   connect( c_blocking, SIGNAL(toggled(bool)), SLOT(slotCheckBlock(bool)) );
 
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
   c_blocking->setChecked(block);
   c_expire->setChecked(expire);
                                  
   setBlockBox (manager->blockItem(KV_LEV1_GRADE), block1);
   setBlockBox (manager->blockItem(KV_LEV2_GRADE), block2);
   setBlockBox (manager->blockItem(KV_LEV3_GRADE), block3);
   setBlockBox (manager->blockItem(KV_LEV4_GRADE), block4);
   setBlockBox (manager->blockItem(KV_LEV5_GRADE), block5);
   setBlockBox (manager->blockItem(KV_LEV6_GRADE), block6);
   setBlockBox (manager->blockItem(KV_LEV7_GRADE), block7);

   setExpireBox (manager->expireItem(KV_LEV1_GRADE), expire1);
   setExpireBox (manager->expireItem(KV_LEV2_GRADE), expire2);
   setExpireBox (manager->expireItem(KV_LEV3_GRADE), expire3);
   setExpireBox (manager->expireItem(KV_LEV4_GRADE), expire4);
   setExpireBox (manager->expireItem(KV_LEV5_GRADE), expire5);
   setExpireBox (manager->expireItem(KV_LEV6_GRADE), expire6);
   setExpireBox (manager->expireItem(KV_LEV7_GRADE), expire7);

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

   emit blockExpire (c_blocking->isChecked(),
                     c_expire->isChecked() );
}


void BlockOptPage::initFocus() const
{
  c_blocking->setFocus();
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
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV7_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire7(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV7_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock6(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV6_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire6(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV6_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock5(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV5_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire5(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV5_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock4(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV4_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire4(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV4_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetBlock3(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV3_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire3(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV3_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock2(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV2_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire2(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV2_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetSetBlock1(int idx)
{
  manager->setBlockItem(date_itemlist[idx].num, KV_LEV1_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::slotSetExpire1(int idx)
{
  manager->setExpireItem(date_itemlist[idx].num, KV_LEV1_GRADE);
  emit modifySetting();
  checkValidity();
}


void BlockOptPage::checkValidity()
{
  QString all_msg;
  bool found = false;
  for (int i = KV_LEV2_GRADE; i <= KV_MAX_GRADE; i++) {

    if (  block
        && manager->blockItem(i-1) != 0
        && manager->blockItem(i) != 0
        && manager->blockItem(i-1) >= manager->blockItem(i)) {
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
        && manager->expireItem(i-1) != 0
        && manager->expireItem(i) != 0
        && manager->expireItem(i-1) >= manager->expireItem(i)) {
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
          && manager->expireItem(i) != 0
          && manager->blockItem(i) != 0
          && manager->blockItem(i) >= manager->expireItem(i)) {
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
          kapp->makeStdCaption(i18n("Nonsensical values")));
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

