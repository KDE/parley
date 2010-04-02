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
#include <QLabel>

#include <KLocale>
#include <KRandomSequence>
#include <KLineEdit>

AdjQueryDlg::AdjQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Comparison Training"), doc, parent)
{
    mw = new Ui::AdjQueryDlgForm();
    mw->setupUi(mainWidget());

    mw->stopPracticeButton->setIcon( KIcon("process-stop") );
    mw->verify->setIcon(KIcon("dialog-ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));
    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->lev1Field, SIGNAL(textChanged(const QString&)), SLOT(lev1Changed(const QString&)));
    connect(mw->lev2Field, SIGNAL(textChanged(const QString&)), SLOT(lev2Changed(const QString&)));
    connect(mw->lev3Field, SIGNAL(textChanged(const QString&)), SLOT(lev3Changed(const QString&)));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->know_it->setVisible(Prefs::skipKnownEnabled());

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    mw->imageGraphicsView->setVisible(false);

    KConfigGroup cg(KGlobal::config(), "AdjQueryDlg");
    restoreDialogSize(cg);
}


AdjQueryDlg::~AdjQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "AdjQueryDlg");
    KDialog::saveDialogSize(cg);
}


void AdjQueryDlg::setEntry(TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    ///@todo set adjective/adverb hint!

    mw->timebar->setVisible(Prefs::practiceTimeout());
    mw->timelabel->setVisible(Prefs::practiceTimeout());
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
        mw->lev1Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->text());
        break;

    case 1:
        mw->lev2Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->comparative());
        break;

    case 2:
        mw->lev3Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->superlative());
        break;
    }

    mw->lev1Field->setEnabled(!m_entry->entry()->translation(Prefs::solutionLanguage())->text().isEmpty());
    mw->lev2Field->setEnabled(!m_entry->entry()->translation(Prefs::solutionLanguage())->comparative().isEmpty());
    mw->lev3Field->setEnabled(!m_entry->entry()->translation(Prefs::solutionLanguage())->superlative().isEmpty());

    resetAllFields();

    mw->lev1Field->setFocus();

    imageShowFromEntry( mw->imageGraphicsView );
}


void AdjQueryDlg::showSolution()
{
    resetAllFields();
    mw->lev1Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->text());
    mw->lev2Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->comparative());
    mw->lev3Field->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->superlative());

    mw->dont_know->setDefault(true);
    setAnswerTainted();
}


void AdjQueryDlg::verifyClicked()
{
    bool all_known = true;

    double result = verifyAnswer(mw->lev1Field->text(), m_entry->entry()->translation(Prefs::solutionLanguage())->text());
    if ( result == 1.0 ) {
        setWidgetStyle( mw->lev1Field, PositiveResult );
    } else {
        setWidgetStyle( mw->lev1Field, NegativeResult );
        all_known = false;
    }

    result = verifyAnswer(mw->lev2Field->text(), m_entry->entry()->translation(Prefs::solutionLanguage())->comparative());
    if ( result == 1.0 ) {
        setWidgetStyle( mw->lev2Field, PositiveResult );
    } else {
        setWidgetStyle( mw->lev2Field, NegativeResult );
        all_known = false;
    }

    result = verifyAnswer(mw->lev3Field->text(), m_entry->entry()->translation(Prefs::solutionLanguage())->superlative());
    if ( result == 1.0 ) {
        setWidgetStyle( mw->lev3Field, PositiveResult );
    } else {
        setWidgetStyle( mw->lev3Field, NegativeResult );
        all_known = false;
    }

    if (all_known) {
//  know_it->setDefault(true);
        resultCorrect();
        emit currentEntryFinished();
    } else {
        mw->dont_know->setDefault(true);
        setAnswerTainted();
    }
}


void AdjQueryDlg::resetAllFields()
{
    setWidgetStyle(mw->lev1Field, Default);
    setWidgetStyle(mw->lev2Field, Default);
    setWidgetStyle(mw->lev3Field, Default);
}


void AdjQueryDlg::lev1Changed(const QString&)
{
    mw->verify->setDefault(true);
    setWidgetStyle(mw->lev1Field);
}


void AdjQueryDlg::lev2Changed(const QString&)
{
    mw->verify->setDefault(true);
    setWidgetStyle(mw->lev2Field);
}


void AdjQueryDlg::lev3Changed(const QString&)
{
    mw->verify->setDefault(true);
    setWidgetStyle(mw->lev3Field);
}


void AdjQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}

#include "AdjQueryDlg.moc"

