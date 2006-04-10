/***************************************************************************

                      blocking options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <QComboBox>
#include <QCheckBox>

#include <klocale.h>
#include <kmessagebox.h>

#include "blockoptions.h"
#include "prefs.h"

struct ListRef
{
  const char *text;
  long int num;
};

static ListRef date_itemlist [] =
{
  {I18N_NOOP("Do not Care"),    0},

  {I18N_NOOP("30 Min"),        30*60},
  {I18N_NOOP("1 Hour"),    1 * 60*60},
  {I18N_NOOP("2 Hours"),   2 * 60*60},
  {I18N_NOOP("4 Hours"),   4 * 60*60},
  {I18N_NOOP("8 Hours"),   8 * 60*60},
  {I18N_NOOP("12 Hours"), 12 * 60*60},
  {I18N_NOOP("18 Hours"), 18 * 60*60},

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


BlockOptions::BlockOptions(QWidget* parent): QWidget(parent)
{
  setupUi(this);
  connect(expire1, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block1, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire2, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block2, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire3, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block3, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire4, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block4, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire5, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block5, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire6, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block6, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(expire7, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));
  connect(block7, SIGNAL(activated(int)), this, SLOT(slotComboBoxActivated(int)));

  connect(kcfg_Block, SIGNAL(toggled(bool)), this, SLOT(slotBlockToggled(bool)));
  connect(kcfg_Expire, SIGNAL(toggled(bool)), this, SLOT(slotExpireToggled(bool)));

  fillWidgets();
  updateWidgets();
}

void BlockOptions::fillWidgets()
{
  fillComboBox(block1);
  fillComboBox(block2);
  fillComboBox(block3);
  fillComboBox(block4);
  fillComboBox(block5);
  fillComboBox(block6);
  fillComboBox(block7);

  m_blockComboList.append(block1);
  m_blockComboList.append(block2);
  m_blockComboList.append(block3);
  m_blockComboList.append(block4);
  m_blockComboList.append(block5);
  m_blockComboList.append(block6);
  m_blockComboList.append(block7);

  fillComboBox(expire1);
  fillComboBox(expire2);
  fillComboBox(expire3);
  fillComboBox(expire4);
  fillComboBox(expire5);
  fillComboBox(expire6);
  fillComboBox(expire7);

  m_expireComboList.append(expire1);
  m_expireComboList.append(expire2);
  m_expireComboList.append(expire3);
  m_expireComboList.append(expire4);
  m_expireComboList.append(expire5);
  m_expireComboList.append(expire6);
  m_expireComboList.append(expire7);
}

void BlockOptions::updateWidgets()
{
  updateComboBox(Prefs::blockItem(KV_LEV1_GRADE), block1);
  updateComboBox(Prefs::blockItem(KV_LEV2_GRADE), block2);
  updateComboBox(Prefs::blockItem(KV_LEV3_GRADE), block3);
  updateComboBox(Prefs::blockItem(KV_LEV4_GRADE), block4);
  updateComboBox(Prefs::blockItem(KV_LEV5_GRADE), block5);
  updateComboBox(Prefs::blockItem(KV_LEV6_GRADE), block6);
  updateComboBox(Prefs::blockItem(KV_LEV7_GRADE), block7);

  updateComboBox(Prefs::expireItem(KV_LEV1_GRADE), expire1);
  updateComboBox(Prefs::expireItem(KV_LEV2_GRADE), expire2);
  updateComboBox(Prefs::expireItem(KV_LEV3_GRADE), expire3);
  updateComboBox(Prefs::expireItem(KV_LEV4_GRADE), expire4);
  updateComboBox(Prefs::expireItem(KV_LEV5_GRADE), expire5);
  updateComboBox(Prefs::expireItem(KV_LEV6_GRADE), expire6);
  updateComboBox(Prefs::expireItem(KV_LEV7_GRADE), expire7);

  bool block = Prefs::block();
  kcfg_Block->setChecked(block);
  block1->setEnabled(block);
  block2->setEnabled(block);
  block3->setEnabled(block);
  block4->setEnabled(block);
  block5->setEnabled(block);
  block6->setEnabled(block);
  block7->setEnabled(block);

  bool expire = Prefs::expire();
  kcfg_Expire->setChecked(expire);
  expire1->setEnabled(expire);
  expire2->setEnabled(expire);
  expire3->setEnabled(expire);
  expire4->setEnabled(expire);
  expire5->setEnabled(expire);
  expire6->setEnabled(expire);
  expire7->setEnabled(expire);

  emit blockExpireChanged(block, expire);
}

void BlockOptions::fillComboBox(QComboBox * cb)
{
  ListRef *ref = date_itemlist;

  cb->clear();
  while (ref->text != 0)
  {
    cb->insertItem (i18n(ref->text));
    ref++;
  }
}

void BlockOptions::updateComboBox(int value, QComboBox * cb)
{
  ListRef *ref = date_itemlist;
  int index = 0;

  while (ref->text != 0)
  {
    if (value == ref->num)
      index = ref - date_itemlist;
    ref++;
  }
  cb->setCurrentItem(index);
}

void BlockOptions::slotBlockToggled(bool state)
{
  block1->setEnabled(state);
  block2->setEnabled(state);
  block3->setEnabled(state);
  block4->setEnabled(state);
  block5->setEnabled(state);
  block6->setEnabled(state);
  block7->setEnabled(state);
  emit blockExpireChanged(state, kcfg_Expire->isChecked());
}

void BlockOptions::slotExpireToggled(bool state)
{
  expire1->setEnabled(state);
  expire2->setEnabled(state);
  expire3->setEnabled(state);
  expire4->setEnabled(state);
  expire5->setEnabled(state);
  expire6->setEnabled(state);
  expire7->setEnabled(state);
  emit blockExpireChanged(kcfg_Block->isChecked(), state);
}

void BlockOptions::slotComboBoxActivated(int)
{
  emit widgetModified();
  checkValidity();
}


void BlockOptions::checkValidity()
{
  QString message;
  bool found = false;
  for (int i = 1; i <= 6; i++)
  {
    if (kcfg_Block->isChecked())
    {
      /*if(Prefs::blockItem(i-1) != 0  &&
        Prefs::blockItem(i) != 0 &&
        Prefs::blockItem(i-1) >= Prefs::blockItem(i))*/
      if (date_itemlist[m_blockComboList[i - 1]->currentIndex()].num != 0 &&
          date_itemlist[m_blockComboList[i]->currentIndex()].num != 0 &&
          date_itemlist[m_blockComboList[i - 1]->currentIndex()].num >= date_itemlist[m_blockComboList[i]->currentIndex()].num)
      {
        QString format;
        if (!found)
          message.append(i18n("Illogical blocking times.\n"));
        found = true;
        message.append(i18n("The time for level %1 should be lower than the time for level %2.\n", i, i + 1));
      }
    }
  }

  for (int i = 1; i <= 6; i++)
  {
    found = false;
    if (kcfg_Expire->isChecked())
    {
      if (date_itemlist[m_expireComboList[i - 1]->currentIndex()].num != 0 &&
          date_itemlist[m_expireComboList[i]->currentIndex()].num != 0 &&
          date_itemlist[m_expireComboList[i - 1]->currentIndex()].num >=
            date_itemlist[m_expireComboList[i]->currentIndex()].num)
      {
        QString format;
        if (!found)
          message.append(i18n("\nIllogical expiration times.\n"));
        found = true;
        message.append(i18n("The time for level %1 should be lower than the time for level %2.\n", i, i + 1));
      }
      }
  }

  if (message.length() == 0)
  {
    found = false;
    for (int i = 0; i <= 6; i++)
    {
      if (kcfg_Block->isChecked() &&
          kcfg_Expire->isChecked() &&
          date_itemlist[m_expireComboList[i]->currentIndex()].num != 0 &&
          date_itemlist[m_blockComboList[i]->currentIndex()].num != 0 &&
          date_itemlist[m_blockComboList[i]->currentIndex()].num >= date_itemlist[m_expireComboList[i]->currentIndex()].num)
      {
        QString format;
        if (!found)
          message.append(i18n("\nIllogical blocking vs. expiration times.\n"));
        found = true;
        message.append(i18n("The blocking time at level %1 should be lower than the expiration time.\n", i + 1));
      }
    }
  }

  if (message.length() != 0)
    KMessageBox::sorry(this, message, i18n("Illogical Values"));
}


bool BlockOptions::isDefault()
{
  return date_itemlist[block7->currentIndex()].num == 5184000 &&
         date_itemlist[block6->currentIndex()].num == 2592000 &&
         date_itemlist[block5->currentIndex()].num == 1209600 &&
         date_itemlist[block4->currentIndex()].num == 604800 &&
         date_itemlist[block3->currentIndex()].num == 345600 &&
         date_itemlist[block2->currentIndex()].num == 172800 &&
         date_itemlist[block1->currentIndex()].num == 86400 &&
         date_itemlist[expire7->currentIndex()].num == 10368000 &&
         date_itemlist[expire6->currentIndex()].num == 5184000 &&
         date_itemlist[expire5->currentIndex()].num == 2592000 &&
         date_itemlist[expire4->currentIndex()].num == 1209600 &&
         date_itemlist[expire3->currentIndex()].num == 604800 &&
         date_itemlist[expire2->currentIndex()].num == 345600 &&
         date_itemlist[expire1->currentIndex()].num == 172800;
}


void BlockOptions::updateSettings()
{
  Prefs::setBlockItem(KV_LEV7_GRADE, date_itemlist[block7->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV6_GRADE, date_itemlist[block6->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV5_GRADE, date_itemlist[block5->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV4_GRADE, date_itemlist[block4->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV3_GRADE, date_itemlist[block3->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV2_GRADE, date_itemlist[block2->currentIndex()].num);
  Prefs::setBlockItem(KV_LEV1_GRADE, date_itemlist[block1->currentIndex()].num);

  Prefs::setExpireItem(KV_LEV7_GRADE, date_itemlist[expire7->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV6_GRADE, date_itemlist[expire6->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV5_GRADE, date_itemlist[expire5->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV4_GRADE, date_itemlist[expire4->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV3_GRADE, date_itemlist[expire3->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV2_GRADE, date_itemlist[expire2->currentIndex()].num);
  Prefs::setExpireItem(KV_LEV1_GRADE, date_itemlist[expire1->currentIndex()].num);
}

bool BlockOptions::hasChanged()
{
  return date_itemlist[block7->currentIndex()].num != Prefs::blockItem(KV_LEV7_GRADE) ||
         date_itemlist[block6->currentIndex()].num != Prefs::blockItem(KV_LEV6_GRADE) ||
         date_itemlist[block5->currentIndex()].num != Prefs::blockItem(KV_LEV5_GRADE) ||
         date_itemlist[block4->currentIndex()].num != Prefs::blockItem(KV_LEV4_GRADE) ||
         date_itemlist[block3->currentIndex()].num != Prefs::blockItem(KV_LEV3_GRADE) ||
         date_itemlist[block2->currentIndex()].num != Prefs::blockItem(KV_LEV2_GRADE) ||
         date_itemlist[block1->currentIndex()].num != Prefs::blockItem(KV_LEV1_GRADE) ||
         date_itemlist[expire7->currentIndex()].num != Prefs::expireItem(KV_LEV7_GRADE) ||
         date_itemlist[expire6->currentIndex()].num != Prefs::expireItem(KV_LEV6_GRADE) ||
         date_itemlist[expire5->currentIndex()].num != Prefs::expireItem(KV_LEV5_GRADE) ||
         date_itemlist[expire4->currentIndex()].num != Prefs::expireItem(KV_LEV4_GRADE) ||
         date_itemlist[expire3->currentIndex()].num != Prefs::expireItem(KV_LEV3_GRADE) ||
         date_itemlist[expire2->currentIndex()].num != Prefs::expireItem(KV_LEV2_GRADE) ||
         date_itemlist[expire1->currentIndex()].num != Prefs::expireItem(KV_LEV1_GRADE);
}

#include "blockoptions.moc"
