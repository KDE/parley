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

    connect(fauxami_line, SIGNAL(textChanged(const QString&)), this, SLOT(slotFalseFriendChanged(const QString &)));

    connect(never, SIGNAL(clicked()), this, SLOT(slotNever()));
    connect(today, SIGNAL(clicked()), this, SLOT(slotToday()));
    connect(gradebox, SIGNAL(activated(int)), this, SLOT(slotGradeSelected(int)));

    connect(queryDateEdit, SIGNAL(dateChanged(const QDate &)), this, SLOT(slotDateChanged(const QDate &)));
    connect(totalCountEdit, SIGNAL(valueChanged(int)), this, SLOT(totalCountChanged(int)));
    connect(badCountEdit, SIGNAL(valueChanged(int)), this, SLOT(badCountChanged(int)));

    for (int i = 0; i <= KV_MAX_GRADE; i++)
        gradebox->addItem(KVTQuery::gradeStr(i));

}


void FromToEntryPage::slotFalseFriendChanged(const QString& s)
{
    emit sigModified();
}


void FromToEntryPage::slotGradeSelected(int g)
{
    emit sigModified();
}


void FromToEntryPage::slotToday()
{
    queryDateEdit->setDateTime(QDateTime::currentDateTime());
    emit sigModified();
}


void FromToEntryPage::slotNever()
{
    queryDateEdit->setDate(queryDateEdit->minimumDate());
    queryDateEdit->setTime(queryDateEdit->minimumTime());
    emit sigModified();
}


bool FromToEntryPage::isModified()
{
    if ( m_currentRow < 0 || m_translationFrom < 0 || m_translationTo < 0 ) {
        return false;
    }

    KEduVocExpression *entry = m_doc->entry(m_currentRow);

    kDebug() << "FromToEntryPage::isModified(): entry: " << m_currentRow << " from " << m_translationFrom << " to: " << m_translationTo;

    if ( m_selection.count() == 1 ) {
        if( fauxami_line->text() != entry->translation( m_translationTo ).falseFriend( m_translationFrom ) ) {
            return true;
        }
    }

    if ( entry->translation(m_translationTo).gradeFrom(m_translationFrom).grade()
            != gradebox->currentIndex() ) {
        return true;
    }

    if ( entry->translation(m_translationTo).gradeFrom(m_translationFrom).queryCount()
            != totalCountEdit->value() ) {
        return true;
    }

    if ( entry->translation(m_translationTo).gradeFrom(m_translationFrom).badCount()
            != badCountEdit->value() ) {
        return true;
    }

    return false;
/// @todo check other fields!!!


//queryDateEdit->dateTime();
    /*
if ( time.toTime_t() != 0 ) {
            valid_date = true;
            queryDateEdit->setDateTime(time);
        }
    queryDateEdit->setDateTime(QDateTime());
    QDateTime time = m_doc->entry(m_currentRow)
        ->translation(m_translationTo).gradeFrom(m_translationFrom).queryDate();

*/
}


void FromToEntryPage::slotDateChanged(const QDate & d)
{
    emit sigModified();
}

void FromToEntryPage::totalCountChanged(int count)
{
    emit sigModified();
}

void FromToEntryPage::badCountChanged(int count)
{
    emit sigModified();
}


void FromToEntryPage::setData(int row, int toTrans, int fromTrans, const QModelIndexList & selection )
{
    m_currentRow = row;
    m_translationFrom = fromTrans;
    m_translationTo = toTrans;
    m_selection = selection;

    // only set Grades as title for now:
    QString label = QString(i18n("Grades"));
    direc_label->setTitle(label);

    KEduVocExpression *entry = m_doc->entry(row);

    queryDateEdit->setDateTime(
        m_doc->entry(m_currentRow)->translation(m_translationTo)
        .gradeFrom(m_translationFrom).queryDate() );

    if ( m_selection.count() > 1 ) {
        fauxami_line->setEnabled(false);
        fauxami_line->setText(QString());
    } else {
        fauxami_line->setEnabled(true);
        fauxami_line->setText(entry->translation( m_translationTo ).falseFriend( m_translationFrom ) );
    }

    gradebox->setCurrentIndex(m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).grade());

    totalCountEdit->setValue(m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).queryCount());

    badCountEdit->setValue(m_doc->entry(m_currentRow)->translation(m_translationTo).gradeFrom(m_translationFrom).badCount());

}

void FromToEntryPage::commitData()
{
    if ( m_currentRow < 0 || m_translationFrom < 0 || m_translationTo < 0 ) {
        kDebug() << "FromToEntryPage::commitData() invalid data to commit: " << m_currentRow << ", " << m_translationFrom << ", " << m_translationTo;
    }

/// @todo enable writing of data for multiple selection
    KEduVocTranslation * trans = &m_doc->entry(m_currentRow)->translation(m_translationTo);

    trans->setFalseFriend(m_translationFrom, fauxami_line->text());
    trans->gradeFrom(m_translationFrom).setGrade( gradebox->currentIndex() );
    trans->gradeFrom(m_translationFrom).setQueryCount( totalCountEdit->value() );
    trans->gradeFrom(m_translationFrom).setBadCount( badCountEdit->value() );
    trans->gradeFrom(m_translationFrom).setQueryDate( queryDateEdit->dateTime() );

/*
    Q_UNUSED(d);
    m_dateIsModified = true;
    setModified(true);
    if (!valid_date)
        slotToday();

    validate();
*/

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

