/***************************************************************************

                     edit properties from/to original

    -----------------------------------------------------------------------

    begin          : Mon Jun 28 21:02:16 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>

#include <klocale.h>

#include <QueryManager.h>
#include <langset.h>
#include "FromToEntryPage.h"
#include "EntryDlg.h"
#include "blockall.h"
#include "MySpinBox.h"

static QStringList  monthnames;

FromToEntryPage::FromToEntryPage
(
  bool        multi_sel,
  grade_t     _grade,
  time_t      _time,
  count_t     _qcount,
  count_t     _bcount,
  QString     faux,
  QString     label,
  QWidget    *parent
)
  :
  QWidget(parent), fauxami(faux)
{
  setupUi(this);
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

  connect( bcount_line, SIGNAL(textChanged(const QString&)),  SLOT(slotBCount(const QString&)) );
  connect( qcount_line, SIGNAL(textChanged(const QString&)),  SLOT(slotQCount(const QString&)) );
  connect( fauxami_line, SIGNAL(textChanged(const QString&)), SLOT(slotFauxAmiSelected(const QString&)) );

  connect( never, SIGNAL(clicked()), SLOT(slotNever()) );
  connect( today, SIGNAL(clicked()), SLOT(slotToday()) );
  connect( gradebox, SIGNAL(activated(int)), SLOT(slotGradeSelected(int)) );

  connect( year_spin,  SIGNAL(valueChanged(int)), SLOT(slotYearChanged(int)) );
  connect( month_spin, SIGNAL(valueChanged(int)), SLOT(slotMonthChanged(int)) );
  connect( day_spin,   SIGNAL(valueChanged(int)), SLOT(slotDayChanged(int)) );

  year_spin->setData ((QStringList *) 0, 1980, 2100);
  month_spin->setData (&monthnames, 1, 12);
  day_spin->setData ((QStringList *) 0, 1, 31);

  QString s;
  for (int i = 0; i <= KV_MAX_GRADE; i++) {
    s.setNum (i);
    gradebox->insertItem( QueryManager::gradeStr(i) );
  }
  gradebox->setValidator (new BlockAllValidator() );

  setTabOrder(fauxami_line, year_spin);
  setTabOrder(year_spin, month_spin);
  setTabOrder(month_spin, day_spin);
  setTabOrder(day_spin, today);

  setData(multi_sel, _grade, _time, _qcount, _bcount, faux, label);
}


void FromToEntryPage::setData(
  bool        multi_sel,
  grade_t     _grade,
  time_t      _time,
  count_t     _qcount,
  count_t     _bcount,
  QString     faux,
  QString     label)
{
  grade = _grade;
  qcount = _qcount;
  bcount = _bcount;
  fauxami = faux;
  fauxami_line->setText(fauxami);

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
    year_spin->setSpecial(QString());
    month_spin->setSpecial(QString());
    day_spin->setSpecial(QString());
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

  gradebox->setCurrentItem (grade);

  QString s;
  s.setNum (qcount);
  qcount_line->setText (s);

  s.setNum (bcount);
  bcount_line->setText (s);

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

  setModified(false);
}


void FromToEntryPage::slotFauxAmiSelected(const QString& s)
{
  setModified(true);
  fauxami = s;
}


void FromToEntryPage::slotGradeSelected (int g)
{
  setModified(true);
  grade_dirty = true;
  grade = g;
}


void FromToEntryPage::slotQCount(const QString& s)
{
  setModified(true);
  qcount_dirty = true;
  qcount = atoi (s.local8Bit());
}


void FromToEntryPage::slotBCount(const QString& s)
{
  setModified(true);
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
  setModified(true);
  date_dirty = true;
  if (!valid_date) {
    slotToday();
    new_year = year;
  }

  year_spin->setSpecial(QString());

  year = new_year;
  validate();
}


void FromToEntryPage::slotDecYear()
{
  setModified(true);
}


// FIXME: dec month when day decrease below 1
void FromToEntryPage::slotIncYear()
{
  setModified(true);
}


void FromToEntryPage::slotMonthChanged(int new_month)
{
  setModified(true);
  date_dirty = true;
  if (!valid_date) {
    slotToday();
    new_month = month;
  }

  month_spin->setSpecial(QString());

  month = new_month;
  validate();
}


void FromToEntryPage::slotDecMonth()
{
  setModified(true);
}


void FromToEntryPage::slotIncMonth()
{
  setModified(true);
}


void FromToEntryPage::slotDayChanged(int new_day)
{
  setModified(true);
  date_dirty = true;
  if (!valid_date) {
    slotToday();
    new_day = day;
  }

  day_spin->setSpecial(QString());

  day = new_day;
  validate();
}


void FromToEntryPage::slotDecDay()
{
  setModified(true);
}


void FromToEntryPage::slotIncDay()
{
  setModified(true);
}


void FromToEntryPage::slotToday()
{
  setModified(true);
  date_dirty = true;
  QDateTime dt;
  dt.setTime_t (time(0L));

  year = dt.date().year();
  month = dt.date().month();
  day = dt.date().day();

  day_spin->setValue(day);
  month_spin->setValue(month);
  year_spin->setValue(year);
  year_spin->setSpecial(QString());
  month_spin->setSpecial(QString());
  day_spin->setSpecial(QString());
  validate();
}


void FromToEntryPage::slotNever()
{
  setModified(true);
  date_dirty = true;
  year = 0;
  month = 0;
  day = 0;
  year_spin->setSpecial("----");
  month_spin->setSpecial("----");
  day_spin->setSpecial("--");
  valid_date = false;
}


bool FromToEntryPage::isModified()
{
  return modified;
}


void FromToEntryPage::setEnabled(int enable)
{
  bool ena = enable == EntryDlg::EnableAll;

  bcount_line->setEnabled(ena);
  qcount_line->setEnabled(ena);
  fauxami_line->setEnabled(ena);

  never->setEnabled(ena);
  today->setEnabled(ena);
  gradebox->setEnabled(ena);

  year_spin->setEnabled(ena);
  month_spin->setEnabled(ena);
  day_spin->setEnabled(ena);
}


void FromToEntryPage::setModified(bool mod)
{
  modified = mod;
  if (mod)
    emit sigModified();
}

#include "FromToEntryPage.moc"

