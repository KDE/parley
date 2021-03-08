/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2016 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "entryfilterdialog.h"

#include "entryfilter.h"
#include "ui_entryfilterdialog.h"
#include <QPushButton>

EntryFilterDialog::EntryFilterDialog(EntryFilter &entryFilter)
    : QDialog()
    , m_ui(new Ui::EntryFilterDialog)
    , m_entryFilter(entryFilter)
{
    m_ui->setupUi(this);

    setupEntryCounts();
    setupCheckBoxes();
    setupConnections();
    updateTotal();

    if (!Prefs::wordTypesInPracticeEnabled()) {
        m_ui->wordTypeLabel->setVisible(false);
        m_ui->wordTypeCheckBox->setVisible(false);
    }
}

EntryFilterDialog::~EntryFilterDialog()
{
}

bool EntryFilterDialog::ignoreBlocked() const
{
    return m_ui->blockedCheckBox->isChecked();
}

void EntryFilterDialog::setupEntryCounts()
{
    int numEntries = m_entryFilter.m_entries[0].count() + m_entryFilter.m_entries[1].count();

    m_ui->lessonLabel->setText(
        QString::number(
            numEntries
            - m_entryFilter.m_entriesLesson[0].count()
            - m_entryFilter.m_entriesLesson[1].count()
        )
    );
    m_ui->wordTypeLabel->setText(
        QString::number(
            numEntries
            - m_entryFilter.m_entriesWordType[0].count()
            - m_entryFilter.m_entriesWordType[1].count()
        )
    );
    m_ui->blockedLabel->setText(
        QString::number(
            numEntries
            - m_entryFilter.m_entriesNotBlocked[0].count()
            - m_entryFilter.m_entriesNotBlocked[1].count()
        )
    );
    m_ui->timesWrongLabel->setText(
        QString::number(
            numEntries
            - m_entryFilter.m_entriesTimesWrong[0].count()
            - m_entryFilter.m_entriesTimesWrong[1].count()
        )
    );
    m_ui->timesPracticedLabel->setText(
        QString::number(numEntries
            - m_entryFilter.m_entriesTimesPracticed[0].count()
            - m_entryFilter.m_entriesTimesPracticed[1].count()
        )
    );
    m_ui->minMaxGradeLabel->setText(
        QString::number(numEntries
            - m_entryFilter.m_entriesMinMaxGrade[0].count()
            - m_entryFilter.m_entriesMinMaxGrade[1].count()
        )
    );
    m_ui->documentTotalLabel->setText(QString::number(numEntries));
}

void EntryFilterDialog::setupCheckBoxes()
{
    m_ui->lessonCheckBox->setChecked(
        (m_entryFilter.m_entriesLesson[0].count()
        + m_entryFilter.m_entriesLesson[1].count()) == 0);

    m_ui->wordTypeCheckBox->setChecked(
        (m_entryFilter.m_entriesWordType[0].count()
        + m_entryFilter.m_entriesWordType[1].count()) == 0);

    m_ui->blockedCheckBox->setChecked(
        (m_entryFilter.m_entriesNotBlocked[0].count()
        + m_entryFilter.m_entriesNotBlocked[1].count()) == 0);

    m_ui->timesWrongCheckBox->setChecked(
        (m_entryFilter.m_entriesTimesWrong[0].count()
        + m_entryFilter.m_entriesTimesWrong[1].count()) == 0);

    m_ui->timesPracticedCheckBox->setChecked(
        (m_entryFilter.m_entriesTimesPracticed[0].count()
        + m_entryFilter.m_entriesTimesPracticed[1].count()) == 0);

    m_ui->minMaxGradeCheckBox->setChecked(
        (m_entryFilter.m_entriesMinMaxGrade[0].count()
        + m_entryFilter.m_entriesMinMaxGrade[1].count()) == 0);
}

void EntryFilterDialog::updateTotal()
{
    for (int i = 0; i < m_entryFilter.m_numSets; ++i) {
        // Introduce reference to current selection here to make code better readable.
        QSet<KEduVocExpression *> &currentSet = m_entryFilter.m_currentSelection[i];

        currentSet = m_entryFilter.m_entries[i];
        if (!m_ui->lessonCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesLesson[i]);
        }
        if (!m_ui->wordTypeCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesWordType[i]);
        }
        if (!m_ui->blockedCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesNotBlocked[i]);
        }
        if (!m_ui->timesWrongCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesTimesWrong[i]);
        }
        if (!m_ui->timesPracticedCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesTimesPracticed[i]);
        }
        if (!m_ui->minMaxGradeCheckBox->isChecked()) {
            currentSet = currentSet.intersect(m_entryFilter.m_entriesMinMaxGrade[i]);
        }
    }

    int numSelected = m_entryFilter.m_currentSelection[0].count()
                    + m_entryFilter.m_currentSelection[1].count();
    m_ui->totalLabel->setText(QString::number(numSelected));
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(numSelected > 0);
}

void EntryFilterDialog::setupConnections()
{
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_ui->lessonCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
    connect(m_ui->wordTypeCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
    connect(m_ui->blockedCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
    connect(m_ui->timesWrongCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
    connect(m_ui->timesPracticedCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
    connect(m_ui->minMaxGradeCheckBox, &QAbstractButton::toggled, this, &EntryFilterDialog::updateTotal);
}

