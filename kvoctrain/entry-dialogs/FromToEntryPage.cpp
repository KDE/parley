/***************************************************************************

    $Id$

                     edit properties from/to original

    -----------------------------------------------------------------------

    begin                : Mon Jun 28 21:02:16 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/05 17:33:17  arnold
    FIX: slot signature changed to KDE2

    Revision 1.1  2001/10/05 15:40:37  arnold
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


#include "FromToEntryPage.h"

#define Inherited FromToEntryPageData

#include <qkeycode.h>

#include <kapp.h>

#include <kvoctraindoc.h>
#include <QueryManager.h>
#include <langset.h>

#include "blockall.h"

  static QStringList  monthnames;

FromToEntryPage::FromToEntryPage
(
        SpecFont_t *font,
        QDialog    *_dlgbook,
        bool        multi_sel,
        grade_t     _grade,
        time_t      _time,
        count_t     _qcount,
        count_t     _bcount,
        QString     faux,
        QString     label,
	QWidget    *parent,
	const char *name
)
	:
	Inherited( parent, name ),
        fauxami(faux),
        dlgbook(_dlgbook)
{

        monthnames.clear();
        monthnames.append ("");
        monthnames.append (i18n("January"));
        monthnames.append (i18n("February"));
        monthnames.append (i18n("March"));
        monthnames.append (i18n("April"));
        monthnames.append (i18n("May"));
        monthnames.append (i18n("June"));
        monthnames.append (i18n("July"));
        monthnames.append (i18n("August"));
        monthnames.append (i18n("September"));
        monthnames.append (i18n("October"));
        monthnames.append (i18n("November"));
        monthnames.append (i18n("December"));

	connect( bcount_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( qcount_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( fauxami_line, SIGNAL(returnPressed()), SLOT(accept()) );

	connect( bcount_line, SIGNAL(textChanged(const QString&)),  SLOT(slotBCount(const QString&)) );
	connect( qcount_line, SIGNAL(textChanged(const QString&)),  SLOT(slotQCount(const QString&)) );
	connect( fauxami_line, SIGNAL(textChanged(const QString&)), SLOT(slotFauxAmiSelected(const QString&)) );

	connect( never, SIGNAL(clicked()), SLOT(slotNever()) );
	connect( today, SIGNAL(clicked()), SLOT(slotToday()) );
	connect( gradebox, SIGNAL(highlighted(int)), SLOT(slotGradeSelected(int)) );

	connect( year_spin,  SIGNAL(valueChanged(int)), SLOT(slotYearChanged(int)) );
	connect( month_spin, SIGNAL(valueChanged(int)), SLOT(slotMonthChanged(int)) );
	connect( day_spin,   SIGNAL(valueChanged(int)), SLOT(slotDayChanged(int)) );

        year_spin->setData ((QStringList *) 0, 1980, 2100);
        month_spin->setData (&monthnames, 1, 12);
        day_spin->setData ((QStringList *) 0, 1, 31);

        grade = _grade;
        qcount = _qcount;
        bcount = _bcount;

	fauxami_line->setText(fauxami);
        fauxami_label->setBuddy(fauxami_line);

        valid_date = false;
        QDateTime dt;
        QDate date;
        if (_time != 0 && !multi_sel) {
          dt.setTime_t (_time);
          valid_date = true;

          date = dt.date();
          year = date.year();
          month = date.month();
          day = date.day();
  
          day_spin->setValue(day);
          month_spin->setValue(month);
          year_spin->setValue(year);
          year_spin->setSpecial(QString::null);
          month_spin->setSpecial(QString::null);
          day_spin->setSpecial(QString::null);
        }
        else {
          dt.setTime_t (time(0L));
          date = dt.date();
          year = date.year();
          month = date.month();
          day = date.day();
          year_spin->setSpecial("----");
          month_spin->setSpecial("----");
          day_spin->setSpecial("--");

        }

        direc_label->setTitle (label);

        QString s;
        for (int i = 0; i <= KV_MAX_GRADE; i++) {
          s.setNum (i);
          gradebox->insertItem( QueryManager::gradeStr(i) );
        }
        gradebox->setCurrentItem (grade);
        grade_label->setBuddy (gradebox);

        s.setNum (qcount);
        qcount_line->setText (s);
        qcount_label->setBuddy (qcount_line);

        s.setNum (bcount);
        bcount_line->setText (s);
        bcount_label->setBuddy (bcount_line);

        gradebox->setValidator (new BlockAllValidator() );
        if (multi_sel) {
          fauxami_line->setEnabled(false);
          valid_date = false;
          bcount_line->setText ("");
          qcount_line->setText ("");
          month_spin->setSpecial(" ");
          day_spin->setSpecial(" ");
          // FIXME: possibly derive new combobox type
          //        which filters ALL charcters to prevent new input
          //        in edit field
          gradebox->clearEdit();
        }

        bcount_dirty = false;
        qcount_dirty = false;
        date_dirty = false;
        grade_dirty = false;

        if (font != 0 && font->specfont) {
          QFont specfont = font->limitedFont();
          fauxami_line ->setFont (specfont);
        }
}


void FromToEntryPage::initFocus() const
{
  gradebox->setFocus();
}


void FromToEntryPage::slotFauxAmiSelected(const QString& s)
{
   fauxami = s;
}


void FromToEntryPage::slotGradeSelected (int g)
{
   grade_dirty = true;
   grade = g;
}


void FromToEntryPage::slotQCount(const QString& s)
{
   qcount_dirty = true;
   qcount = atoi (s.local8Bit());
}


void FromToEntryPage::slotBCount(const QString& s)
{
   bcount_dirty = true;
   bcount = atoi (s.local8Bit());
}


time_t FromToEntryPage::getDate   () const
{
// FIXME: warning dialog/don`t quit dialog when date invalid
   if (valid_date) {
     QDate act_date (year, month, day);
     QDateTime time_null (QDate (1970, 1, 1), QTime (0,0,0));
     return -QDateTime(act_date).secsTo (time_null);
   }
   else
     return 0;
}


void FromToEntryPage::validate()
{
  if (!valid_date) {
    valid_date = true;
    day_spin->setValue(day);
    month_spin->setValue(month);
    year_spin->setValue(year);
  }
}


void FromToEntryPage::slotYearChanged(int new_year)
{
   date_dirty = true;
   if (!valid_date) {
     slotToday();
     new_year = year;
   }

   year_spin->setSpecial(QString::null);

   year = new_year;
   validate();
}


void FromToEntryPage::slotDecYear()
{
}


// FIXME: dec month when day decrease below 1 ..
void FromToEntryPage::slotIncYear()
{
}


void FromToEntryPage::slotMonthChanged(int new_month)
{
   date_dirty = true;
   if (!valid_date) {
     slotToday();
     new_month = month;
   }

   month_spin->setSpecial(QString::null);

   month = new_month;
   validate();
}


void FromToEntryPage::slotDecMonth()
{
}


void FromToEntryPage::slotIncMonth()
{
}


void FromToEntryPage::slotDayChanged(int new_day)
{
   date_dirty = true;
   if (!valid_date) {
     slotToday();
     new_day = day;
   }

   day_spin->setSpecial(QString::null);

   day = new_day;
   validate();
}


void FromToEntryPage::slotDecDay()
{
}


void FromToEntryPage::slotIncDay()
{
}


void FromToEntryPage::slotToday()
{
   date_dirty = true;
   QDateTime dt;
   dt.setTime_t (time(0L));

   year = dt.date().year();
   month = dt.date().month();
   day = dt.date().day();

   day_spin->setValue(day);
   month_spin->setValue(month);
   year_spin->setValue(year);
   year_spin->setSpecial(QString::null);
   month_spin->setSpecial(QString::null);
   day_spin->setSpecial(QString::null);
   validate();
}


void FromToEntryPage::slotNever()
{
   date_dirty = true;
   year = 0;
   month = 0;
   day = 0;
   year_spin->setSpecial("----");
   month_spin->setSpecial("----");
   day_spin->setSpecial("--");
   valid_date = false;
}


void FromToEntryPage::keyPressEvent( QKeyEvent *e )
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
#include "FromToEntryPage.moc"
