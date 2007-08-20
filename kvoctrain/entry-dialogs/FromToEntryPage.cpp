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

#include "FromToEntryPage.h"

#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>

#include <klocale.h>

#include "query-dialogs/kvtquery.h"
#include <kvtlanguages.h>
#include "EntryDlg.h"

FromToEntryPage::FromToEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_currentRow = -1;
    m_translationFrom = -1;
    m_translationTo = -1;

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


void FromToEntryPage::setData(bool multi_sel, grade_t _grade, const QDateTime &_time, count_t _qcount, count_t _bcount, const QString &faux, const QString &label)
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
    } else {
        fauxami_line->setEnabled(true);
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


void FromToEntryPage::setData(int row, int toTrans, int fromTrans, const QModelIndexList & selection )
{
    m_currentRow = row;
    m_translationFrom = fromTrans;
    m_translationTo = toTrans;
    m_selection = selection;

    KEduVocExpression *entry = m_doc->entry(row);

    queryDateEdit->setDateTime(QDateTime());
    QDateTime time = m_doc->entry(m_currentRow)
        ->translation(m_translationTo).gradeFrom(m_translationFrom).queryDate();

    if ( m_selection.count() > 1 ) {
        fauxami_line->setEnabled(false);
        fauxami_line->setText(QString());
    } else {
        fauxami_line->setEnabled(true);
        fauxami_line->setText(entry->translation( m_translationTo ).falseFriend( m_translationFrom ) );
        if ( time.toTime_t() != 0 ) {
            valid_date = true;
            queryDateEdit->setDateTime(time);
        }
    }

    grade = m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).grade();

    qcount = m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).queryCount();

    bcount = m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).badCount();

    gradebox->setCurrentIndex(grade);
    totalCountEdit->setValue(qcount);
    badCountEdit->setValue(bcount);


    QString label = QString(i18n("Grades"));
    direc_label->setTitle(label);

    setModified(false);
    m_gradeIsModified = false;
    m_queryCountIsModified = false;
    m_badCountIsModified = false;
    m_dateIsModified = false;
}

void FromToEntryPage::commitData()
{
/// @todo enable writing of data for multiple selection
    KEduVocTranslation * trans = &m_doc->entry(m_currentRow)->translation(m_translationTo);

    trans->setFalseFriend(m_translationFrom, getFauxAmi());
    trans->gradeFrom(m_translationFrom).setGrade( getGrade() );
    trans->gradeFrom(m_translationFrom).setQueryCount( getQCount() );
    trans->gradeFrom(m_translationFrom).setBadCount( getBCount() );
    trans->gradeFrom(m_translationFrom).setQueryDate( getDate() );

// } else {
//         foreach(QModelIndex selIndex, m_selection) {
//             //QModelIndex index = m_sortFilterModel->mapToSource(selIndex);
//             KEduVocExpression *expr = m_doc->entry(m_currentRow);
//
//             if (m_currentTranslation >= 0) {
//                 // only updated "common" props in multimode
//                 // is the modified necessary? maybe because it can be different and will only be saved if the user changes it. otherwise it should stay different probably. so maybe leave the modified stuff in here.
//                 if (fromPage->gradeIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setGrade(fromPage->getGrade());
//                 if (toPage->gradeIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setGrade(toPage->getGrade());
//                 if (fromPage->queryCountIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setQueryCount(fromPage->getQCount());
//                 if (toPage->queryCountIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setQueryCount(toPage->getQCount());
//                 if (fromPage->badCountIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setBadCount(fromPage->getBCount());
//                 if (toPage->badCountIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setBadCount(toPage->getBCount());
//                 if (fromPage->dateIsModified())
//                     expr->translation(m_currentTranslation).gradeFrom(0).setQueryDate(fromPage->getDate());
//                 if (toPage->dateIsModified())
//                     expr->translation(0).gradeFrom(m_currentTranslation).setQueryDate( toPage->getDate());
//             }
//         }

}

void FromToEntryPage::clear()
{
}


#include "FromToEntryPage.moc"

