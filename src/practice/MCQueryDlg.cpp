/***************************************************************************

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 11:45:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#define MAX_MULTIPLE_CHOICE  5  // select one out of x

MCQueryDlg::MCQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Multiple Choice"), doc, parent)
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

    connect(this, SIGNAL(user1Clicked()), this, SLOT(editEntry()));

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


void MCQueryDlg::setEntry( TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

// const QString &org, int entry, int orgcol, int transcol, int queryCycle, int q_num, int q_start,

    KEduVocExpression *vocExpression = entry->exp;
    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->orgField->setFont(Prefs::tableFont());
    mw->orgField->setText(entry->exp->translation(Prefs::fromIdentifier()).text());
    mw->show_all->setDefault(true);

    // Query cycle - how often did this show up (?)
    mw->progCount->setText(QString::number(entry->statisticCount()));

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


    KEduVocMultipleChoice multipleChoice = vocExpression->translation(Prefs::toIdentifier()).multipleChoice();
    for (int i = 0; i < qMin(MAX_MULTIPLE_CHOICE, (int) multipleChoice.size()); ++i) {
        choices.append(multipleChoice.choice(i));
        kDebug() << "Append choice: " << multipleChoice.choice(i);
    }

    if (choices.count() > 1)
        randomSequence.randomize(choices);

    // always include false friend
    QString ff;
    /// @todo: check if it works: false friend should always be included! (Cannot think if the following is ok in both directions...)
    ff = vocExpression->translation(Prefs::toIdentifier()).falseFriend(Prefs::fromIdentifier()).simplified();

    if (!ff.isEmpty())
        choices.prepend(ff);

    if (m_doc->entryCount() <= MAX_MULTIPLE_CHOICE) {
        for (int i = choices.count(); i < m_doc->entryCount(); ++i) {
            KEduVocExpression *act = m_doc->entry(i);

            if (act != vocExpression) {
                choices.append(act->translation(Prefs::toIdentifier()).text());
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;

        int count = MAX_MULTIPLE_CHOICE;
        // gather random expressions for the choice
        while (count > 0) {
            int nr = randomSequence.getLong(m_doc->entryCount());
            // append if new expr found
            bool newex = true;
            for (int i = 0; newex && i < exprlist.count(); i++) {
                if (exprlist[i] == m_doc->entry(nr))
                    newex = false;
            }
            if (newex && vocExpression != m_doc->entry(nr)) {
                count--;
                exprlist.append(m_doc->entry(nr));
            }
        }

        for (int i = 0; i < exprlist.count(); i++) {
            choices.append(exprlist[i]->translation(Prefs::toIdentifier()).text());
        }

    }

    choices.prepend(vocExpression->translation(Prefs::toIdentifier()).text());

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

    // As long as the buttons are AutoExclusive we cannot uncheck all.
    mw->rb_trans5->setChecked(true);
    mw->rb_trans5->setAutoExclusive ( false );
    mw->rb_trans5->setChecked(false);
    mw->rb_trans5->setAutoExclusive ( true );

    mw->show_all->setFocus();
}


void MCQueryDlg::initFocus()
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
kDebug() << "verify";
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
kDebug() << "Emit signal: Known";
    mw->status->setText("");
    emit sigQueryChoice(SkipKnown);
}


void MCQueryDlg::dontKnowClicked()
{
    mw->status->setText("");
    emit sigQueryChoice(SkipUnknown);
}


void MCQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}


#include "MCQueryDlg.moc"


