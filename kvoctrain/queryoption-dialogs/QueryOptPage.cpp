/***************************************************************************

    $Id$

                     query options dialog page

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.3  2001/10/28 16:56:56  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:46:01  arnold
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

#include "QueryOptPage.h"
#include <qvalidator.h>

#include <qkeycode.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qvalidator.h>
#include <qlineedit.h>

#include <stdlib.h>

#include <kapp.h>

QueryOptPage::QueryOptPage
(
        int           _mqtime,
        bool          show,
        kvq_timeout_t type_to,
        QueryManager *_manager,
        bool          _swapdir,
	QWidget      *parent,
	const char   *name
)
	:
	QueryOptPageForm( parent, name )
{
   query_group->insert( r_show_to );
   query_group->insert( r_cont_to );
   query_group->insert( r_nolimit_to );

   connect( showrem, SIGNAL(toggled(bool)), SLOT(slotShowRemTime(bool)) );
   connect( r_nolimit_to, SIGNAL(clicked()), SLOT(slotNoTimeout()) );
   connect( r_cont_to, SIGNAL(clicked()), SLOT(slotContTimeOut()) );
   connect( r_show_to, SIGNAL(clicked()), SLOT(slotShowTimeout()) );
   connect( swap, SIGNAL(toggled(bool)), SLOT(slotCheckSwap(bool)) );

   connect( e_mqtime, SIGNAL(textChanged(const QString&)), SLOT(slotChangeMQTime(const QString&)) );

   manager = *_manager;
   setCaption(i18n("Options" ));

   validator = new QIntValidator (0, 60*60*24*7, 0); // at least once a week

   e_mqtime->setValidator (validator);
   label_mqtime->setBuddy(e_mqtime);

   setStates(_mqtime, _swapdir, show, type_to);
}


void QueryOptPage::setStates(int _mqtime, bool _swapdir, bool show, kvq_timeout_t type_to)
{
   ask_sub->setEnabled(false);
   ask_verbcon->setEnabled(false);
   ask_article->setEnabled(false);
   ask_compare->setEnabled(false);
   ask_syno->setEnabled(false);
   ask_anto->setEnabled(false);
   ask_examp->setEnabled(false);
   ask_para->setEnabled(false);

   swapdir = _swapdir;
   mqtime = _mqtime;
   showCounter = show;
   type_timeout = type_to;

   QString s;

   s.setNum (mqtime);
   e_mqtime->setText (s);
   swap->setChecked(swapdir);
   showrem->setChecked(show);

   if (type_to == kvq_show) {
     e_mqtime->setEnabled(true);
     showrem->setEnabled(true);
     r_show_to->setChecked (true);
   }
   else if (type_to == kvq_cont) {
     e_mqtime->setEnabled(true);
     showrem->setEnabled(true);
     r_cont_to->setChecked (true);
   }
   else {
     e_mqtime->setEnabled(false);
     showrem->setEnabled(false);
     r_nolimit_to->setChecked (true);
   }
}


void QueryOptPage::initFocus() const
{
  e_mqtime->setFocus();
}


void QueryOptPage::slotChangeMQTime(const QString& s)
{
   mqtime = atoi (s.local8Bit());
   emit modifySetting();
}


void QueryOptPage::slotCheckSwap(bool state)
{
   swapdir = state;
   emit modifySetting();
}


void QueryOptPage::slotShowTimeout()
{
   e_mqtime->setEnabled(true);
   showrem->setEnabled(true);
   type_timeout = kvq_show;
}


void QueryOptPage::slotContTimeOut()
{
   e_mqtime->setEnabled(true);
   showrem->setEnabled(true);
   type_timeout = kvq_cont;
}


void QueryOptPage::slotNoTimeout()
{
   e_mqtime->setEnabled(false);
   showrem->setEnabled(false);
   type_timeout = kvq_notimeout;
}


void QueryOptPage::slotShowRemTime(bool state)
{
   showCounter = state;
}


void QueryOptPage::keyPressEvent( QKeyEvent *e )
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
#include "QueryOptPage.moc"
