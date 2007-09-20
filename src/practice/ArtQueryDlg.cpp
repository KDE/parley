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

    articles = m_doc->identifier(Prefs::toIdentifier()).article();

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->maleRadio, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->femaleRadio, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->neutralRadio, SIGNAL(clicked()), SLOT(verifyClicked()));

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
    mw->femaleRadio->setText(i18nc("@label the gender of the word: female", "&female:\t")+def+" / "+indef);
    mw->femaleRadio->setEnabled(!QString(def+indef).isEmpty());
    if (!removed && s.indexOf(def+' ') == 0) {
        s.remove(0, def.length()+1);
        removed = true;
    }
    if (!removed && s.indexOf(indef+' ') == 0) {
        s.remove(0, indef.length()+1);
        removed = true;
    }

    articles.getMale(&def, &indef);
    mw->maleRadio->setText(i18nc("@label the gender of the word: male", "&male:\t")+def+" / "+indef);
    mw->maleRadio->setEnabled(!QString(def+indef).isEmpty());
    if (!removed && s.indexOf(def+' ') == 0) {
        s.remove(0, def.length()+1);
        removed = true;
    }
    if (!removed && s.indexOf(indef+' ') == 0) {
        s.remove(0, indef.length()+1);
        removed = true;
    }

    articles.getNeutral(&def, &indef);
    mw->neutralRadio->setText(i18nc("@label the gender of the word: neutral", "&neutral:\t")+def+" / "+indef);
    mw->neutralRadio->setEnabled(!QString(def+indef).isEmpty());
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

    resetQueryWidget(mw->maleRadio);
    resetQueryWidget(mw->femaleRadio);
    resetQueryWidget(mw->neutralRadio);

    // As long as the buttons are AutoExclusive we cannot uncheck all.
    mw->maleRadio->setChecked(true);
    mw->maleRadio->setAutoExclusive ( false );
    mw->maleRadio->setChecked(false);
    mw->maleRadio->setAutoExclusive ( true );

    mw->dont_know->setFocus();
}


void ArtQueryDlg::showSolution()
{
    resetQueryWidget(mw->maleRadio);
    resetQueryWidget(mw->femaleRadio);
    resetQueryWidget(mw->neutralRadio);

    QString specialSubType = m_doc->wordTypes().specialSubType(m_entry->exp->translation(Prefs::toIdentifier()).type(), m_entry->exp->translation(Prefs::toIdentifier()).subType());

    if (specialSubType == m_doc->wordTypes().specialTypeNounMale()) {
        mw->maleRadio->setChecked(true);
        verifyButton(mw->maleRadio, true);
    } else if (specialSubType == m_doc->wordTypes().specialTypeNounFemale()) {
        mw->femaleRadio->setChecked(true);
        verifyButton(mw->femaleRadio, true);
    } else if (specialSubType == m_doc->wordTypes().specialTypeNounNeutral()) {
        mw->neutralRadio->setChecked(true);
        verifyButton(mw->neutralRadio, true);
    }
    mw->dont_know->setDefault(true);
}


void ArtQueryDlg::verifyClicked()
{
    QString specialSubType = m_doc->wordTypes().specialSubType(m_entry->exp->translation(Prefs::toIdentifier()).type(), m_entry->exp->translation(Prefs::toIdentifier()).subType());

    bool known = false;
    if (specialSubType ==  m_doc->wordTypes().specialTypeNounMale())
        known = mw->maleRadio->isChecked();
    else if (specialSubType == m_doc->wordTypes().specialTypeNounFemale())
        known = mw->femaleRadio->isChecked();
    else if (specialSubType == m_doc->wordTypes().specialTypeNounNeutral())
        known = mw->neutralRadio->isChecked();

    if (mw->femaleRadio->isChecked()) {
        verifyButton(mw->femaleRadio, known);
        resetQueryWidget(mw->maleRadio);
        resetQueryWidget(mw->neutralRadio);
    } else if (mw->maleRadio->isChecked()) {
        verifyButton(mw->maleRadio, known);
        resetQueryWidget(mw->femaleRadio);
        resetQueryWidget(mw->neutralRadio);
    } else if (mw->neutralRadio->isChecked()) {
        verifyButton(mw->neutralRadio, known);
        resetQueryWidget(mw->maleRadio);
        resetQueryWidget(mw->femaleRadio);
    }

    if (known)
        resultCorrect();
    else
        mw->dont_know->setDefault(true);
}


void ArtQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}


#include "ArtQueryDlg.moc"


