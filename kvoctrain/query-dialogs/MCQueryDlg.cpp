/***************************************************************************

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 11:45:53 MET 1999

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

#include "MCQueryDlg.h"

#include <QTimer>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QKeyEvent>

#include <KLocale>
#include <KDebug>
#include <KRandomSequence>

#include <kvttablemodel.h>

#include <keduvocdocument.h>

MCQueryDlg::MCQueryDlg(QWidget *parent) : QueryDlgBase(i18n("Multiple Choice"), parent)
{
    mw = new Ui::MCQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));
    connect(mw->rb_trans5, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_trans4, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_trans3, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_trans2, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_trans1, SIGNAL(clicked()), SLOT(verifyClicked()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    KConfigGroup cg(KGlobal::config(), "MCQueryDlg");
    restoreDialogSize(cg);
}


MCQueryDlg::~MCQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "MCQueryDlg");
    KDialog::saveDialogSize(cg);
}


void MCQueryDlg::setQuery(const QString &org, int entry, int orgcol, int transcol, int queryCycle, int q_num, int q_start, KEduVocDocument *doc)
{
    m_doc = doc;
    m_row = entry;
    KEduVocExpression *vocExpression = m_doc->entry(m_row);
    m_queryOriginalColumn = orgcol;
    m_queryTranslationColumn = transcol;
    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->orgField->setFont(Prefs::tableFont());
    mw->orgField->setText(org);
    mw->show_all->setDefault(true);

    // Query cycle - how often did this show up (?)
    mw->progCount->setText(QString::number(queryCycle));
    //Counter - how many correct out of...
    mw->countbar->setMaximum(q_start);
    mw->countbar->setValue(q_start - q_num + 1);

    //Time
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

    KRandomSequence randomSequence;
    QStringList choices;
    button_ref.clear();
    button_ref.append(qMakePair(mw->rb_trans1, mw->trans1));
    button_ref.append(qMakePair(mw->rb_trans2, mw->trans2));
    button_ref.append(qMakePair(mw->rb_trans3, mw->trans3));
    button_ref.append(qMakePair(mw->rb_trans4, mw->trans4));
    button_ref.append(qMakePair(mw->rb_trans5, mw->trans5));
    randomSequence.randomize(button_ref);
    resetQueryWidget(button_ref[0].first);
    resetQueryWidget(button_ref[1].first);
    resetQueryWidget(button_ref[2].first);
    resetQueryWidget(button_ref[3].first);
    resetQueryWidget(button_ref[4].first);
    resetQueryWidget(button_ref[0].second);
    resetQueryWidget(button_ref[1].second);
    resetQueryWidget(button_ref[2].second);
    resetQueryWidget(button_ref[3].second);
    resetQueryWidget(button_ref[4].second);

    KEduVocMultipleChoice multipleChoice = vocExpression->multipleChoice(m_queryTranslationColumn);
    for (int i = 0; i < qMin(MAX_MULTIPLE_CHOICE, (int) multipleChoice.size()); ++i) {
        choices.append(multipleChoice.mc(i));
    }

    if (choices.count() > 1)
        randomSequence.randomize(choices);

    // always include false friend
    QString ff;
    if (m_queryTranslationColumn != 0)
        ff = vocExpression->fauxAmi(m_queryTranslationColumn, false).simplified();
    else
        ff = vocExpression->fauxAmi(m_queryOriginalColumn, true).simplified();

    if (!ff.isEmpty())
        choices.prepend(ff);

    if (doc->entryCount() <= MAX_MULTIPLE_CHOICE) {
        for (int i = choices.count(); i < doc->entryCount(); ++i) {
            KEduVocExpression *act = doc->entry(i);

            if (act != vocExpression) {
                if (m_queryTranslationColumn == 0)
                    choices.append(act->original());
                else
                    choices.append(act->translation(m_queryTranslationColumn));
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;

        int count = MAX_MULTIPLE_CHOICE;
        // gather random expressions for the choice
        while (count > 0) {
            int nr = randomSequence.getLong(doc->entryCount());
            // append if new expr found
            bool newex = true;
            for (int i = 0; newex && i < exprlist.count(); i++) {
                if (exprlist[i] == doc->entry(nr))
                    newex = false;
            }
            if (newex && vocExpression != doc->entry(nr)) {
                count--;
                exprlist.append(doc->entry(nr));
            }
        }

        for (int i = 0; i < exprlist.count(); i++) {
            if (m_queryTranslationColumn == 0)
                choices.append(exprlist[i]->original());
            else
                choices.append(exprlist[i]->translation(m_queryTranslationColumn));
        }

    }

    if (m_queryTranslationColumn == 0)
        choices.prepend(vocExpression->original());
    else
        choices.prepend(vocExpression->translation(m_queryTranslationColumn));

    for (int i = choices.count(); i < MAX_MULTIPLE_CHOICE; i++)
        choices.append("");

    if (choices.count() > MAX_MULTIPLE_CHOICE)
        choices.erase(choices.begin()+MAX_MULTIPLE_CHOICE, choices.end());

    button_ref[0].first->setEnabled(!choices[0].isEmpty());
    button_ref[1].first->setEnabled(!choices[1].isEmpty());
    button_ref[2].first->setEnabled(!choices[2].isEmpty());
    button_ref[3].first->setEnabled(!choices[3].isEmpty());
    button_ref[4].first->setEnabled(!choices[4].isEmpty());

    button_ref[0].second->setEnabled(!choices[0].isEmpty());
    button_ref[1].second->setEnabled(!choices[1].isEmpty());
    button_ref[2].second->setEnabled(!choices[2].isEmpty());
    button_ref[3].second->setEnabled(!choices[3].isEmpty());
    button_ref[4].second->setEnabled(!choices[4].isEmpty());

    button_ref[0].second->setText(choices[0]);
    button_ref[0].second->setFont(Prefs::tableFont());
    button_ref[1].second->setText(choices[1]);
    button_ref[1].second->setFont(Prefs::tableFont());
    button_ref[2].second->setText(choices[2]);
    button_ref[2].second->setFont(Prefs::tableFont());
    button_ref[3].second->setText(choices[3]);
    button_ref[3].second->setFont(Prefs::tableFont());
    button_ref[4].second->setText(choices[4]);
    button_ref[4].second->setFont(Prefs::tableFont());

    mw->rb_trans1->setChecked(false);
    mw->rb_trans2->setChecked(false);
    mw->rb_trans3->setChecked(false);
    mw->rb_trans4->setChecked(false);
    mw->rb_trans5->setChecked(false);

    mw->show_all->setFocus();
}


void MCQueryDlg::initFocus() const
{
    mw->rb_trans1->setFocus();
}


void MCQueryDlg::showSolution()
{
    resetQueryWidget(button_ref[0].first);
    resetQueryWidget(button_ref[1].first);
    resetQueryWidget(button_ref[2].first);
    resetQueryWidget(button_ref[3].first);
    resetQueryWidget(button_ref[4].first);
    resetQueryWidget(button_ref[0].second);
    resetQueryWidget(button_ref[1].second);
    resetQueryWidget(button_ref[2].second);
    resetQueryWidget(button_ref[3].second);
    resetQueryWidget(button_ref[4].second);

    button_ref[0].first->setFocus();
    button_ref[0].first->setChecked(true);
    verifyButton(button_ref[0].first, true, button_ref[0].second);
    mw->dont_know->setDefault(true);
}


void MCQueryDlg::verifyClicked()
{
    bool known = button_ref[0].first->isChecked();

    if (button_ref[0].first->isChecked()) {
        verifyButton(button_ref[0].first, known, button_ref[0].second);
        resetQueryWidget(button_ref[1].first);
        resetQueryWidget(button_ref[1].second);
        resetQueryWidget(button_ref[2].first);
        resetQueryWidget(button_ref[2].second);
        resetQueryWidget(button_ref[3].first);
        resetQueryWidget(button_ref[3].second);
        resetQueryWidget(button_ref[4].first);
        resetQueryWidget(button_ref[4].second);
    } else if (button_ref[1].first->isChecked()) {
        resetQueryWidget(button_ref[0].first);
        resetQueryWidget(button_ref[0].second);
        verifyButton(button_ref[1].first, known, button_ref[1].second);
        resetQueryWidget(button_ref[2].first);
        resetQueryWidget(button_ref[2].second);
        resetQueryWidget(button_ref[3].first);
        resetQueryWidget(button_ref[3].second);
        resetQueryWidget(button_ref[4].first);
        resetQueryWidget(button_ref[4].second);
    } else if (button_ref[2].first->isChecked()) {
        resetQueryWidget(button_ref[0].first);
        resetQueryWidget(button_ref[0].second);
        resetQueryWidget(button_ref[1].first);
        resetQueryWidget(button_ref[1].second);
        verifyButton(button_ref[2].first, known, button_ref[2].second);
        resetQueryWidget(button_ref[3].first);
        resetQueryWidget(button_ref[3].second);
        resetQueryWidget(button_ref[4].first);
        resetQueryWidget(button_ref[4].second);
    } else if (button_ref[3].first->isChecked()) {
        resetQueryWidget(button_ref[0].first);
        resetQueryWidget(button_ref[0].second);
        resetQueryWidget(button_ref[1].first);
        resetQueryWidget(button_ref[1].second);
        resetQueryWidget(button_ref[2].first);
        resetQueryWidget(button_ref[2].second);
        verifyButton(button_ref[3].first, known, button_ref[3].second);
        resetQueryWidget(button_ref[4].first);
        resetQueryWidget(button_ref[4].second);
    } else if (button_ref[4].first->isChecked()) {
        resetQueryWidget(button_ref[0].first);
        resetQueryWidget( button_ref[0].second);
        resetQueryWidget(button_ref[1].first);
        resetQueryWidget(button_ref[1].second);
        resetQueryWidget(button_ref[2].first);
        resetQueryWidget(button_ref[2].second);
        resetQueryWidget(button_ref[3].first);
        resetQueryWidget(button_ref[3].second);
        verifyButton(button_ref[4].first, known, button_ref[4].second);
    }

    if (known) {
        mw->status->setText(getOKComment((mw->countbar->value()/mw->countbar->maximum()) * 100));
        knowItClicked();
    } else {
        mw->status->setText(getNOKComment((mw->countbar->value()/mw->countbar->maximum()) * 100));
        mw->dont_know->setDefault(true);
    }
}


void MCQueryDlg::knowItClicked()
{
    mw->status->setText("");
    emit sigQueryChoice(Known);
}


void MCQueryDlg::dontKnowClicked()
{
    mw->status->setText("");
    emit sigQueryChoice(Unknown);
}


void MCQueryDlg::slotUser1()
{
    if (m_timer != 0)
        m_timer->stop();

    emit sigEditEntry(m_row, KV_COL_ORG+m_queryOriginalColumn);

    KEduVocExpression *vocExpression = m_doc->entry(m_row);
    mw->orgField->setText(m_queryOriginalColumn == 0 ? vocExpression->original() : vocExpression->translation(m_queryOriginalColumn));
}

#include "MCQueryDlg.moc"
