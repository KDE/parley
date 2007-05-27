/***************************************************************************

                     query dialog for articles

    -----------------------------------------------------------------------

    begin         : Fri Dec 3 18:28:18 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include "ArtQueryDlg.h"

#include <QTimer>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

#include <KLocale>

#include <kvttablemodel.h>

ArtQueryDlg::ArtQueryDlg(QWidget *parent) : QueryDlgBase(i18n("Article Training"), parent)
{
    mw = new Ui::ArtQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showAllClicked()));

    connect(mw->natural, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->male, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_fem, SIGNAL(clicked()), SLOT(verifyClicked()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    KConfigGroup cg(KGlobal::config(), "ArtQueryDlg");
    restoreDialogSize(cg);
}


ArtQueryDlg::~ArtQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "ArtQueryDlg");
    KDialog::saveDialogSize(cg);
}


void ArtQueryDlg::setQuery(int entry,
                           int col,
                           int q_cycle,
                           int q_num,
                           int q_start,
                           KEduVocExpression *exp,
                           const KEduVocArticle &art)
{
    m_expression = exp;
    m_row = entry;
    m_queryOriginalColumn = col;
    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->show_all->setDefault(true);
    articles = art;

    QString s;
    if (col == 0)
        s = exp->original().simplified();
    else
        s = exp->translation(m_queryOriginalColumn).simplified();

    QString def, indef;
    bool removed = false;

    articles.getFemale(&def, &indef);
    mw->rb_fem->setText(i18n("&female:\t")+def+" / "+indef);
    mw->rb_fem->setEnabled(!QString(def+indef).isEmpty());
    if (!removed && s.indexOf(def+' ') == 0) {
        s.remove(0, def.length()+1);
        removed = true;
    }
    if (!removed && s.indexOf(indef+' ') == 0) {
        s.remove(0, indef.length()+1);
        removed = true;
    }

    articles.getMale(&def, &indef);
    mw->male->setText(i18n("&male:\t")+def+" / "+indef);
    mw->male->setEnabled(!QString(def+indef).isEmpty());
    if (!removed && s.indexOf(def+' ') == 0) {
        s.remove(0, def.length()+1);
        removed = true;
    }
    if (!removed && s.indexOf(indef+' ') == 0) {
        s.remove(0, indef.length()+1);
        removed = true;
    }

    articles.getNatural(&def, &indef);
    mw->natural->setText(i18n("&natural:\t")+def+" / "+indef);
    mw->natural->setEnabled(!QString(def+indef).isEmpty());
    if (!removed && s.indexOf(def+' ') == 0) {
        s.remove(0, def.length()+1);
        removed = true;
    }
    if (!removed && s.indexOf(indef+' ') == 0) {
        s.remove(0, indef.length()+1);
        removed = true;
    }

    mw->orgField->setText(s);
    s.setNum(q_cycle);
    mw->progCount->setText(s);

    mw->countbar->setMaximum(q_start);
    mw->countbar->setValue(q_start - q_num + 1);
    int mqtime = Prefs::maxTimePer();
    if (mqtime > 0) {
        if (m_timer == 0) {
            m_timer = new QTimer(this);
            m_timer->setSingleShot(true);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
        }

        if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
            m_timerCount = mqtime;
            mw->timebar->setMaximum(m_timerCount);
            mw->timebar->setValue(m_timerCount);
            m_timer->start(1000);
        } else
            mw->timebar->setEnabled(false);
    } else
        mw->timebar->setEnabled(false);
}


void ArtQueryDlg::initFocus() const
{
    mw->rb_fem->setFocus();
}


void ArtQueryDlg::showSolution()
{
    resetQueryWidget(mw->rb_fem);
    resetQueryWidget(mw->male);
    resetQueryWidget(mw->natural);

    if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F) {
        mw->rb_fem->setChecked(true);
        verifyButton(mw->rb_fem, true);
    } else if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M) {
        mw->male->setChecked(true);
        verifyButton(mw->male, true);
    } else if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S) {
        mw->natural->setChecked(true);
        verifyButton(mw->natural, true);
    }
    mw->dont_know->setDefault(true);
}


void ArtQueryDlg::showMoreClicked()
{}


void ArtQueryDlg::verifyClicked()
{
    bool known = false;
    if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F)
        known = mw->rb_fem->isChecked();
    else if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M)
        known = mw->male->isChecked();
    else if (m_expression->type(m_queryOriginalColumn) == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S)
        known = mw->natural->isChecked();

    if (mw->rb_fem->isChecked()) {
        verifyButton(mw->rb_fem, known);
        resetQueryWidget(mw->male);
        resetQueryWidget(mw->natural);
    } else if (mw->male->isChecked()) {
        verifyButton(mw->male, known);
        resetQueryWidget(mw->rb_fem);
        resetQueryWidget(mw->natural);
    } else if (mw->natural->isChecked()) {
        verifyButton(mw->natural, known);
        resetQueryWidget(mw->male);
        resetQueryWidget(mw->rb_fem);
    }

    if (known)
//    know_it->setDefault(true);
        knowItClicked();
    else
        mw->dont_know->setDefault(true);
}


void ArtQueryDlg::knowItClicked()
{
    emit sigQueryChoice(Known);
}


void ArtQueryDlg::timeoutReached()
{
    if (m_timerCount > 0) {
        m_timerCount--;
        mw->timebar->setValue(m_timerCount);
        m_timer->start(1000);
    }

    if (m_timerCount <= 0) {
        mw->timebar->setValue(0);
        if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
            showSolution();
            mw->dont_know->setDefault(true);
        } else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
            emit sigQueryChoice(Timeout);
    }
}


void ArtQueryDlg::dontKnowClicked()
{
    emit sigQueryChoice(Unknown);
}


void ArtQueryDlg::slotUser1()
{

    if (m_timer != 0)
        m_timer->stop();

    emit sigEditEntry(m_row, KV_COL_ORG+m_queryOriginalColumn);
}

#include "ArtQueryDlg.moc"
