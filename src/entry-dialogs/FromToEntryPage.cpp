/***************************************************************************

                     edit properties from/to original

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "practice/kvtquery.h"
#include "EntryDlg.h"

FromToEntryPage::FromToEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
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

    connect(resetGradesButton, SIGNAL(clicked()), SLOT(slotResetGrades()));

    for (int i = 0; i <= KV_MAX_GRADE; i++) {
        gradebox->addItem(TestEntryManager::gradeStr(i));
    }

}


void FromToEntryPage::slotFalseFriendChanged(const QString& s)
{
    Q_UNUSED(s)
    emit sigModified();
}


void FromToEntryPage::slotGradeSelected(int g)
{
    m_gradeChanged=true;
    Q_UNUSED(g)
    emit sigModified();
}


void FromToEntryPage::slotToday()
{
    m_practiceDateChanged=true;
    queryDateEdit->setDateTime(QDateTime::currentDateTime());
    emit sigModified();
}


void FromToEntryPage::slotNever()
{
    m_practiceDateChanged=true;
    queryDateEdit->setDate(queryDateEdit->minimumDate());
    queryDateEdit->setTime(queryDateEdit->minimumTime());
    emit sigModified();
}


bool FromToEntryPage::isModified()
{
    if ( m_entries.value(0) < 0 || m_translationFrom < 0 || m_translationTo < 0 ) {
        return false;
    }

    KEduVocExpression *entry = m_doc->entry(m_entries.value(0));

    if ( m_entries.count() == 1 ) {
        if( fauxami_line->text() != entry->translation( m_translationTo ).falseFriend( m_translationFrom ) ) {
            return true;
        }
    }

    if ( m_gradeChanged || m_totalCountChanged || m_wrongCountChanged || m_practiceDateChanged ) {
        return true;
    }

    return false;
}


void FromToEntryPage::slotDateChanged(const QDate & d)
{
    m_practiceDateChanged=true;
    Q_UNUSED(d)
    emit sigModified();
}

void FromToEntryPage::totalCountChanged(int count)
{
    m_totalCountChanged=true;
    Q_UNUSED(count)
    emit sigModified();
}

void FromToEntryPage::badCountChanged(int count)
{
    m_wrongCountChanged=true;
    Q_UNUSED(count)
    emit sigModified();
}


void FromToEntryPage::setData(const QList<int>& entries, int toTrans, int fromTrans)
{
    m_translationFrom = fromTrans;
    m_translationTo = toTrans;
    m_entries = entries;

    // only set Grades as title for now:
    QString label = QString(i18n("Grades"));
    direc_label->setTitle(label);

    KEduVocExpression *firstEntry = m_doc->entry(m_entries.value(0));

    queryDateEdit->setDateTime(
        firstEntry->translation(m_translationTo)
        .gradeFrom(m_translationFrom).queryDate() );

    gradebox->setCurrentIndex(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).grade());

    totalCountEdit->setValue(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).queryCount());

    badCountEdit->setValue(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).badCount());

    if ( m_entries.count() > 1 ) {
        // fill enabled fields if equal for all edited entries, otherwise empty.
        foreach ( int entry, m_entries) {
            // grade
            KEduVocExpression *currentEntry = m_doc->entry(entry);
            if ( firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).grade()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).grade() ) {
                gradebox->setCurrentIndex(-1);
            }
            // date
            if ( firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).queryDate()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).queryDate() ) {
                queryDateEdit->setDateTime(QDateTime());
            }

            // total count
            if ( firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).queryCount()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).queryCount() ) {
                totalCountEdit->clear();
            }

            // wrong count
            if ( firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).badCount()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).badCount() ) {
                badCountEdit->clear();
            }

        }
        fauxami_line->setEnabled(false);
        fauxami_line->setText(QString());
    } else {
        fauxami_line->setEnabled(true);
        fauxami_line->setText(firstEntry->translation( m_translationTo ).falseFriend( m_translationFrom ) );
    }

    m_gradeChanged=false;
    m_practiceDateChanged=false;
    m_totalCountChanged=false;
    m_wrongCountChanged=false;

}

void FromToEntryPage::commitData()
{
    if ( m_entries.value(0) < 0 || m_translationFrom < 0 || m_translationTo < 0 ) {
        kDebug() << "FromToEntryPage::commitData() invalid data to commit: " << m_entries.value(0) << ", " << m_translationFrom << ", " << m_translationTo;
    }

    if ( m_entries.count() == 1 ) {
    // these things are only changed when editing a single entry
        KEduVocTranslation * trans = &m_doc->entry(m_entries.value(0))->translation(m_translationTo);
        trans->setFalseFriend(m_translationFrom, fauxami_line->text());
    }

    // things that are changed for multiple or single entries
    foreach(int entry, m_entries) {
        if ( m_gradeChanged ) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setGrade( gradebox->currentIndex() );
        }
        if ( m_totalCountChanged ) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setQueryCount( totalCountEdit->value() );
        }
        if ( m_wrongCountChanged ) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setBadCount( badCountEdit->value() );
        }
        if ( m_practiceDateChanged ) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setQueryDate( queryDateEdit->dateTime() );
        }
    }
}

void FromToEntryPage::slotResetGrades()
{
    queryDateEdit->setDate(queryDateEdit->minimumDate());
    queryDateEdit->setTime(queryDateEdit->minimumTime());

    gradebox->setCurrentIndex(KV_MIN_GRADE);
    totalCountEdit->setValue(0);
    badCountEdit->setValue(0);

    m_gradeChanged=true;
    m_practiceDateChanged=true;
    m_totalCountChanged=true;
    m_wrongCountChanged=true;
}


#include "FromToEntryPage.moc"

