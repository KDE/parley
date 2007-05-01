/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

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

#include "MCEntryPage.h"

#include <QLineEdit>

#include "EntryDlg.h"
#include <kvtlanguages.h>

MCEntryPage::MCEntryPage(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(mc1Field, SIGNAL(textChanged(const QString&)), SLOT(mc1Changed(const QString&)));
    connect(mc2Field, SIGNAL(textChanged(const QString&)), SLOT(mc2Changed(const QString&)));
    connect(mc3Field, SIGNAL(textChanged(const QString&)), SLOT(mc3Changed(const QString&)));
    connect(mc4Field, SIGNAL(textChanged(const QString&)), SLOT(mc4Changed(const QString&)));
    connect(mc5Field, SIGNAL(textChanged(const QString&)), SLOT(mc5Changed(const QString&)));
}


void MCEntryPage::setData(bool multi_sel, const KEduVocMultipleChoice &mc)
{
    multiplechoice = mc;

    mc1Field->setText(mc.mc1());
    mc2Field->setText(mc.mc2());
    mc3Field->setText(mc.mc3());
    mc4Field->setText(mc.mc4());
    mc5Field->setText(mc.mc5());

    m_largeSelection = multi_sel;
    if (m_largeSelection) {
        mc1Field->setEnabled(false);
        mc2Field->setEnabled(false);
        mc3Field->setEnabled(false);
        mc4Field->setEnabled(false);
        mc5Field->setEnabled(false);
    }

    setModified(false);
}


void MCEntryPage::mc1Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC1(s);
}


void MCEntryPage::mc2Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC2(s);
}


void MCEntryPage::mc3Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC3(s);
}


void MCEntryPage::mc4Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC4(s);
}


void MCEntryPage::mc5Changed(const QString& s)
{
    setModified(true);
    multiplechoice.setMC5(s);
}


bool MCEntryPage::isModified()
{
    return modified;
}


void MCEntryPage::setModified(bool mod)
{
    modified = mod;
    if (mod)
        emit sigModified();
}


void MCEntryPage::setEnabled(int enable)
{
    bool ena = enable == EntryDlg::EnableAll;
    if (m_largeSelection)
        ena = false;
    mc1Field->setEnabled(ena);
    mc2Field->setEnabled(ena);
    mc3Field->setEnabled(ena);
    mc4Field->setEnabled(ena);
    mc5Field->setEnabled(ena);
}

#include "MCEntryPage.moc"
