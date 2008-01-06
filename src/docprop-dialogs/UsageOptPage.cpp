/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin         : Thu Mar 30 20:38:31 1999

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

#include "UsageOptPage.h"

#include <QGroupBox>
#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "practice/testentrymanager.h"

#define USAGE_TAG ". "

UsageOptPage::UsageOptPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Usage Labels"));

    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotUsageChosen(int)));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteUsage()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyUsage()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewUsage()));

    m_doc = doc;

    /// @todo get rid of the old clean up button in the ui
    cleanUpButton->setEnabled(false);

//     int i = 1;
//     foreach(const QString &usageName, doc->usages()) {
//         optionsList->addItem(QString("%1").arg(i++, 2).append(USAGE_TAG).append(usageName));
//         usageIndex.append(i);
//     }

    m_currentUsage = 0;
    if (optionsList->count() > 0) {
        optionsList->setCurrentRow(m_currentUsage);
    }

    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);

    optionsList->setFocus();
}


void UsageOptPage::slotUsageChosen(int index)
{
    m_currentUsage = index;
}


void UsageOptPage::slotNewUsage()
{
    bool ok;
    QString newUsageString = KInputDialog::getText(i18nc("usage (area) of an expression", "Usage Description"), i18n("Enter usage description:"), QString(), &ok, this);

    if (!ok) {
        return;
    }

    QString str;
    int i = optionsList->count() + 1;


    optionsList->addItem( newUsageString );

    m_currentUsage = optionsList->count();
    optionsList->setCurrentRow(i - 1);

    m_newUsages.append( newUsageString );

    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}


void UsageOptPage::slotModifyUsage()
{
    if (optionsList->count() > 0 && optionsList->count() > m_currentUsage) {
        QString str = optionsList->item(m_currentUsage)->text();

        bool ok;
        QString newUsageString = KInputDialog::getText(i18nc("usage (area) of an expression", "Usage Description"), i18n("Enter usage description:"), str, &ok, this);
        if (!ok) {
            return;
        }

        optionsList->item(m_currentUsage)->setText(newUsageString);

        m_renameList.append(qMakePair(str, newUsageString));
    }
}


void UsageOptPage::slotDeleteUsage()
{
    m_deletedUsages.append(optionsList->currentItem()->text());
    delete optionsList->takeItem(optionsList->currentRow());
}


void UsageOptPage::accept()
{
    foreach ( const QString &newUsage, m_newUsages ) {
        m_doc->addUsage(newUsage);
    }

    for (int i = 0; i < m_renameList.count(); i++) {
        m_doc->renameUsage(m_renameList[i].first, m_renameList[i].second);
    }

    foreach ( const QString &newUsage, m_deletedUsages ) {
        m_doc->removeUsage(newUsage);
    }
}

#include "UsageOptPage.moc"
