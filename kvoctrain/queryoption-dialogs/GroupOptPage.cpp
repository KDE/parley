/***************************************************************************

    $Id$

                     query groups dialog page

    -----------------------------------------------------------------------

    begin                : Mon Nov 1 13:10:21 1999
                                           
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

#include "GroupOptPage.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <klocale.h>
#include <klineeditdlg.h>

GroupOptPage::GroupOptPage
(
        vector<PreSetting>  preset,
	QWidget            *parent,
	const char         *name
)
	:
	GroupOptPageForm( parent, name )
{
	connect( ps_del, SIGNAL(clicked()), SLOT(slotDeleteGroup()) );
	connect( ps_new, SIGNAL(clicked()), SLOT(slotNewGroup()) );
	connect( ps_recall, SIGNAL(clicked()), SLOT(slotRecallGroup()) );
	connect( ps_store, SIGNAL(clicked()), SLOT(slotStoreGroup()) );
	connect( ps_name, SIGNAL(activated(int)), SLOT(slotGroupSelected(int)) );

	setCaption(i18n("Groups"));
        l_name->setBuddy(ps_name);
        for (int i = 0; i < (int) preset.size(); i++)
          ps_name->insertItem (preset[i].name);
        updateButtons();
}


void GroupOptPage::initFocus() const
{
  ps_name->setFocus();
}


void GroupOptPage::slotGroupSelected(int item)
{
}


void GroupOptPage::slotDeleteGroup()
{
  if (ps_name->count() != 0) {
    emit deleteGroup (ps_name->currentItem());
    int curr = ps_name->currentItem();
    ps_name->removeItem(curr);
    if (curr >= ps_name->count()-1)
      ps_name->setCurrentItem(ps_name->count()-1);
  }

   updateButtons();
}


void GroupOptPage::slotNewGroup()
{
  KLineEditDlg dlg(i18n("Enter group description:"), QString::null, this);
  dlg.setCaption(i18n("Group Description"));
  if (dlg.exec()) {
     ps_name->insertItem(dlg.text().stripWhiteSpace());
     emit newGroup(dlg.text().stripWhiteSpace());
     ps_name->setCurrentItem(ps_name->count()-1);
     emit selectGroup(ps_name->currentItem());
   }
   updateButtons();
}


void GroupOptPage::slotRecallGroup()
{
  if (ps_name->count() != 0)
    emit selectGroup(ps_name->currentItem());
}


void GroupOptPage::slotStoreGroup()
{
  if (ps_name->count() != 0)
    emit modifyGroup(ps_name->currentItem());
}


void GroupOptPage::updateButtons()
{
   ps_del->setEnabled (ps_name->count() > 0);
   ps_store->setEnabled (ps_name->count() > 0);
   ps_recall->setEnabled (ps_name->count() > 0);
   ps_name->setEnabled (ps_name->count() > 0);
}


void GroupOptPage::keyPressEvent( QKeyEvent *e )
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

#include "GroupOptPage.moc"

