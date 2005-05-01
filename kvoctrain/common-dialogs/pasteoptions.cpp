/***************************************************************************

                      paste options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 31 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

#include <qpushbutton.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qstringlist.h>

#include <kcombobox.h>
#include <klocale.h>

#include "pasteoptions.h"
#include "prefs.h"
#include "kvoctraindoc.h"
#include "langset.h"

static const char *separator_id[] =
{
  ";",              // 0
  "#",              // 1
  "!",              // 2
  "|",              // 3
  ",",              // 4
  "\t",             // 5
  "  ",             // 6
  ":",              // 7
  "::",             // 8
  0
};

PasteOptions::PasteOptions(LangSet & langset, kvoctrainDoc * doc, QWidget* parent, const char* name, WFlags fl)
: PasteOptionsBase(parent,name,fl), m_langSet(langset)
{
  OrderList->setEnabled(!kcfg_UseCurrent->isChecked());
  UpButton->setEnabled(!kcfg_UseCurrent->isChecked());
  SkipButton->setEnabled(!kcfg_UseCurrent->isChecked());
  DownButton->setEnabled(!kcfg_UseCurrent->isChecked());

  connect(OrderList, SIGNAL(selectionChanged()), this, SLOT(syncButtons()));
  connect(DownButton, SIGNAL(clicked()), this, SLOT(slotDownButtonClicked()));
  connect(SkipButton, SIGNAL(clicked()), this, SLOT(slotSkipButtonClicked()));
  connect(UpButton, SIGNAL(clicked()), this, SLOT(slotUpButtonClicked()));

  connect(SeparatorCombo, SIGNAL(activated(int)), this, SLOT(slotSeparatorComboActivated(int)));
  connect(kcfg_UseCurrent, SIGNAL(toggled(bool)), this, SLOT(slotUseCurrentDocToggled(bool)));

  m_doc = doc;
  fillWidgets();
  updateWidgets();
}

void PasteOptions::fillWidgets()
{
  SeparatorCombo->insertItem(i18n(";"));
  SeparatorCombo->insertItem(i18n("#"));
  SeparatorCombo->insertItem(i18n("!"));
  SeparatorCombo->insertItem(i18n("|"));
  SeparatorCombo->insertItem(i18n(","));
  SeparatorCombo->insertItem(i18n("TAB"));
  SeparatorCombo->insertItem(i18n(">= 2 SPACES"));
  SeparatorCombo->insertItem(i18n(" : "));
  SeparatorCombo->insertItem(i18n(" :: "));

  QStringList sl = Prefs::pasteOrder();
  OrderList->clear();
  for (int i = 0; i < (int) sl.count(); i++)
  {
    QString codename = m_langSet.findLongId(sl[i]);
    if (codename.isEmpty())
      codename = sl[i];
    OrderList->insertItem(codename);
  }
  // any new languages to add to the listbox?
  for (int i = 0; i < (int) m_langSet.size(); i++)
  {
    bool found = false;
    for (int j = 0; j < (int) OrderList->count(); j++)
      if (OrderList->text(j) == m_langSet.longId(i))
        found = true;

    if (!found)
      OrderList->insertItem(m_langSet.longId(i));
  }
}

void PasteOptions::updateWidgets()
{
  for (int i = 0; i < 9; ++i)
  {
    if (separator_id[i] == Prefs::separator())
    {
      SeparatorCombo->setCurrentItem(i);
      break;
    }
  }
}


void PasteOptions::syncButtons()
{
  bool usingCurrent  = kcfg_UseCurrent->isChecked();
  OrderList->setDisabled(usingCurrent);
  if (OrderList->count() > 1 && !usingCurrent)
  {
    DownButton->setEnabled(OrderList->currentItem() < (int) OrderList->count()-1);
    SkipButton->setEnabled(true);
    UpButton->setEnabled(OrderList->currentItem() != 0);
  }
  else
  {
    DownButton->setEnabled(false);
    SkipButton->setEnabled(false);
    UpButton->setEnabled(false);
  }
}

void PasteOptions::slotUpButtonClicked()
{
  if (OrderList->currentItem() > 0 && (int) OrderList->count() > 1)
  {
    int pos = OrderList->currentItem();
    QString item = OrderList->text(pos);
    OrderList->removeItem(pos);
    OrderList->insertItem(item, pos-1);
    OrderList->setCurrentItem(pos-1);
    emit widgetModified();
  }
  syncButtons();
}

void PasteOptions::slotSkipButtonClicked()
{
  int pos = OrderList->currentItem();
  OrderList->insertItem("", OrderList->currentItem());
  OrderList->setCurrentItem(pos);
  syncButtons();
  emit widgetModified();
}

void PasteOptions::slotDownButtonClicked()
{
  if ((int)OrderList->currentItem() < (int) OrderList->count()-1 && (int) OrderList->count() >  1)
  {
    int pos = OrderList->currentItem();
    QString item = OrderList->text(pos);
    OrderList->removeItem(pos);
    OrderList->insertItem(item, pos+1);
    OrderList->setCurrentItem(pos+1);
    emit widgetModified();
  }
  syncButtons();
}

void PasteOptions::slotSeparatorComboActivated(int)
{
  emit widgetModified();
}

void PasteOptions::slotUseCurrentDocToggled(bool)
{
  OrderList->clear();
  OrderList->insertItem (m_langSet.findLongId(m_doc->getOriginalIdent()));
  for (int i = 1; i < (int) m_doc->numLangs(); i++)
    OrderList->insertItem (m_langSet.findLongId(m_doc->getIdent(i)));

  // any other languages to append besides those from doc?
  for (int i = 0; i < (int) m_langSet.size(); i++)
  {
    bool found = false;
    for (int j = 0; j < (int) OrderList->count(); j++)
      if (OrderList->text(j) == m_langSet.longId(i))
        found = true;

    if (!found)
      OrderList->insertItem(m_langSet.longId(i));
  }

  OrderList->setCurrentItem (0);
}

bool PasteOptions::hasChanged()
{
  bool result;
  result = ((separator_id[SeparatorCombo->currentItem()] != Prefs::separator()));
  if (result)
    return true;
  QStringList ol, pl;
  pl = Prefs::pasteOrder();
  ol = preparePasteOrderList();
  result = (pl.count() != ol.count());
  if (result)
    return true;
  for (uint i = 0; i < ol.count(); i--)
  {
    result = (ol[i] != pl[i]);
    if (result)
      return true;
  }
  return false;
}

bool PasteOptions::isDefault()
{
  return separator_id[SeparatorCombo->currentItem()] == "\t";
}

void PasteOptions::updateSettings()
{
  Prefs::setSeparator((separator_id[SeparatorCombo->currentItem()]));
  Prefs::setPasteOrder(preparePasteOrderList());
}


void PasteOptions::slotSyncPasteOrderList()
{
  for (int j = (int) OrderList->count()-1; j > 1; j--)
  {
    QString comp = OrderList->text(j);
    for (int i = (int) j-1; i >=0; i--)
    {
      if (OrderList->text(i) == comp && !OrderList->text(i).isEmpty())
      {
        OrderList->removeItem(j);
        break;
      }
    }
  }

  // order_list contains elements unknown in language set? remove them
  for (int i = (int) OrderList->count()-1; i >=0; i--)
  {
    if (!OrderList->text(i).isEmpty())
    {
      if (m_langSet.indexLongId(OrderList->text(i)) < 0)
      {
        OrderList->removeItem(i);
      }
    }
  }

  // language set contains new elements? append them
  for (int i = 0; i < (int) m_langSet.size(); i++)
  {
    bool found = false;
    for (int j = 0; j < (int) OrderList->count(); j++)
      if (m_langSet.longId(i) == OrderList->text(j))
        found = true;

    if (!found)
    {
      if (m_langSet.longId(i).isEmpty())
        OrderList->insertItem(m_langSet.shortId(i));
      else
        OrderList->insertItem(m_langSet.longId(i));
    }
  }

  // skip "skip" elements at tail
  int i;
  for (i = (int) OrderList->count()-1; i >= 0 && OrderList->text(i).isEmpty(); i--)
  {
   if (OrderList->currentItem() > (int) OrderList->count()-2)
     OrderList->setCurrentItem(OrderList->count()-2);
   OrderList->removeItem(i);
  }
}

QStringList PasteOptions::preparePasteOrderList()
{
  slotSyncPasteOrderList();
  QStringList sl;
  // skip "skip" elements at tail
  int i;
  for (i = (int) OrderList->count()-1; i >= 0 && OrderList->text(i).isEmpty(); i--);

  // insert language codes and "skips" between them
  for ( /**/; i >= 0; i--)
  {
    QString code = m_langSet.findShortId(OrderList->text(i));
    if (code.isEmpty())
      code = OrderList->text(i); // no longId previously available
    if (OrderList->text(i).isEmpty())
      sl.insert (sl.begin(), "");
    else
      sl.insert (sl.begin(), code);
  }
  return sl;
}

#include "pasteoptions.moc"

