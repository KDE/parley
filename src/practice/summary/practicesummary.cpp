/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicesummary.h"
#include "prefs.h"

#include "practice/practiceentrymanager.h"
#include "practice/statistics/statistics.h"
#include "practice/summary/summarymodel.h"

#include <keduvocexpression.h>

#include <KStandardGuiItem>
#include <QHeaderView>

PracticeSummary::PracticeSummary(Statistics* statistics, PracticeEntryManager* entryManager, QWidget* parent)
    :KDialog(parent)
{
//     m_statistics = statistics;

    setupUi(mainWidget());
    setButtons(KDialog::User1 | KDialog::Ok);
    setButtonGuiItem(KDialog::User1, KStandardGuiItem::print());


    setCaption(i18n("Practice Summary"));

    totalCountLineEdit->setText("TODO"); //QString::number(statistics->totalEntryCount()));

    correctLineEdit->setText(QString::number(statistics->correct()));
///@todo     correctProgressBar->setValue(statistics->statisticTotalCorrectFirstAttempt() * 100 / statistics->totalEntryCount());

///@todo     wrongLineEdit->setText(QString::number(statistics->statisticTotalWrong()));
///@todo     wrongProgressBar->setValue(statistics->statisticTotalWrong() * 100 / statistics->totalEntryCount());

/*
    if ( Prefs::skipKnownEnabled() ) {
        skipKnownLineEdit->setText(QString::number(
            statistics->statisticTotalSkipKnown()));
        skipKnownProgressBar->setValue(
            statistics->statisticTotalSkipKnown() * 100
            / statistics->totalEntryCount());
    } else {
        skipKnownLabel->setVisible(false);
        skipKnownLineEdit->setVisible(false);
        skipKnownProgressBar->setVisible(false);
    }

    skipUnknownLineEdit->setText(QString::number(statistics->statisticTotalSkipUnknown()));
    skipUnknownProgressBar->setValue(statistics->statisticTotalSkipUnknown() * 100 / statistics->totalEntryCount());

    notAnsweredLineEdit->setText(QString::number(statistics->statisticTotalUnanswered()));
    notAnsweredProgressBar->setValue(statistics->statisticTotalUnanswered() * 100 / statistics->totalEntryCount());
*/

    setButtonFocus(KDialog::Ok);

    // create a model and populate the view
    SummaryModel* model = new SummaryModel(entryManager->m_entriesOriginal, this);
    VocabularyTable->setModel(model);

//     VocabularyTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    KConfigGroup cg(KGlobal::config(), "practicesummarydialog");
    restoreDialogSize(cg);
}


PracticeSummary::~PracticeSummary()
{
    KConfigGroup cg(KGlobal::config(), "practicesummarydialog");
    KDialog::saveDialogSize(cg);
}


#include "practicesummary.moc"

