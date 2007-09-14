/***************************************************************************

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin         : Sat Dec 4 15:09:18 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "AdjQueryDlg.h"

#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <KLocale>
#include <KRandomSequence>

#include <kvttablemodel.h>

AdjQueryDlg::AdjQueryDlg(KEduVocDocument *doc, QWidget *parent) : QueryDlgBase(i18n("Comparison Training"), doc, parent)
{
    mw = new Ui::AdjQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->lev1Field, SIGNAL(textChanged(const QString&)), SLOT(lev1Changed(const QString&)));
    connect(mw->lev2Field, SIGNAL(textChanged(const QString&)), SLOT(lev2Changed(const QString&)));
    connect(mw->lev3Field, SIGNAL(textChanged(const QString&)), SLOT(lev3Changed(const QString&)));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    KConfigGroup cg(KGlobal::config(), "AdjQueryDlg");
    restoreDialogSize(cg);
}


AdjQueryDlg::~AdjQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "AdjQueryDlg");
    KDialog::saveDialogSize(cg);
}


void AdjQueryDlg::setQuery(TestEntry* entry)
{
    QueryDlgBase::setQuery(entry);

    comp = entry->exp->translation(Prefs::toIdentifier()).comparison();

    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->show_all->setDefault(true);
    QString s;
//     s.setNum(q_cycle);
    mw->progCount->setText(s);

    mw->lev1Field->setText("");
    mw->lev2Field->setText("");
    mw->lev3Field->setText("");

    KRandomSequence rs;
    int sel = rs.getLong(3);
    switch (sel) {
    case 0:
        mw->lev1Field->setText(comp.l1());
        break;

    case 1:
        mw->lev2Field->setText(comp.l2());
        break;

    case 2:
        mw->lev3Field->setText(comp.l3());
        break;
    }

    mw->lev1Field->setEnabled(!comp.l1().isEmpty());
    mw->lev2Field->setEnabled(!comp.l2().isEmpty());
    mw->lev3Field->setEnabled(!comp.l3().isEmpty());

///@todo
//     mw->countbar->setMaximum(q_start);
//     mw->countbar->setValue(q_start - q_num + 1);

    startTimer();

    resetAllFields();
}


void AdjQueryDlg::initFocus()
{
    mw->lev1Field->setFocus();
}


void AdjQueryDlg::showSolution()
{
    resetAllFields();
    mw->lev1Field->setText(comp.l1());
    mw->lev2Field->setText(comp.l2());
    mw->lev3Field->setText(comp.l3());

    verifyField(mw->lev1Field, comp.l1());
    verifyField(mw->lev2Field, comp.l2());
    verifyField(mw->lev3Field, comp.l3());

    mw->dont_know->setDefault(true);
}


void AdjQueryDlg::verifyClicked()
{
    bool all_known = true;

    if (!verifyField(mw->lev1Field, comp.l1()))
        all_known = false;

    if (!verifyField(mw->lev2Field, comp.l2()))
        all_known = false;

    if (!verifyField(mw->lev3Field, comp.l3()))
        all_known = false;

    if (all_known)
//  know_it->setDefault(true);
        knowItClicked();
    else
        mw->dont_know->setDefault(true);
}


void AdjQueryDlg::resetAllFields()
{
    resetQueryWidget(mw->lev1Field);
    resetQueryWidget(mw->lev2Field);
    resetQueryWidget(mw->lev3Field);
}


void AdjQueryDlg::knowItClicked()
{
    emit sigQueryChoice(SkipKnown);
}



void AdjQueryDlg::dontKnowClicked()
{
    emit sigQueryChoice(SkipUnknown);
}


void AdjQueryDlg::slotUser1()
{
    if (m_timer != 0)
        m_timer->stop();

    emit sigEditEntry(m_entry->m_index, Prefs::toIdentifier());
}


void AdjQueryDlg::lev1Changed(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->lev1Field);
}


void AdjQueryDlg::lev2Changed(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->lev2Field);
}


void AdjQueryDlg::lev3Changed(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->lev3Field);
}


void AdjQueryDlg::setProgressCounter(int current, int total)
{///@todo
}

#include "AdjQueryDlg.moc"


