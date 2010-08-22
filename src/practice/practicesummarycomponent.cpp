/***************************************************************************
    Copyright 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicesummarycomponent.h"
#include "prefs.h"

#include "parleyactions.h"

#include <QTableWidgetItem>
#include <keduvocexpression.h>
#include <KConfigGroup>
#include <KActionCollection>
#include <KColorScheme>
#include <KToolBar>

using namespace Practice; 

class PracticeSummaryComponent::SortedAttemptTableWidgetItem: public QTableWidgetItem
{
    virtual bool operator<(const QTableWidgetItem &other) const
    {
        if (data(Qt::DisplayRole).toInt() == other.data(Qt::DisplayRole).toInt()) {
            return data(Qt::UserRole).toInt() < other.data(Qt::UserRole).toInt();
        }
        return data(Qt::DisplayRole).toInt() < other.data(Qt::DisplayRole).toInt();
    }
};

PracticeSummaryComponent::PracticeSummaryComponent(TestEntryManager* testEntryManager, QWidget* parent)
    :KXmlGuiWindow(parent)
    ,m_testEntryManager(testEntryManager)
{
    // KXmlGui
    setXMLFile("practicesummaryui.rc");
    setObjectName("Statistics");
    
    QWidget *mainWidget = new QWidget(this);
    setupUi(mainWidget);
    setCentralWidget(mainWidget);

    initActions(parent);
    
    setupDetailsTable();
    summaryBar->setStatistics(m_testEntryManager->statisticTotalCorrectFirstAttempt(), m_testEntryManager->statisticTotalWrong(), m_testEntryManager->statisticTotalUnanswered());

    int total = m_testEntryManager->statisticTotalCorrectFirstAttempt() + m_testEntryManager->statisticTotalWrong();
    int minutes = m_testEntryManager->totalTime() / 60;
    int seconds = m_testEntryManager->totalTime() % 60;
    
    testSummaryLabel->setText(i18nc("number of words, minutes, seconds", "You practiced %1 in %2 and %3.",
                                    i18np("one word", "%1 words", total),
                                    i18np("one minute", "%1 minutes", minutes),
                                    i18np("one second", "%1 seconds", seconds)));
    
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);
}

PracticeSummaryComponent::~PracticeSummaryComponent()
{
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void PracticeSummaryComponent::initActions(QWidget* parleyMainWindow)
{
    ParleyActions::create(ParleyActions::EnterEditMode, parleyMainWindow, SLOT(showEditor()), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, parleyMainWindow, SLOT(showPracticeConfiguration()), actionCollection());
    actionCollection()->action("practice_start")->setText(i18n("Practice Again"));
    actionCollection()->action("practice_start")->setToolTip(i18n("Practice Again"));
}

void PracticeSummaryComponent::setupDetailsTable()
{
    tableWidget->setRowCount(m_testEntryManager->totalEntryCount());
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    Qt::ItemFlags flags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    KColorScheme scheme(QPalette::Active);
    QPalette correctPalette = QApplication::palette();
    correctPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::PositiveText).color());
    correctPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::PositiveText).color());

    QPalette wrongPalette = QApplication::palette();
    wrongPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::NegativeText).color());
    wrongPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::NegativeText).color());

    int i = 0;
    // TODO headers with languages
    // TODO some colors, maybe an indicator icon wether the word was right/wrong
    foreach(TestEntry* entry, m_testEntryManager->allTestEntries()) {
        QTableWidgetItem* itemFrom = new QTableWidgetItem(
                entry->entry()->translation(TestEntry::gradeFrom())->text());
        QTableWidgetItem* itemTo = new QTableWidgetItem(
                entry->entry()->translation(TestEntry::gradeTo())->text());
        if (entry->statisticGoodCount() > 0) {
            itemTo->setForeground(correctPalette.foreground());
        }

        QTableWidgetItem* itemUserAnswer = new QTableWidgetItem(
                entry->userAnswers().join("; "));
        itemUserAnswer->setForeground(wrongPalette.foreground());

        SortedAttemptTableWidgetItem* itemAttempts = new SortedAttemptTableWidgetItem();
        itemAttempts->setData(Qt::DisplayRole, entry->statisticCount());
        itemAttempts->setData(Qt::UserRole, entry->statisticBadCount());
        itemAttempts->setTextAlignment(Qt::AlignRight);

        itemFrom->setFlags(flags);
        itemTo->setFlags(flags);
        itemUserAnswer->setFlags(flags);
        itemAttempts->setFlags(flags);

        if (entry->correctAtFirstAttempt()) {
            itemUserAnswer->setIcon(KIcon("dialog-ok-apply"));
        } else if (entry->statisticGoodCount() > 0) {
            itemUserAnswer->setIcon(KIcon("task-attempt"));
        } else if (entry->statisticCount() > 0) {
            itemUserAnswer->setIcon(KIcon("dialog-error"));
        } else {
            itemUserAnswer->setIcon(KIcon("task-attempt"));
        }

        tableWidget->setItem(i, 0, itemAttempts);
        tableWidget->setItem(i, 1, itemFrom);
        tableWidget->setItem(i, 2, itemTo);
        tableWidget->setItem(i, 3, itemUserAnswer);
        ++i;
    }

    tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(0, Qt::DescendingOrder);
}


#include "practicesummarycomponent.moc"

