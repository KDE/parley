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
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showItClicked()));
    connect(mw->rb_trans5, SIGNAL(clicked()), SLOT(trans5clicked()));
    connect(mw->rb_trans4, SIGNAL(clicked()), SLOT(trans4clicked()));
    connect(mw->rb_trans3, SIGNAL(clicked()), SLOT(trans3clicked()));
    connect(mw->rb_trans2, SIGNAL(clicked()), SLOT(trans2clicked()));
    connect(mw->rb_trans1, SIGNAL(clicked()), SLOT(trans1clicked()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

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


void MCQueryDlg::setQuery(const QString &org,
                          int entry,
                          int orgcol,
                          int transcol,
                          int queryCycle,
                          int q_num,
                          int q_start,
                          KEduVocDocument *doc)
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
    QString s;
    s.setNum(queryCycle);
    mw->progCount->setText(s);
    //Counter - how many right out of...
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
    QList<QString> strings; // great descriptive name
    button_ref.clear();
    button_ref.append(RB_Label(mw->rb_trans1, mw->trans1));
    button_ref.append(RB_Label(mw->rb_trans2, mw->trans2));
    button_ref.append(RB_Label(mw->rb_trans3, mw->trans3));
    button_ref.append(RB_Label(mw->rb_trans4, mw->trans4));
    button_ref.append(RB_Label(mw->rb_trans5, mw->trans5));
    randomSequence.randomize(button_ref);
    resetButton(button_ref[0].rb, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);

    solution = 0;

    KEduVocMultipleChoice multipleChoice = vocExpression->multipleChoice(m_queryTranslationColumn);
    for (int i = 0; i < qMin(MAX_MULTIPLE_CHOICE, (int)multipleChoice.size()); ++i) {
        strings.push_back(multipleChoice.mc(i));
    }

    if (strings.count() > 1)
        randomSequence.randomize(strings);

    // always include false friend
    QString ff;
    if (m_queryTranslationColumn != 0)
        ff = vocExpression->fauxAmi(m_queryTranslationColumn, false).simplified();
    else
        ff = vocExpression->fauxAmi(m_queryOriginalColumn, true).simplified();

    if (ff.length())
        strings.insert(strings.begin(), ff);

    if (doc->entryCount() <= MAX_MULTIPLE_CHOICE) {
        for (int i = strings.size(); i < doc->entryCount(); ++i) {
            KEduVocExpression *act = doc->entry(i);

            if (act != vocExpression) {
                if (m_queryTranslationColumn == 0)
                    strings.push_back(act->original());
                else
                    strings.push_back(act->translation(m_queryTranslationColumn));
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;
        solution = 0;

        int count = MAX_MULTIPLE_CHOICE;
        // gather random expressions for the choice
        while (count > 0) {
            int nr = getRandom(doc->entryCount()); // * ((1.0*rand())/RAND_MAX));
            // append if new expr found
            bool newex = true;
            for (int i = 0; newex && i < (int) exprlist.size(); i++) {
                if (exprlist[i] == doc->entry(nr))
                    newex = false;
            }
            if (newex && vocExpression != doc->entry(nr)) {
                count--;
                exprlist.push_back(doc->entry(nr));
            }
        }

        for (int i = 0; i < (int) exprlist.size(); i++) {
            if (m_queryTranslationColumn == 0)
                strings.push_back(exprlist[i]->original());
            else
                strings.push_back(exprlist[i]->translation(m_queryTranslationColumn));
        }

    }

    // solution is always the firandomSequencet
    if (m_queryTranslationColumn == 0)
        strings.insert(strings.begin(), vocExpression->original());
    else
        strings.insert(strings.begin(), vocExpression->translation(m_queryTranslationColumn));

    for (int i = strings.size(); i < MAX_MULTIPLE_CHOICE; i++)
        strings.push_back("");

    if (strings.size() > MAX_MULTIPLE_CHOICE)
        strings.erase(strings.begin()+MAX_MULTIPLE_CHOICE, strings.end());

    button_ref[0].rb->setEnabled(!strings[0].isEmpty());
    button_ref[1].rb->setEnabled(!strings[1].isEmpty());
    button_ref[2].rb->setEnabled(!strings[2].isEmpty());
    button_ref[3].rb->setEnabled(!strings[3].isEmpty());
    button_ref[4].rb->setEnabled(!strings[4].isEmpty());

    button_ref[0].label->setEnabled(!strings[0].isEmpty());
    button_ref[1].label->setEnabled(!strings[1].isEmpty());
    button_ref[2].label->setEnabled(!strings[2].isEmpty());
    button_ref[3].label->setEnabled(!strings[3].isEmpty());
    button_ref[4].label->setEnabled(!strings[4].isEmpty());

    button_ref[0].label->setText(strings[0]);
    button_ref[0].label->setFont(Prefs::tableFont());
    button_ref[1].label->setText(strings[1]);
    button_ref[1].label->setFont(Prefs::tableFont());
    button_ref[2].label->setText(strings[2]);
    button_ref[2].label->setFont(Prefs::tableFont());
    button_ref[3].label->setText(strings[3]);
    button_ref[3].label->setFont(Prefs::tableFont());
    button_ref[4].label->setText(strings[4]);
    button_ref[4].label->setFont(Prefs::tableFont());

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


void MCQueryDlg::showItClicked()
{
    resetButton(button_ref[0].rb, button_ref[0].label);
    resetButton(button_ref[1].rb, button_ref[1].label);
    resetButton(button_ref[2].rb, button_ref[2].label);
    resetButton(button_ref[3].rb, button_ref[3].label);
    resetButton(button_ref[4].rb, button_ref[4].label);

    button_ref[solution].rb->setFocus();
    button_ref[solution].rb->setChecked(true);
    verifyButton(button_ref[solution].rb, true, button_ref[solution].label);
    mw->dont_know->setDefault(true);
}


void MCQueryDlg::verifyClicked()
{

    bool known = false;
    if (solution == 0)
        known = button_ref[0].rb->isChecked();
    if (solution == 1)
        known = button_ref[1].rb->isChecked();
    if (solution == 2)
        known = button_ref[2].rb->isChecked();
    if (solution == 3)
        known = button_ref[3].rb->isChecked();
    if (solution == 4)
        known = button_ref[4].rb->isChecked();

    if (button_ref[0].rb->isChecked()) {
        verifyButton(button_ref[0].rb, known, button_ref[0].label);
        resetButton(button_ref[1].rb, button_ref[1].label);
        resetButton(button_ref[2].rb, button_ref[2].label);
        resetButton(button_ref[3].rb, button_ref[3].label);
        resetButton(button_ref[4].rb, button_ref[4].label);
    } else if (button_ref[1].rb->isChecked()) {
        resetButton(button_ref[0].rb, button_ref[0].label);
        verifyButton(button_ref[1].rb, known, button_ref[1].label);
        resetButton(button_ref[2].rb, button_ref[2].label);
        resetButton(button_ref[3].rb, button_ref[3].label);
        resetButton(button_ref[4].rb, button_ref[4].label);
    } else if (button_ref[2].rb->isChecked()) {
        resetButton(button_ref[0].rb, button_ref[0].label);
        resetButton(button_ref[1].rb, button_ref[1].label);
        verifyButton(button_ref[2].rb, known, button_ref[2].label);
        resetButton(button_ref[3].rb, button_ref[3].label);
        resetButton(button_ref[4].rb, button_ref[4].label);
    } else if (button_ref[3].rb->isChecked()) {
        resetButton(button_ref[0].rb, button_ref[0].label);
        resetButton(button_ref[1].rb, button_ref[1].label);
        resetButton(button_ref[2].rb, button_ref[2].label);
        verifyButton(button_ref[3].rb, known, button_ref[3].label);
        resetButton(button_ref[4].rb, button_ref[4].label);
    } else if (button_ref[4].rb->isChecked()) {
        resetButton(button_ref[0].rb, button_ref[0].label);
        resetButton(button_ref[1].rb, button_ref[1].label);
        resetButton(button_ref[2].rb, button_ref[2].label);
        resetButton(button_ref[3].rb, button_ref[3].label);
        verifyButton(button_ref[4].rb, known, button_ref[4].label);
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


void MCQueryDlg::timeoutReached()
{
    if (m_timerCount > 0) {
        m_timerCount--;
        mw->timebar->setValue(m_timerCount);
        m_timer->start(1000);
    }

    if (m_timerCount <= 0) {
        mw->status->setText(getTimeoutComment((mw->countbar->value()/mw->countbar->maximum()) * 100));
        mw->timebar->setValue(0);
        if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
            showItClicked();
            mw->dont_know->setDefault(true);
        } else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
            emit sigQueryChoice(Timeout);
    } else
        mw->status->setText("");
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
    mw->orgField->setText(m_queryOriginalColumn == 0
                          ? vocExpression->original()
                          : vocExpression->translation(m_queryOriginalColumn));
}


void MCQueryDlg::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        dontKnowClicked();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (mw->dont_know->isDefault())
            dontKnowClicked();
        else if (mw->know_it->isDefault())
            knowItClicked();
        else if (mw->show_all->isDefault())
            showItClicked();
        break;

    default:
        e->ignore();
        break;
    }
}


void MCQueryDlg::trans1clicked()
{
    verifyClicked();
}


void MCQueryDlg::trans2clicked()
{
    verifyClicked();
}


void MCQueryDlg::trans3clicked()
{
    verifyClicked();
}


void MCQueryDlg::trans4clicked()
{
    verifyClicked();
}


void MCQueryDlg::trans5clicked()
{
    verifyClicked();
}


#include "MCQueryDlg.moc"
