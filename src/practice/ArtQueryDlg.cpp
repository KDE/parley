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

    mw->stopPracticeButton->setIcon( KIcon("process-stop") );
    mw->editEntryButton->setIcon( KIcon("document-properties") );
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

    mw->timebar->setVisible(Prefs::practiceTimeout());
    mw->timelabel->setVisible(Prefs::practiceTimeout());
    mw->show_all->setDefault(true);

    QString def, indef;

    KEduVocArticle::ArticleNumber singular = KEduVocArticle::Singular;
    KEduVocArticle::ArticleDefiniteness definite = KEduVocArticle::Definite;
    KEduVocArticle::ArticleDefiniteness indefinite = KEduVocArticle::Indefinite;
    KEduVocArticle::ArticleGender male = KEduVocArticle::Masculine;
    KEduVocArticle::ArticleGender female = KEduVocArticle::Feminine;
    KEduVocArticle::ArticleGender neutral = KEduVocArticle::Neutral;

    // set the word (possibly without the article)
    QString noun = m_entry->exp->translation(Prefs::solutionLanguage()).text();
    // strip the article
    ///@todo
    int pos;
    foreach(QString word, noun.split(QRegExp("\\W"), QString::SkipEmptyParts) ) {
        if (articles.isArticle(word)) {
            while ((pos = noun.indexOf(word)) >= 0) {
                noun.remove(pos, word.length());
            }
        }
    }
//     QString firstPart = noun.substring(
//     if (articles.isArticle(firstPart)){
        //remove
//     }

    mw->orgField->setText(noun);
    mw->progCount->setText(QString::number(entry->statisticCount()));


    // set the choices
    if(articles.isEmpty()) {
        mw->maleRadio->setText(i18nc("@label the gender of the word: male", "&male"));
        mw->femaleRadio->setText(i18nc("@label the gender of the word: female", "&female"));
        mw->neutralRadio->setText(i18nc("@label the gender of the word: neutral", "&neutral"));
    } else {
        def = articles.article( singular, definite, male );
        indef = articles.article( singular, indefinite, male );
        bool male = !(def.isEmpty() && indef.isEmpty());
        QString article;
        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        mw->maleRadio->setText(i18nc("@label the gender of the word: male", "&male:\t") + article);

        def = articles.article( singular, definite, female );
        indef = articles.article( singular, indefinite, female );
        bool female = !(def.isEmpty() && indef.isEmpty());
        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        mw->femaleRadio->setText(i18nc("@label the gender of the word: female", "&female:\t") + article);

        def = articles.article( singular, definite, neutral );
        indef = articles.article( singular, indefinite, neutral );
        bool neutral = !(def.isEmpty() && indef.isEmpty());
        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        if (!neutral && male && female) {
            mw->neutralRadio->setVisible(false);
        } else {
            mw->neutralRadio->setText(i18nc("@label the gender of the word: neutral", "&neutral:\t") + article);
            mw->neutralRadio->setVisible(true);
        }
    }

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
        emit showSolutionFinished();
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


