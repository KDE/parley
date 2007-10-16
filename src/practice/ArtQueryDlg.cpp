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

ArtQueryDlg::ArtQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18nc("@title:window", "Article Training"), doc, parent)
{
    mw = new Ui::ArtQueryDlgForm();
    mw->setupUi(mainWidget());

    mw->stopPracticeButton->setIcon( KIcon("list-remove") );
    mw->editEntryButton->setIcon( KIcon("edit") );
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));
    connect(mw->editEntryButton, SIGNAL(clicked()), SLOT(editEntry()));

    articles = m_doc->identifier(Prefs::solutionLanguage()).article();

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->maleRadio, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->femaleRadio, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->neutralRadio, SIGNAL(clicked()), SLOT(verifyClicked()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    mw->know_it->setVisible(Prefs::skipKnownEnabled());
    mw->imageGraphicsView->setVisible(false);

    setEntry(m_entryManager->nextEntry());
    setProgressCounter(m_entryManager->totalEntryCount()-m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());

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

    mw->timebar->setVisible(Prefs::practiceTimeout() != Prefs::EnumPracticeTimeout::NoTimeout);
    mw->timelabel->setVisible(Prefs::practiceTimeout() != Prefs::EnumPracticeTimeout::NoTimeout);
    mw->show_all->setDefault(true);

    QString s;
    s = m_entry->exp->translation(Prefs::solutionLanguage()).text().simplified();

    QString def, indef;
    bool removed = false;

    def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine );
    indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine );
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

    def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine );
    indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine );
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

    def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neutral );
    indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neutral );
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

    setWidgetStyle(mw->maleRadio);
    setWidgetStyle(mw->femaleRadio);
    setWidgetStyle(mw->neutralRadio);

    // As long as the buttons are AutoExclusive we cannot uncheck all.
    mw->maleRadio->setChecked(true);
    mw->maleRadio->setAutoExclusive ( false );
    mw->maleRadio->setChecked(false);
    mw->maleRadio->setAutoExclusive ( true );

    mw->dont_know->setFocus();

    imageShowFromEntry( mw->imageGraphicsView, entry );
}


void ArtQueryDlg::showSolution()
{
    setWidgetStyle(mw->maleRadio);
    setWidgetStyle(mw->femaleRadio);
    setWidgetStyle(mw->neutralRadio);

    QString specialSubType = m_doc->wordTypes().specialSubType(m_entry->exp->translation(Prefs::solutionLanguage()).type(), m_entry->exp->translation(Prefs::solutionLanguage()).subType());

    if (specialSubType == m_doc->wordTypes().specialTypeNounMale()) {
        mw->maleRadio->setChecked(true);
        setWidgetStyle(mw->maleRadio, PositiveResult);
    } else if (specialSubType == m_doc->wordTypes().specialTypeNounFemale()) {
        mw->femaleRadio->setChecked(true);
        setWidgetStyle(mw->femaleRadio, PositiveResult);
    } else if (specialSubType == m_doc->wordTypes().specialTypeNounNeutral()) {
        mw->neutralRadio->setChecked(true);
        setWidgetStyle(mw->neutralRadio, PositiveResult);
    }
    mw->dont_know->setDefault(true);
    setAnswerTainted();
}


void ArtQueryDlg::verifyClicked()
{
    QString specialSubType = m_doc->wordTypes().specialSubType(m_entry->exp->translation(Prefs::solutionLanguage()).type(), m_entry->exp->translation(Prefs::solutionLanguage()).subType());

    bool correct = false;

    if (specialSubType ==  m_doc->wordTypes().specialTypeNounMale()) {
        if ( mw->maleRadio->isChecked() ) {
            setWidgetStyle(mw->maleRadio, PositiveResult);
            correct = true;
        } else {
            setWidgetStyle(mw->maleRadio, NegativeResult);
        }
    }
    if (specialSubType == m_doc->wordTypes().specialTypeNounFemale()) {
        if ( mw->femaleRadio->isChecked() ) {
            setWidgetStyle(mw->femaleRadio, PositiveResult);
            correct = true;
        } else {
            setWidgetStyle(mw->femaleRadio, NegativeResult);
        }
    }
    if (specialSubType == m_doc->wordTypes().specialTypeNounNeutral()) {
        if ( mw->neutralRadio->isChecked() ) {
            setWidgetStyle(mw->neutralRadio, PositiveResult);
            correct = true;
        } else {
            setWidgetStyle(mw->neutralRadio, NegativeResult);
        }
    }

    if (correct) {
        resultCorrect();
        emit nextEntry();
    } else {
        mw->dont_know->setDefault(true);
    }
}


void ArtQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}


#include "ArtQueryDlg.moc"


