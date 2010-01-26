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

#include <keduvocexpression.h>
#include <KConfigGroup>
#include <KActionCollection>

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


    totalCountLineEdit->setText(QString::number(m_testEntryManager->totalEntryCount()));
    correctLineEdit->setText(QString::number(m_testEntryManager->statisticTotalCorrectFirstAttempt()));


    if (m_testEntryManager->totalEntryCount() != 0) {
        correctProgressBar->setValue(m_testEntryManager->statisticTotalCorrectFirstAttempt() * 100 / m_testEntryManager->totalEntryCount());

        wrongLineEdit->setText(QString::number(m_testEntryManager->statisticTotalWrong()));
        wrongProgressBar->setValue(m_testEntryManager->statisticTotalWrong() * 100 / m_testEntryManager->totalEntryCount());

        if ( Prefs::skipKnownEnabled() ) {
            skipKnownLineEdit->setText(QString::number(
                    m_testEntryManager->statisticTotalSkipKnown()));
            skipKnownProgressBar->setValue(
                    m_testEntryManager->statisticTotalSkipKnown() * 100
                    / m_testEntryManager->totalEntryCount());
        } else {
            skipKnownLabel->setVisible(false);
            skipKnownLineEdit->setVisible(false);
            skipKnownProgressBar->setVisible(false);
        }

        skipUnknownLineEdit->setText(QString::number(m_testEntryManager->statisticTotalSkipUnknown()));
        skipUnknownProgressBar->setValue(m_testEntryManager->statisticTotalSkipUnknown() * 100 / m_testEntryManager->totalEntryCount());

        notAnsweredLineEdit->setText(QString::number(m_testEntryManager->statisticTotalUnanswered()));
        notAnsweredProgressBar->setValue(m_testEntryManager->statisticTotalUnanswered() * 100 / m_testEntryManager->totalEntryCount());
    }

    initActions(parent);
    
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


#include "practicesummarycomponent.moc"

