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
#include <prefs.h>

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
        //kvq_timeout_t type_to,
        QueryManager *_manager,
        bool          _swapdir,
        bool          _suggestions,
        bool          _split,
        bool          _periods,
        bool          _colons,
        bool          _semicolons,
        bool          _commas,
        int           _fields,
        bool          _show_more,
        bool          _i_know,
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
   connect( enable_crib_lists, SIGNAL(toggled(bool)), SLOT(slotCheckSuggestions(bool)) );
   connect( split_translations, SIGNAL(toggled(bool)), SLOT(slotCheckSplit(bool)) );
   connect( split_at_periods, SIGNAL(toggled(bool)), SLOT(slotCheckPeriods(bool)) );
   connect( split_at_colons, SIGNAL(toggled(bool)), SLOT(slotCheckColons(bool)) );
   connect( split_at_semicolons, SIGNAL(toggled(bool)), SLOT(slotCheckSemicolons(bool)) );
   connect( split_at_commas, SIGNAL(toggled(bool)), SLOT(slotCheckCommas(bool)) );
   connect( enable_show_more, SIGNAL(toggled(bool)), SLOT(slotCheckShowMore(bool)) );
   connect( enable_i_know, SIGNAL(toggled(bool)), SLOT(slotCheckIKnow(bool)) );
   connect( kcfg_AltLearn, SIGNAL(toggled(bool)), SLOT(slotAltLearn(bool)) );

   connect( kcfg_maxTimePer, SIGNAL(textChanged(const QString&)), SLOT(slotChangeMQTime(const QString&)) );
   connect( split_max_fields, SIGNAL(textChanged(const QString&)), SLOT(slotChangeFields(const QString&)) );

   manager = *_manager;
   setCaption(i18n("Options" ));

   validator = new QIntValidator (0, 60*60*24*7, 0); // at least once a week

   kcfg_maxTimePer->setValidator (validator);

   validator = new QIntValidator (2, 10, 0);

   split_max_fields->setValidator (validator);

   setStates(_mqtime, _swapdir, _altlearn, show, _suggestions, _split,
     _periods, _colons, _semicolons, _commas, _fields, _show_more, _i_know);

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


void QueryOptPage::setStates(
        int           _mqtime,
        bool          _swapdir,
        bool          _altlearn,
        bool           show,
        bool          _suggestions,
        bool          _split,
        bool          _periods,
        bool          _colons,
        bool          _semicolons,
        bool          _commas,
        int           _fields,
        bool          _show_more,
        bool          _i_know)
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
   //type_timeout = type_to;
   suggestions = _suggestions;
   split = _split;
   periods = _periods;
   colons = _colons;
   semicolons = _semicolons;
   commas = _commas;
   fields = _fields;
   show_more = _show_more;
   i_know = _i_know;

   QString s;

   s.setNum (mqtime);
   kcfg_maxTimePer->setText (s);
   kcfg_SwapDir->setChecked(swapdir);
   kcfg_AltLearn->setChecked(altlearn);
   kcfg_showcounter->setChecked(show);
   enable_crib_lists->setChecked(suggestions);
   split_translations->setChecked(split);
   slotCheckSplit (split);
   enable_show_more->setChecked(show_more);
   enable_i_know->setChecked(i_know);

   if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
     kcfg_maxTimePer->setEnabled(true);
     kcfg_showcounter->setEnabled(true);
     r_show_to->setChecked (true);
   }
   else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue) {
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

void QueryOptPage::slotChangeFields(const QString& s)
{
   if ( split_max_fields -> isEnabled() )
     fields = atoi (s.local8Bit());
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
   Prefs::setQueryTimeout(Prefs::EnumQueryTimeout::Show);
}


void QueryOptPage::slotContTimeOut()
{
   kcfg_maxTimePer->setEnabled(true);
   kcfg_showcounter->setEnabled(true);
   Prefs::setQueryTimeout(Prefs::EnumQueryTimeout::Continue);
}


void QueryOptPage::slotNoTimeout()
{
   kcfg_maxTimePer->setEnabled(false);
   kcfg_showcounter->setEnabled(false);
   Prefs::setQueryTimeout(Prefs::EnumQueryTimeout::NoTimeout);
}


void QueryOptPage::slotShowRemTime(bool state)
{
   showCounter = state;
}

void QueryOptPage::slotCheckSuggestions(bool _suggestions)
{
   suggestions = _suggestions;
}

void QueryOptPage::slotCheckSplit(bool _split)
{
   split = _split;
   if (split) {
     label_at->setEnabled(true);
     split_at_periods->setEnabled(true);
     split_at_periods->setChecked(periods);
     split_at_colons->setEnabled(true);
     split_at_colons->setChecked(colons);
     split_at_semicolons->setEnabled(true);
     split_at_semicolons->setChecked(semicolons);
     split_at_commas->setEnabled(true);
     split_at_commas->setChecked(commas);
     label_split_max_fields->setEnabled(true);
     split_max_fields->setEnabled(true);
     split_max_fields->setText (QString::number(fields));
   }
   else {
     label_at->setEnabled(false);
     split_at_periods->setEnabled(false);
     split_at_periods->setChecked(false);
     split_at_colons->setEnabled(false);
     split_at_colons->setChecked(false);
     split_at_semicolons->setEnabled(false);
     split_at_semicolons->setChecked(false);
     split_at_commas->setEnabled(false);
     split_at_commas->setChecked(false);
     label_split_max_fields->setEnabled(false);
     split_max_fields->setEnabled(false);
     split_max_fields->setText ("");
   }
}

void QueryOptPage::slotCheckPeriods(bool _periods)
{
   if ( split_at_periods -> isEnabled() )
     periods = _periods;
}

void QueryOptPage::slotCheckColons(bool _colons)
{
   if ( split_at_colons -> isEnabled() )
     colons = _colons;
}

void QueryOptPage::slotCheckSemicolons(bool _semicolons)
{
   if ( split_at_semicolons -> isEnabled() )
     semicolons = _semicolons;
}

void QueryOptPage::slotCheckCommas(bool _commas)
{
   if ( split_at_commas -> isEnabled() )
     commas = _commas;
}

void QueryOptPage::slotCheckShowMore(bool _show_more)
{
   show_more = _show_more;
}

void QueryOptPage::slotCheckIKnow(bool _i_know)
{
   i_know = _i_know;
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
