/***************************************************************************

                       query dialog for properties

    -----------------------------------------------------------------------

    begin         : Sun Apr 9 18:55:34 2000

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

#include "SimpleQueryDlg.h"

#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <KLocale>

#include <kvttablemodel.h>
#include <keduvocdocument.h>

SimpleQueryDlg::SimpleQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog("", doc, parent)
{
    mw = new Ui::SimpleQueryDlgForm();
    mw->setupUi(mainWidget());

    mw->stopPracticeButton->setIcon( KIcon("list-remove") );
    mw->editEntryButton->setIcon( KIcon("edit") );
    mw->verify->setIcon(KIcon("ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));
    connect(mw->editEntryButton, SIGNAL(clicked()), SLOT(editEntry()));

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));
    connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()));
    connect(mw->answerField, SIGNAL(textChanged()), SLOT(slotAnswerChanged()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    mw->know_it->setVisible(Prefs::skipKnownEnabled());
    mw->imageGraphicsView->setVisible(false);

    KConfigGroup cg(KGlobal::config(), "SimpleQueryDialog");
    restoreDialogSize(cg);
}


SimpleQueryDlg::~SimpleQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "SimpleQueryDialog");
    KDialog::saveDialogSize(cg);
}


void SimpleQueryDlg::setEntry(TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    mw->timebar->setEnabled(Prefs::practiceTimeout());
    mw->timelabel->setEnabled(Prefs::practiceTimeout());
    mw->queryField->setFont(Prefs::tableFont());
    mw->answerField->setFont(Prefs::tableFont());
    mw->answerField->setText("");

    int column = Prefs::solutionLanguage();

    QString s;
    switch (m_testType) {
    case Prefs::EnumTestType::SynonymTest: {
            mw->queryLabel->setText(i18n("Expression"));
            mw->instructionLabel->setText(i18n("Enter the synonym:"));
            setWindowTitle(i18n("Synonym Training"));
            answerstring = m_entry->entry()->translation(column).synonym();
            mw->queryField->setAlignment(Qt::AlignVCenter);
            mw->queryField->setText( m_entry->entry()->translation(column).text() );
            setQueryFieldWordwrap();
        }
        break;

    case Prefs::EnumTestType::AntonymTest: {
            mw->queryLabel->setText(i18n("Expression"));
            mw->instructionLabel->setText(i18n("Enter the antonym:"));
            setWindowTitle(i18n("Antonym Training"));
            answerstring = m_entry->entry()->translation(column).antonym();
            mw->queryField->setText( m_entry->entry()->translation(column).text() );
            setQueryFieldWordwrap();
        }
        break;

    case Prefs::EnumTestType::ParaphraseTest: {
            mw->queryLabel->setText(i18n("Paraphrase"));
            mw->instructionLabel->setText(i18n("Enter the word:"));
            setWindowTitle(i18n("Paraphrase Training"));
            mw->queryField->setText(m_entry->entry()->translation(column).paraphrase());
            answerstring = m_entry->entry()->translation(column).text();
            setQueryFieldWordwrap();
        }
        break;

    case Prefs::EnumTestType::ExampleTest: {
            mw->queryLabel->setText(i18n("Example sentence"));
            mw->instructionLabel->setText(i18n("Fill in the missing word:"));
            setWindowTitle(i18n("Example Training"));
            s = m_entry->entry()->translation(column).example();
            answerstring = m_entry->entry()->translation(column).text().simplified();
            int pos = -1;
            while ((pos = s.indexOf(answerstring)) >= 0) {
                s.remove(pos, answerstring.length());
                s.insert(pos, "<font color=\"#FF0000\"><b>...</b></font>");
            }
            mw->queryField->setText(s);
            setQueryFieldWordwrap();
        }
        break;
    }

    mw->verify->setDefault(true);
    s.setNum(entry->statisticCount());
    mw->progCount->setText(s);

    setWidgetStyle(mw->answerField, Default);
    mw->answerField->setFocus();

    imageShowFromEntry( mw->imageGraphicsView );
}


void SimpleQueryDlg::slotAnswerChanged()
{
    mw->verify->setDefault(true);
    setWidgetStyle(mw->answerField);
}


void SimpleQueryDlg::showMoreClicked()
{
    if (mw->answerField->text().length() < answerstring.length()) {
        mw->answerField->setText(answerstring.left(mw->answerField->text().length()+1));
        mw->dont_know->setDefault(true);
    }
    setWidgetStyle(mw->answerField);
    setAnswerTainted();
}


void SimpleQueryDlg::showSolution()
{
    mw->answerField->setText(answerstring);
    setWidgetStyle( mw->answerField, NegativeResult );
    setAnswerTainted();
    mw->dont_know->setDefault(true);
}


void SimpleQueryDlg::verifyClicked()
{
    double result = verifyAnswer(mw->answerField->text(), answerstring);
    if ( result == 1.0 ) {
        resultCorrect();
        emit showSolutionFinished();
    } else {
        setWidgetStyle( mw->answerField, NegativeResult );
        setAnswerTainted();
        mw->dont_know->setDefault(true);
    }
}


void SimpleQueryDlg::knowItClicked()
{
    skipKnown();
}


void SimpleQueryDlg::dontKnowClicked()
{
    skipUnknown();
}


void SimpleQueryDlg::setQueryFieldWordwrap()
{
    QFontMetrics fm(Prefs::tableFont());
    int w = fm.width(mw->queryField->text());
    int w2 = mainWidget()->width();
    mw->queryField->setAlignment(Qt::AlignVCenter);
    mw->queryField->setWordWrap(w > w2);
}

void SimpleQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}


#include "SimpleQueryDlg.moc"


