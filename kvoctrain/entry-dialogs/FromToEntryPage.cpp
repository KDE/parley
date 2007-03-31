/***************************************************************************

                     edit properties from/to original

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <kvtquery.h>
#include <kvtlanguages.h>
#include "FromToEntryPage.h"
#include "EntryDlg.h"

FromToEntryPage::FromToEntryPage(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(fauxami_line, SIGNAL(textChanged(const QString&)), this, SLOT(slotFauxAmiChanged(const QString &)));

    connect(never, SIGNAL(clicked()), this, SLOT(slotNever()));
    connect(today, SIGNAL(clicked()), this, SLOT(slotToday()));
    connect(gradebox, SIGNAL(activated(int)), this, SLOT(slotGradeSelected(int)));

    connect(queryDateEdit, SIGNAL(dateChanged(const QDate &)), this, SLOT(slotDateChanged(const QDate &)));
    connect(totalCountEdit, SIGNAL(valueChanged(int)), this, SLOT(totalCountChanged(int)));
    connect(badCountEdit, SIGNAL(valueChanged(int)), this, SLOT(badCountChanged(int)));

    for (int i = 0; i <= KV_MAX_GRADE; i++)
        gradebox->addItem(KVTQuery::gradeStr(i));

}


void FromToEntryPage::setData(bool multi_sel, grade_t _grade, QDateTime _time, count_t _qcount, count_t _bcount, QString faux, QString label)
{
    grade = _grade;
    qcount = _qcount;
    bcount = _bcount;
    fauxami = faux;
    fauxami_line->setText(fauxami);

    valid_date = false;

    if (_time.toTime_t() != 0 && !multi_sel) {
        valid_date = true;
        queryDateEdit->setDateTime(_time);
    } else
        queryDateEdit->setDateTime(QDateTime());

    direc_label->setTitle(label);

    gradebox->setCurrentIndex(grade);

    totalCountEdit->setValue(qcount);
    badCountEdit->setValue(bcount);

    m_largeSelection = multi_sel;
    if (m_largeSelection) {
        fauxami_line->setEnabled(false);
        valid_date = false;
    }

    setModified(false);
    m_gradeIsModified = false;
    m_queryCountIsModified = false;
    m_badCountIsModified = false;
    m_dateIsModified = false;
}


void FromToEntryPage::slotFauxAmiChanged(const QString& s)
{
    setModified(true);
    fauxami = s;
}


void FromToEntryPage::slotGradeSelected(int g)
{
    setModified(true);
    m_gradeIsModified = true;
    grade = g;
}


QDateTime FromToEntryPage::getDate() const
{
    if (valid_date)
        return queryDateEdit->dateTime();
    else
        return QDateTime();
}


void FromToEntryPage::validate()
{
    if (!valid_date)
        valid_date = true;
}


void FromToEntryPage::slotToday()
{
    setModified(true);
    m_dateIsModified = true;
    queryDateEdit->setDateTime(QDateTime::currentDateTime());
    validate();
}


void FromToEntryPage::slotNever()
{
    setModified(true);
    m_dateIsModified = true;
    queryDateEdit->setDate(queryDateEdit->minimumDate());
    queryDateEdit->setTime(queryDateEdit->minimumTime());
    valid_date = false;
}


bool FromToEntryPage::isModified()
{
    return modified;
}


void FromToEntryPage::setEnabled(int enable)
{
    bool ena = enable == EntryDlg::EnableAll;

    totalCountEdit->setEnabled(ena);
    badCountEdit->setEnabled(ena);
    fauxami_line->setEnabled(ena && !m_largeSelection);

    never->setEnabled(ena);
    today->setEnabled(ena);
    gradebox->setEnabled(ena);

    queryDateEdit->setEnabled(ena);
}


void FromToEntryPage::setModified(bool mod)
{
    modified = mod;
    if (mod)
        emit sigModified();
}

void FromToEntryPage::slotDateChanged(const QDate & d)
{
    Q_UNUSED(d);
    m_dateIsModified = true;
    setModified(true);
    if (!valid_date)
        slotToday();

    validate();
}

void FromToEntryPage::totalCountChanged(int count)
{
    setModified(true);
    m_queryCountIsModified = true;
    qcount = count;
}

void FromToEntryPage::badCountChanged(int count)
{
    setModified(true);
    m_badCountIsModified = true;
    bcount = count;
}

#include "FromToEntryPage.moc"
