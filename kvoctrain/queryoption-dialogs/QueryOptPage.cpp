/***************************************************************************

    $Id$

                     query options dialog page

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999

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

#include "QueryOptPage.h"

#include <qkeycode.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qvalidator.h>

#include <stdlib.h>

#include <kapplication.h>

QueryOptPage::QueryOptPage
(
        int           _mqtime,
        bool          show,
        kvq_timeout_t type_to,
        QueryManager *_manager,
        bool          _swapdir,
        bool          _altlearn,
	QWidget      *parent,
	const char   *name
)
	:
	QueryOptPageForm( parent, name )
{
   query_group->insert( r_show_to );
   query_group->insert( r_cont_to );
   query_group->insert( kcfg_queryTimeout );

   connect( kcfg_showcounter, SIGNAL(toggled(bool)), SLOT(slotShowRemTime(bool)) );
   connect( kcfg_queryTimeout, SIGNAL(clicked()), SLOT(slotNoTimeout()) );
   connect( r_cont_to, SIGNAL(clicked()), SLOT(slotContTimeOut()) );
   connect( r_show_to, SIGNAL(clicked()), SLOT(slotShowTimeout()) );
   connect( kcfg_SwapDir, SIGNAL(toggled(bool)), SLOT(slotCheckSwap(bool)) );
   connect( kcfg_AltLearn, SIGNAL(toggled(bool)), SLOT(slotAltLearn(bool)) );

   connect( kcfg_maxTimePer, SIGNAL(textChanged(const QString&)), SLOT(slotChangeMQTime(const QString&)) );

   manager = *_manager;
   setCaption(i18n("Options" ));

   validator = new QIntValidator (0, 60*60*24*7, 0); // at least once a week

   kcfg_maxTimePer->setValidator (validator);

   setStates(_mqtime, _swapdir, _altlearn, show, type_to);

   // FIXME: until really needed
   GroupBox5->hide();
   ask_sub->hide();
   ask_verbcon->hide();
   ask_article->hide();
   ask_compare->hide();
   ask_syno->hide();
   ask_anto->hide();
   ask_examp->hide();
   ask_para->hide();
}


void QueryOptPage::setStates(int _mqtime, bool _swapdir, bool _altlearn, bool show, kvq_timeout_t type_to)
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
   altlearn = _altlearn;
   mqtime = _mqtime;
   showCounter = show;
   type_timeout = type_to;

   QString s;

   s.setNum (mqtime);
   kcfg_maxTimePer->setText (s);
   kcfg_SwapDir->setChecked(swapdir);
   kcfg_AltLearn->setChecked(altlearn);
   kcfg_showcounter->setChecked(show);

   if (type_to == kvq_show) {
     kcfg_maxTimePer->setEnabled(true);
     kcfg_showcounter->setEnabled(true);
     r_show_to->setChecked (true);
   }
   else if (type_to == kvq_cont) {
     kcfg_maxTimePer->setEnabled(true);
     kcfg_showcounter->setEnabled(true);
     r_cont_to->setChecked (true);
   }
   else {
     kcfg_maxTimePer->setEnabled(false);
     kcfg_showcounter->setEnabled(false);
     kcfg_queryTimeout->setChecked (true);
   }
}


void QueryOptPage::initFocus() const
{
  kcfg_maxTimePer->setFocus();
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

void QueryOptPage::slotAltLearn(bool state)
{
   altlearn = state;
   emit modifySetting();
}


void QueryOptPage::slotShowTimeout()
{
   kcfg_maxTimePer->setEnabled(true);
   kcfg_showcounter->setEnabled(true);
   type_timeout = kvq_show;
}


void QueryOptPage::slotContTimeOut()
{
   kcfg_maxTimePer->setEnabled(true);
   kcfg_showcounter->setEnabled(true);
   type_timeout = kvq_cont;
}


void QueryOptPage::slotNoTimeout()
{
   kcfg_maxTimePer->setEnabled(false);
   kcfg_showcounter->setEnabled(false);
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
