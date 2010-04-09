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

using namespace Practice; 

PracticeSummaryComponent::PracticeSummaryComponent(TestEntryManager* testEntryManager, QWidget* parent)
    :KXmlGuiWindow(parent)
    ,m_testEntryManager(testEntryManager)
{
    // KXmlGui
    setXMLFile("statisticsui.rc");
    setObjectName("Statistics");
    
    QWidget *mainWidget = new QWidget(this);
    setupUi(mainWidget);
    setCentralWidget(mainWidget);

    initActions(parent);
    
    setupDetailsTable();
    summaryBar->setStatistics(m_testEntryManager->statisticTotalCorrectFirstAttempt(), m_testEntryManager->statisticTotalWrong(), m_testEntryManager->statisticTotalUnanswered());

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
    ParleyActions::create(ParleyActions::StartPractice, parleyMainWindow, SLOT(showStatistics()), actionCollection());
    
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

        QTableWidgetItem* itemAttempts = new QTableWidgetItem(
                QString::number(entry->statisticBadCount()));

        itemFrom->setFlags(flags);
        itemTo->setFlags(flags);
        itemUserAnswer->setFlags(flags);
        itemAttempts->setFlags(flags);

        tableWidget->setItem(i, 0, itemFrom);
        tableWidget->setItem(i, 1, itemTo);
        tableWidget->setItem(i, 2, itemUserAnswer);
        tableWidget->setItem(i, 3, itemAttempts);
        ++i;
    }

    tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(3, Qt::DescendingOrder);
}


#include "practicesummarycomponent.moc"

