/*
    edit properties from/to original
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "FromToEntryPage.h"

#include <QPushButton>
#include <QGroupBox>

#include <KLocalizedString>
#include <KComboBox>
#include <QLineEdit>

#include "EntryDlg.h"


// this has nothing really to do with the rest. stays here until it has a better home.
QString FromToEntryPage::gradeToString(int i)
{
    switch (i) {
    case KV_NORM_GRADE:
        return i18n(KV_NORM_TEXT);
        break;
    case KV_LEV1_GRADE:
        return i18n(KV_LEV1_TEXT);
        break;
    case KV_LEV2_GRADE:
        return i18n(KV_LEV2_TEXT);
        break;
    case KV_LEV3_GRADE:
        return i18n(KV_LEV3_TEXT);
        break;
    case KV_LEV4_GRADE:
        return i18n(KV_LEV4_TEXT);
        break;
    case KV_LEV5_GRADE:
        return i18n(KV_LEV5_TEXT);
        break;
    case KV_LEV6_GRADE:
        return i18n(KV_LEV6_TEXT);
        break;
    case KV_LEV7_GRADE:
        return i18n(KV_LEV7_TEXT);
        break;
    default:
        return i18n(KV_LEV1_TEXT);
        break;
    }
}


FromToEntryPage::FromToEntryPage(KEduVocDocument *doc, int fromIdentifier, int toIdentifier, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;

    m_translationFrom = fromIdentifier;
    m_translationTo = toIdentifier;

    setupUi(this);

    connect(fauxami_line, &QLineEdit::textChanged, this, &FromToEntryPage::slotFalseFriendChanged);

    connect(never, &QPushButton::clicked, this, &FromToEntryPage::slotNever);
    connect(today, &QPushButton::clicked, this, &FromToEntryPage::slotToday);
    connect(gradebox, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &FromToEntryPage::slotGradeSelected);

    connect(queryDateEdit, &QDateEdit::dateChanged, this, &FromToEntryPage::slotDateChanged);
    connect(totalCountEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FromToEntryPage::totalCountChanged);
    connect(badCountEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FromToEntryPage::badCountChanged);

    connect(resetGradesButton, &QPushButton::clicked, this, &FromToEntryPage::slotResetGrades);

    for (int i = 0; i <= KV_MAX_GRADE; i++) {
        gradebox->addItem(gradeToString(i));
    }

}


void FromToEntryPage::slotFalseFriendChanged(const QString& s)
{
    Q_UNUSED(s)
    emit sigModified();
}


void FromToEntryPage::slotGradeSelected(int g)
{
    m_gradeChanged = true;
    Q_UNUSED(g)
    emit sigModified();
}


void FromToEntryPage::slotToday()
{
    m_practiceDateChanged = true;
    queryDateEdit->setDateTime(QDateTime::currentDateTime());
    emit sigModified();
}


void FromToEntryPage::slotNever()
{
    m_practiceDateChanged = true;
    queryDateEdit->setDate(queryDateEdit->minimumDate());
    queryDateEdit->setTime(queryDateEdit->minimumTime());
    emit sigModified();
}


void FromToEntryPage::slotDateChanged(const QDate & d)
{
    m_practiceDateChanged = true;
    Q_UNUSED(d)
    emit sigModified();
}

void FromToEntryPage::totalCountChanged(int count)
{
    m_totalCountChanged = true;
    Q_UNUSED(count)
    emit sigModified();
}

void FromToEntryPage::badCountChanged(int count)
{
    m_wrongCountChanged = true;
    Q_UNUSED(count)
    emit sigModified();
}


void FromToEntryPage::setData(const QList<int>& entries)
{
    m_entries = entries;

    // only set Grades as title for now:
    direc_label->setTitle(QString(i18n("Confidence Levels from %1 to %2", m_doc->identifier(m_translationFrom).name(), m_doc->identifier(m_translationTo).name())));

    KEduVocExpression *firstEntry = m_doc->entry(m_entries.value(0));

    queryDateEdit->setDateTime(
        firstEntry->translation(m_translationTo)
        .gradeFrom(m_translationFrom).practiceDate());

    gradebox->setCurrentIndex(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).grade());

    totalCountEdit->setValue(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).practiceCount());

    badCountEdit->setValue(firstEntry->translation(m_translationTo).gradeFrom(m_translationFrom).badCount());

    if (m_entries.count() > 1) {
        // fill enabled fields if equal for all edited entries, otherwise empty.
        foreach(int entry, m_entries) {
            // grade
            KEduVocExpression *currentEntry = m_doc->entry(entry);
            if (firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).grade()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).grade()) {
                gradebox->setCurrentIndex(-1);
            }
            // date
            if (firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).practiceDate()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).practiceDate()) {
                queryDateEdit->setDate(queryDateEdit->minimumDate());
                queryDateEdit->setTime(queryDateEdit->minimumTime());
            }

            // total count
            if (firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).practiceCount()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).practiceCount()) {
                totalCountEdit->clear();
            }

            // wrong count
            if (firstEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).badCount()
                    != currentEntry->translation(m_translationTo)
                    .gradeFrom(m_translationFrom).badCount()) {
                badCountEdit->clear();
            }

        }
        fauxami_line->setEnabled(false);
        fauxami_line->setText(QString());
    } else {
        fauxami_line->setEnabled(true);
        fauxami_line->setText(firstEntry->translation(m_translationTo).falseFriend(m_translationFrom));
    }

    m_gradeChanged = false;
    m_practiceDateChanged = false;
    m_totalCountChanged = false;
    m_wrongCountChanged = false;

}

void FromToEntryPage::commitData()
{
    qDebug() << "Grade page commit data: " << m_translationFrom << m_translationTo;
    if (m_entries.count() == 1) {
        // these things are only changed when editing a single entry
        KEduVocTranslation * trans = &m_doc->entry(m_entries.value(0))->translation(m_translationTo);
        trans->setFalseFriend(m_translationFrom, fauxami_line->text());
    }

    // things that are changed for multiple or single entries
    foreach(int entry, m_entries) {
        if (m_gradeChanged) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setGrade(gradebox->currentIndex());
        }
        if (m_totalCountChanged) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setPracticeCount(totalCountEdit->value());
        }
        if (m_wrongCountChanged) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setBadCount(badCountEdit->value());
        }
        if (m_practiceDateChanged) {
            m_doc->entry(entry)->translation(m_translationTo).gradeFrom(m_translationFrom).setPracticeDate(queryDateEdit->dateTime());
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

    m_gradeChanged = true;
    m_practiceDateChanged = true;
    m_totalCountChanged = true;
    m_wrongCountChanged = true;

//     emit sigModified();
}



