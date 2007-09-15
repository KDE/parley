/***************************************************************************

                     query dialog for articles

    -----------------------------------------------------------------------

    begin         : Fri Dec 3 18:28:18 1999

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

#include "ArtQueryDlg.h"

#include "kvttablemodel.h"
#include <keduvocwordtype.h>
#include <KLocale>
#include <QTimer>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

ArtQueryDlg::ArtQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Article Training"), doc, parent)
{
    mw = new Ui::ArtQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->natural, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->male, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->rb_fem, SIGNAL(clicked()), SLOT(verifyClicked()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(editEntry()));

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


void ArtQueryDlg::setEntry(TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->show_all->setDefault(true);

    QString s;
    s = m_entry->exp->translation(Prefs::toIdentifier()).text().simplified();

    QString def, indef;
    bool removed = false;

    articles.getFemale(&def, &indef);
    mw->rb_fem->setText(i18nc("@label the gender of the word: female", "&female:\t")+def+" / "+indef);
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
    mw->male->setText(i18nc("@label the gender of the word: male", "&male:\t")+def+" / "+indef);
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
    mw->natural->setText(i18nc("@label the gender of the word: neutral", "&neutral:\t")+def+" / "+indef);
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
    s.setNum(entry->statisticCount());
    mw->progCount->setText(s);

    mw->rb_fem->setFocus();
}


void ArtQueryDlg::showSolution()
{
    resetQueryWidget(mw->male);
    resetQueryWidget(mw->rb_fem);
    resetQueryWidget(mw->natural);

    if (m_entry->exp->translation(Prefs::toIdentifier()).subType() == m_doc->wordTypes()->specialTypeNounMale()) {
        mw->male->setChecked(true);
        verifyButton(mw->male, true);
    } else if (m_entry->exp->translation(Prefs::toIdentifier()).type() == m_doc->wordTypes()->specialTypeNounFemale()) {
        mw->rb_fem->setChecked(true);
        verifyButton(mw->rb_fem, true);
    } else if (m_entry->exp->translation(Prefs::toIdentifier()).type() == m_doc->wordTypes()->specialTypeNounNeutral()) {
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
    if (m_entry->exp->translation(Prefs::toIdentifier()).subType() ==  m_doc->wordTypes()->specialTypeNounMale())
        known = mw->male->isChecked();
    else if (m_entry->exp->translation(Prefs::toIdentifier()).subType() == m_doc->wordTypes()->specialTypeNounFemale())
        known = mw->rb_fem->isChecked();
    else if (m_entry->exp->translation(Prefs::toIdentifier()).subType() == m_doc->wordTypes()->specialTypeNounNeutral())
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
    emit sigQueryChoice(SkipKnown);
}


void ArtQueryDlg::dontKnowClicked()
{
    emit sigQueryChoice(SkipUnknown);
}


void ArtQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}


#include "ArtQueryDlg.moc"


