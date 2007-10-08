/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicesummarydialog.h"
#include "prefs.h"

#include "testentrymanager.h"
#include <keduvocexpression.h>

PracticeSummaryDialog::PracticeSummaryDialog(TestEntryManager* testEntryManager, QWidget* parent)
    :KDialog(parent)
{
    m_testEntryManager = testEntryManager;

    setupUi(mainWidget());
    setButtons(KDialog::Ok);

    setCaption(i18n("Practice Summary"));

    totalCountLineEdit->setText(QString::number(testEntryManager->totalEntryCount()));
    correctLineEdit->setText(QString::number(testEntryManager->statisticTotalCorrectFirstAttempt()));
    correctProgressBar->setValue(testEntryManager->statisticTotalCorrectFirstAttempt() * 100 / testEntryManager->totalEntryCount());

    wrongLineEdit->setText(QString::number(testEntryManager->statisticTotalWrong()));
    wrongProgressBar->setValue(testEntryManager->statisticTotalWrong() * 100 / testEntryManager->totalEntryCount());

    if ( Prefs::iKnow() ) {
        skipKnownLineEdit->setText(QString::number(
            testEntryManager->statisticTotalSkipKnown()));
        skipKnownProgressBar->setValue(
            testEntryManager->statisticTotalSkipKnown() * 100
            / testEntryManager->totalEntryCount());
    } else {
        skipKnownLabel->setVisible(false);
        skipKnownLineEdit->setVisible(false);
        skipKnownProgressBar->setVisible(false);
    }

    skipUnknownLineEdit->setText(QString::number(testEntryManager->statisticTotalSkipUnknown()));
    skipUnknownProgressBar->setValue(testEntryManager->statisticTotalSkipUnknown() * 100 / testEntryManager->totalEntryCount());

    notAnsweredLineEdit->setText(QString::number(testEntryManager->statisticTotalUnanswered()));
    notAnsweredProgressBar->setValue(testEntryManager->statisticTotalUnanswered() * 100 / testEntryManager->totalEntryCount());

    setButtonFocus(KDialog::Ok);

    KConfigGroup cg(KGlobal::config(), "practicesummarydialog");
    restoreDialogSize(cg);
}


PracticeSummaryDialog::~PracticeSummaryDialog()
{
    KConfigGroup cg(KGlobal::config(), "practicesummarydialog");
    KDialog::saveDialogSize(cg);
}


#include "practicesummarydialog.moc"

