/***************************************************************************

                    base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Wed Feb 16 20:50:53 MET 2000

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include <QCloseEvent>

#include <klocale.h>

#include "QueryDlgBase.h"
#include <LineList.h>

QueryDlgBase::QueryDlgBase(const QString & caption, QWidget *parent, const char *name, bool modal)
  : KDialogBase(parent, name, modal, caption, User1|User2, NoDefault, false,
    KGuiItem(i18n("&Stop Query")), KGuiItem(i18n("&Edit Expression...")))
{
  kv_doc = 0;
  kv_exp = 0;
  //type_timeout = kvq_notimeout;
}


QueryDlgBase::~QueryDlgBase ()
{
}


void QueryDlgBase::initFocus() const
{
  //
}


bool QueryDlgBase::smartCompare (const QString& s1, const QString &s2,
                                 int ) const
{
  return s1.simplified() == s2.simplified();
}


bool QueryDlgBase::verifyField(QLineEdit *field, const QString &really)
{
  if (!field->isEnabled() )
    return true;

  QPalette u_normal = field->palette();
  u_normal.setColor(QPalette::Text, QColor(0xff, 0x00, 0x00));
  QPalette k_normal = field->palette();
  k_normal.setColor(QPalette::Text, QColor(0x00, 0x80, 0x00));

  QPalette known_pal( field->palette());
  QPalette unknown_pal( field->palette());

  QFont ft = field->font();
  if (ft.weight() != QFont::Bold) {
    ft.setWeight(QFont::Bold);
    field->setFont(ft);
  }

  bool ret = false;
  if (smartCompare(really, field->text(), 0) ) {
    ret = true;
    if ( known_pal.inactive() != k_normal
        || known_pal.active() != k_normal) {
      // replace text colors
      known_pal.setActive(k_normal);
      known_pal.setInactive(k_normal);
      field->setPalette( known_pal );
    }
  }
  else
    if ( unknown_pal.inactive() != u_normal
        || unknown_pal.active() != u_normal) {
      // replace text colors
      unknown_pal.setActive(u_normal);
      unknown_pal.setInactive(u_normal);
      field->setPalette( unknown_pal );
    }
  return ret;
}


void QueryDlgBase::resetField(QLineEdit *field)
{
  if (!field->isEnabled() )
    return;
  QPalette normal = field->palette();
  normal.setColor(QPalette::Text, QColor(0x00, 0x00, 0x00));

  QPalette pal( field->palette());
  // replace text colors

  if ( pal.inactive() != normal
      || pal.active() != normal) {
    pal.setActive(normal);
    pal.setInactive(normal);
    field->setPalette( pal );
  }

  QFont ft = field->font();
  if (ft.weight() != QFont::Normal) {
    ft.setWeight(QFont::Normal);
    field->setFont(ft);
  }
}


bool QueryDlgBase::verifyField(QTextEdit *field, const QString &really, bool mixed)
{
  if (!field->isEnabled())
    return true;
  QPalette u_normal = field->palette();
  u_normal.setColor(QPalette::Text, QColor(0xff, 0x00, 0x00));
  QPalette k_normal = field->palette();
  k_normal.setColor(QPalette::Text, QColor(0x00, 0x80, 0x00));

  QPalette known_pal( field->palette());
  QPalette unknown_pal( field->palette());

  QFont ft = field->font();
  if (ft.weight() != QFont::Bold) {
    ft.setWeight(QFont::Bold);
    field->setFont(ft);
  }

  bool ret = false;
  bool equal = false;
  LineList answerlist (really);
  LineList inputlist (field->text());
  if (!mixed) // no tolerance
    equal = smartCompare(answerlist.allLines(), inputlist.allLines(), 0);
  else {
     bool all = true;
     for (int ai = 0; ai < (int) answerlist.count(); ai++) {
       bool found = false;
       for (int ii = 0; ii < (int) inputlist.count(); ii++) {
         if (answerlist.getLine (ai) == inputlist.getLine(ii) ) {
           found = true;
           break;
         }
       }
       if (!found)
         all = false;
     }
     if (all)
       equal = true;
  }

  if (equal) {
    ret = true;
    if (known_pal.inactive() != k_normal
        || known_pal.active() != k_normal) {
      // replace text colors
      known_pal.setActive(k_normal);
      known_pal.setInactive(k_normal);
      field->setPalette( known_pal );
    }
  }
  else
    if ( unknown_pal.inactive() != u_normal
        || unknown_pal.active() != u_normal) {
      // replace text colors
      unknown_pal.setActive(u_normal);
      unknown_pal.setInactive(u_normal);
      field->setPalette( unknown_pal );
    }
  return ret;
}


void QueryDlgBase::resetField(QTextEdit *field)
{
  if (!field->isEnabled() )
    return;
  QPalette normal = field->palette();
  normal.setColor(QPalette::Text, QColor(0x00, 0x00, 0x00));

  QPalette pal( field->palette());
  // replace text colors

  if ( pal.inactive() != normal
      || pal.active() != normal) {
    pal.setActive(normal);
    pal.setInactive(normal);
    field->setPalette( pal );
  }

  QFont ft = field->font();
  if (ft.weight() != QFont::Normal) {
    ft.setWeight(QFont::Normal);
    field->setFont(ft);
  }
}


void QueryDlgBase::verifyButton(QRadioButton *radio, bool is_ok, QWidget *widget2)
{
  if (!radio->isEnabled() )
    return;

  QPalette u_normal = radio->palette();
  u_normal.setColor(QPalette::Foreground, QColor(0xff, 0x00, 0x00));
  QPalette k_normal = radio->palette();
  k_normal.setColor(QPalette::Foreground, QColor(0x00, 0x80, 0x00));

  QPalette known_pal( radio->palette());
  QPalette unknown_pal( radio->palette());

  // replace text colors

  QFont ft = radio->font();
  if (ft.weight() != QFont::Bold) {
    ft.setWeight(QFont::Bold);
    radio->setFont(ft);
  }

  if (widget2 != 0) {
    ft = widget2->font();
    if (ft.weight() != QFont::Bold) {
      ft.setWeight(QFont::Bold);
      widget2->setFont(ft);
    }
  }

  if (is_ok) {
    if ( known_pal.inactive() != k_normal
        || known_pal.active() != k_normal) {
      known_pal.setActive(k_normal);
      known_pal.setInactive(k_normal);
      radio->setPalette( known_pal );
      if (widget2 != 0)
        widget2->setPalette( known_pal );
    }
  }
  else {
    if ( unknown_pal.inactive() != u_normal
        || unknown_pal.active() != u_normal) {
      unknown_pal.setActive(u_normal);
      unknown_pal.setInactive(u_normal);
      radio->setPalette( unknown_pal );
      if (widget2 != 0)
        widget2->setPalette( unknown_pal );
    }
  }
}



void QueryDlgBase::resetButton(QRadioButton *radio, QWidget *widget2)
{
  if (!radio->isEnabled() )
    return;
  QPalette normal = radio->palette();
  normal.setColor(QPalette::Foreground, QColor(0x00, 0x00, 0x00));

  QPalette pal(radio->palette());
  // replace text colors, avoid flickering
  if ( pal.inactive() != normal
      || pal.active() != normal) {
    pal.setActive(normal);
    pal.setInactive(normal);
    radio->setPalette( pal );
    if (widget2 != 0)
      widget2->setPalette( pal );
  }

  QFont ft = radio->font();
  if (ft.weight() != QFont::Normal) {
    ft.setWeight(QFont::Normal);
    radio->setFont(ft);
  }

  if (widget2 != 0) {
    ft = widget2->font();
    if (ft.weight() != QFont::Normal) {
      ft.setWeight(QFont::Normal);
      widget2->setFont(ft);
    }
  }
}


QString  QueryDlgBase::getOKComment(int percent_done)
{
  return i18n("Well done, you knew the correct answer. %1% done.", percent_done);
}


QString  QueryDlgBase::getTimeoutComment(int percent_done)
{
  return i18n("You waited too long to enter the correct answer. %1% done.", percent_done);
}


QString  QueryDlgBase::getNOKComment(int percent_done)
{
  return i18n("Your answer was wrong. %1% done.", percent_done);
}

void QueryDlgBase::closeEvent(QCloseEvent * /*e*/)
{
  emit sigQueryChoice(StopIt);
}

void QueryDlgBase::slotUser1()
{
  emit sigQueryChoice(StopIt);
}


#include "QueryDlgBase.moc"
