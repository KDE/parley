/***************************************************************************

    $Id$

                    clipboard options dialog page

    -----------------------------------------------------------------------

    begin                : Sun Jun 27 11:07:24 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

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

#include <qlistbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>

#include <iostream>
using namespace std;

#include <klocale.h>


#include <langset.h>

#include "PasteOptPage.h"
#include "GeneralOptionsDlg.h"

static const char *separator_name[] = {
  ";",                         // 0
  "#",                         // 1
  "!",                         // 2
  "|",                         // 3
  ",",                         // 4
  I18N_NOOP("TAB"),            // 5
  I18N_NOOP(">= 2 SPACES"),    // 6
  " : ",                       // 7
  " :: ",                      // 8
  0
};


static const char *separator_id[] = {
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


PasteOptPage::PasteOptPage
(
        QString       _sep,
        LangSet      &_langset,
        QStringList  &paste_list,
        bool          _useCurrent,
        kvoctrainDoc *_doc,
	QWidget      *parent,
	const char   *name
)
	:
	PasteOptPageForm( parent, name ),
        langset(_langset),
        doc(_doc)
{
  connect( item_curr_doc, SIGNAL(toggled(bool)), SLOT(slotCurrentDoc(bool)) );
  connect( item_down, SIGNAL(clicked()), SLOT(slotItemDown()) );
  connect( item_skip, SIGNAL(clicked()), SLOT(slotItemSkip()) );
  connect( item_up, SIGNAL(clicked()), SLOT(slotItemUp()) );
  connect( d_sep, SIGNAL(highlighted(int)), SLOT(slotSelectSep(int)) );
  connect( order_list, SIGNAL(highlighted(int)), SLOT(slotSelectOrder(int)) );


  setCaption(i18n("Options" ));
  sep = _sep;
  useCurrent = _useCurrent;

  int sel = 0;
  const char **seps = separator_name;
  const char **sepid = separator_id;
  while (*seps) {
    if (*sepid == sep)
      sel = seps - separator_name;
    d_sep->insertItem (i18n(*seps));
    seps++;
    sepid++;
  }
  d_sep->setSizeLimit( seps - separator_name );
  d_sep->setCurrentItem(sel);
  label_sep->setBuddy(d_sep);

  order_list->clear();
  for (int i = 0; i < (int) paste_list.count(); i++) {
    QString codename = langset.findLongId(paste_list[i]);

    if (codename.isEmpty())
      codename = paste_list[i];
    order_list->insertItem (codename);
  }

  // any new languages to add to order-list ?
  for (int i = 0; i < (int) langset.size(); i++) {
    bool found = false;
    for (int j = 0; j < (int) order_list->count(); j++)
      if (order_list->text(j) == langset.longId(i))
        found = true;

    if (!found)
      order_list->insertItem(langset.longId(i));
  }

  order_list->setCurrentItem (0);
  item_curr_doc->setChecked (useCurrent);
  order_list->setEnabled(!useCurrent);
  item_down->setEnabled(!useCurrent);
  item_skip->setEnabled(!useCurrent);
  item_up->setEnabled(!useCurrent);
  slotEnableCursor();
}


void PasteOptPage::initFocus() const
{
  d_sep->setFocus();
}


void PasteOptPage::slotSyncLangSet(const QString& dlg_name)
{
  // skip "skip" elements at tail
  if (dlg_name == i18n("&Clipboard")) {
    // order_list contains doublettes??
    for (int j = (int) order_list->count()-1; j > 1; j--) {
      QString comp = order_list->text(j);
      for (int i = (int) j-1; i >=0; i--) {
        if (   order_list->text(i) == comp
            && !order_list->text(i).isEmpty()) {
//          cout << "remove " << j << " " << order_list->text(j).latin1() << endl;
          order_list->removeItem(j);
          break;
        }
      }
    }
    // order_list contains elements unknown in language set? remove them
    for (int i = (int) order_list->count()-1; i >=0; i--) {
//      cout << order_list->text(i).latin1() << endl;
      if (!order_list->text(i).isEmpty())
        if (langset.indexLongId(order_list->text(i)) < 0) {
          order_list->removeItem(i);
        }
    }
    // language set contains new elements? append them
    for (int i = 0; i < (int) langset.size(); i++) {
//    cout << langset.longId(i).latin1() << endl;
//    cout << langset.shortId(i).latin1() << endl;
      bool found = false;
      for (int j = 0; j < (int) order_list->count(); j++)
        if (langset.longId(i) == order_list->text(j))
          found = true;

      if (!found) {
        if (langset.longId(i).isEmpty())
          order_list->insertItem (langset.shortId(i));
        else
          order_list->insertItem (langset.longId(i));
      }
    }
  }

  int i;
  for (i = (int) order_list->count()-1;
       i >= 0 && order_list->text(i).isEmpty();
       i--) {
   if (order_list->currentItem() > (int) order_list->count()-2)
     order_list->setCurrentItem(order_list->count()-2);
   order_list->removeItem(i);
  }
}


void PasteOptPage::slotSelectSep(int idx)
{
  sep = separator_id[idx];
}


void PasteOptPage::slotSelectOrder(int idx)
{
  slotEnableCursor();
}


QStringList PasteOptPage::getPasteOrder()
{
  slotSyncLangSet(i18n("&Clipboard"));
  QStringList sl;
  // skip "skip" elements at tail
  int i;
  for (i = (int) order_list->count()-1;
       i >= 0 && order_list->text(i).isEmpty();
       i--);

  // insert language codes and "skips" between them
  for ( /**/; i >= 0; i--) {
    QString code = langset.findShortId(order_list->text(i));
    if (code.isEmpty())
      code = order_list->text(i); // no longId previously available
    if (order_list->text(i).isEmpty())
      sl.insert (sl.begin(), "");
    else
      sl.insert (sl.begin(), code);
  }
  return sl;
}


void PasteOptPage::slotItemSkip()
{
  int pos = order_list->currentItem();
  order_list->insertItem("", order_list->currentItem());
  order_list->setCurrentItem(pos);
  slotSyncLangSet(i18n("&Clipboard"));
  order_list->setFocus();
  slotEnableCursor();
}


void PasteOptPage::slotEnableCursor()
{
  order_list->setEnabled(!useCurrent);
  if (order_list->count() > 1 && !useCurrent) {
    item_down->setEnabled(order_list->currentItem() < (int) order_list->count()-1);
    item_skip->setEnabled(true);
    item_up->setEnabled(order_list->currentItem() != 0);
  }
  else {
    item_down->setEnabled(false);
    item_skip->setEnabled(false);
    item_up->setEnabled(false);
  }
}


void PasteOptPage::slotItemUp()
{
  if (order_list->currentItem() > 0
      && (int) order_list->count() > 1) {
    int pos = order_list->currentItem();
    QString item = order_list->text(pos);
    order_list->removeItem(pos);
    order_list->insertItem(item, pos-1);
    order_list->setCurrentItem(pos-1);
    slotSyncLangSet(i18n("&Clipboard"));
    order_list->setFocus();
  }
  slotEnableCursor();
}


void PasteOptPage::slotItemDown()
{
  if ((int)order_list->currentItem() < (int)order_list->count()-1
      && (int) order_list->count() >  1) {
    int pos = order_list->currentItem();
    QString item = order_list->text(pos);
    order_list->removeItem(pos);
    order_list->insertItem(item, pos+1);
    order_list->setCurrentItem(pos+1);
    slotSyncLangSet(i18n("&Clipboard"));
    order_list->setFocus();
  }
  slotEnableCursor();
}


void PasteOptPage::slotCurrentDoc(bool use)
{

  order_list->clear();
  order_list->insertItem (langset.findLongId(doc->getOriginalIdent()));
  for (int i = 1; i < (int) doc->numLangs(); i++)
    order_list->insertItem (langset.findLongId(doc->getIdent(i)));

  // any other languages to append besides those from doc?
  for (int i = 0; i < (int) langset.size(); i++) {
    bool found = false;
    for (int j = 0; j < (int) order_list->count(); j++)
      if (order_list->text(j) == langset.longId(i))
        found = true;

    if (!found)
      order_list->insertItem(langset.longId(i));
  }

  order_list->setCurrentItem (0);

  useCurrent = use;

  order_list->setEnabled(!useCurrent);
  item_down->setEnabled(!useCurrent);
  item_skip->setEnabled(!useCurrent);
  item_up->setEnabled(!useCurrent);

  slotEnableCursor();
}


void PasteOptPage::keyPressEvent( QKeyEvent *e )
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
#include "PasteOptPage.moc"
