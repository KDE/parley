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
#include "prefs.h"

#include <kvttablemodel.h>
#include <keduvocdocument.h>

#include <KLocale>
#include <KDebug>
#include <KRandomSequence>

#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QKeyEvent>

MCQueryDlg::MCQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Multiple Choice"), doc, parent)
{
    mw = new Ui::MCQueryDlgForm();
    mw->setupUi(mainWidget());

    // continue button
    mw->continueButton->setIcon(KIcon("ok"));
    connect(mw->continueButton, SIGNAL(clicked()), SLOT(continueButtonClicked()));

    // stop practice
    mw->stopPracticeButton->setIcon(KIcon("list-remove"));
    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));

    // edit
    mw->editEntryButton->setIcon(KIcon("edit"));
    connect(mw->editEntryButton, SIGNAL(clicked()), SLOT(editEntry()));

    // skip known
    mw->know_it->setIcon(KIcon("go-next"));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));

    // skip unknown
    mw->dont_know->setIcon(KIcon("go-next"));
    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    // show solution
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    // sound buttons
    mw->audioPlayQuestionButton->setIcon(KIcon("media-playback-start"));
    connect(mw->audioPlayQuestionButton, SIGNAL(clicked()), SLOT(audioPlayFromIdentifier()));
    mw->audioChoiceButton1->setIcon(KIcon("media-playback-start"));
    mw->audioChoiceButton2->setIcon(KIcon("media-playback-start"));
    mw->audioChoiceButton3->setIcon(KIcon("media-playback-start"));
    mw->audioChoiceButton4->setIcon(KIcon("media-playback-start"));
    mw->audioChoiceButton5->setIcon(KIcon("media-playback-start"));

    // status displays and timer indicator
    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");
    mw->timebar->setVisible(Prefs::practiceTimeout() != Prefs::EnumPracticeTimeout::NoTimeout);
    mw->timelabel->setVisible(Prefs::practiceTimeout() != Prefs::EnumPracticeTimeout::NoTimeout);

    mw->know_it->setVisible(Prefs::skipKnownEnabled());
    mw->imageGraphicsView->setVisible(false);

    m_choiceRadioButtons << mw->choiceRadioButton1
        << mw->choiceRadioButton2
        << mw->choiceRadioButton3
        << mw->choiceRadioButton4
        << mw->choiceRadioButton5;

    foreach (QRadioButton* rb, m_choiceRadioButtons) {
        connect(rb, SIGNAL(clicked()), SLOT(verifyClicked()));
    }

    KConfigGroup cg(KGlobal::config(), "MCQueryDlg");
    restoreDialogSize(cg);
}


MCQueryDlg::~MCQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "MCQueryDlg");
    KDialog::saveDialogSize(cg);
    delete mw;
}


void MCQueryDlg::setEntry( TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    // Query cycle - how often did this entry show up
    mw->progCount->setText(QString::number(m_entry->statisticCount()));

    // general setup
    mw->show_all->setDefault(true);
    showContinueButton(false);

    // question
    mw->orgField->setFont(Prefs::tableFont());
    mw->orgField->setText(m_entry->exp->translation(Prefs::questionLanguage()).text());

    mw->audioPlayQuestionButton->setVisible( Prefs::practiceSoundEnabled()
        && !m_entry->exp->translation(Prefs::questionLanguage())
            .soundUrl().isEmpty());

    mw->audioChoiceButton1->setVisible(false);
    mw->audioChoiceButton2->setVisible(false);
    mw->audioChoiceButton3->setVisible(false);
    mw->audioChoiceButton4->setVisible(false);
    mw->audioChoiceButton5->setVisible(false);

    // answer and choices
    QString solution = m_entry->exp->translation(Prefs::solutionLanguage()).text();

    // gather some choices...
    QStringList choices;

    // the user supplied choices (edit entry -> choices)
    choices << m_entry->exp->translation(Prefs::solutionLanguage()).multipleChoice().choices();

    // always include false friend
    QString falseFriend = m_entry->exp->translation(Prefs::solutionLanguage())
        .falseFriend(Prefs::questionLanguage());
    if (!falseFriend.isEmpty()) {
        choices.append(falseFriend);
    }

    // create choices for the buttons: -solution -choices we have already
    choices << createAdditionalChoices(m_choiceRadioButtons.size() - 1 - choices.size());

    KRandomSequence randomSequence (QDateTime::currentDateTime().toTime_t());
    m_solution = randomSequence.getLong(m_choiceRadioButtons.size());

    for ( int i = 0; i < m_choiceRadioButtons.count(); i++ ) {
        QString choice;
        if ( i == m_solution ) {
            choice = solution;
        } else {
            if ( choices.size() > 0 ) {
                choice = choices.takeAt(randomSequence.getLong(choices.size()));
            } else {
                // should not happen...
                choice = "Parley";
            }
        }
        setWidgetStyle(m_choiceRadioButtons[i], Default);
        m_choiceRadioButtons[i]->setText(QString::number(i+1) + ": " + choice);
        m_choiceRadioButtons[i]->setShortcut(QString::number(i+1));
        m_choiceRadioButtons[i]->setFont(Prefs::tableFont());
    }

    // As long as the buttons are AutoExclusive we cannot uncheck all.
    m_choiceRadioButtons[0]->setChecked(true);
    m_choiceRadioButtons[0]->setAutoExclusive (false);
    m_choiceRadioButtons[0]->setChecked(false);
    m_choiceRadioButtons[0]->setAutoExclusive (true);

    mw->show_all->setFocus();

    imageShowFromEntry( mw->imageGraphicsView, entry );
}


void MCQueryDlg::showSolution()
{
    setWidgetStyle(m_choiceRadioButtons[m_solution], PositiveResult);

    m_choiceRadioButtons[m_solution]->setFocus();
    m_choiceRadioButtons[m_solution]->setChecked(true);

    showContinueButton(true);

    if ( !answerTainted() ) {
        setAnswerTainted();
        resultWrong();
    }
}


void MCQueryDlg::verifyClicked()
{
    if ( m_choiceRadioButtons[m_solution]->isChecked() ) {
        // correct
        setWidgetStyle(m_choiceRadioButtons[m_solution], PositiveResult);
        resultCorrect();
        showContinueButton(true);
        mw->status->setText(
                getOKComment((int)(((double)mw->countbar->value())
                    /mw->countbar->maximum() * 100.0)));
    } else {
        // wrong answer
        foreach ( QRadioButton* rb, m_choiceRadioButtons ) {
            if ( rb->isChecked() ) {
                setWidgetStyle(rb, NegativeResult);
            }
        }
        if ( !answerTainted() ) {
            setAnswerTainted();
            resultWrong();
        }
        mw->dont_know->setDefault(true);
        mw->status->setText(
                getNOKComment((int)(((double)mw->countbar->value())
                    /mw->countbar->maximum() * 100.0)));
    }
    ///@todo move the status bar stuff either in or out of the base class
}


void MCQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}

void MCQueryDlg::showContinueButton(bool show)
{
    if ( show ) {
        if(!answerTainted()) {
            // don't show the solution
            if ( Prefs::showSolutionTime() < 0 ) {
                mw->continueButton->click();
                return;
            }
        }
    }

    mw->know_it->setVisible(!show);
    mw->dont_know->setVisible(!show);
    mw->show_all->setVisible(!show);

    mw->continueButton->setVisible(show);

    if ( show ) {
        stopAnswerTimer();
        mw->continueButton->setDefault(true);
        startShowSolutionTimer();

        // enable the sound for the solution. eventually all entries with sound should get their button enabled.
        if ( Prefs::practiceSoundEnabled() ) {
            if ( !m_entry->exp->translation(Prefs::solutionLanguage())
                .soundUrl().isEmpty()) {
                QList<QPushButton*> audioButtons;
                audioButtons << mw->audioChoiceButton1
                    << mw->audioChoiceButton2
                    << mw->audioChoiceButton3
                    << mw->audioChoiceButton4
                    << mw->audioChoiceButton5;
                audioButtons[m_solution]->setVisible(true);
                connect(audioButtons[m_solution], SIGNAL(clicked()), SLOT(audioPlayToIdentifier()));
            }
        }
    } else {
        mw->dont_know->setDefault(true);
    }
}


QStringList MCQueryDlg::createAdditionalChoices(int numberChoices)
{
    QStringList choices;

    KRandomSequence randomSequence (QDateTime::currentDateTime().toTime_t());

    if (m_doc->entryCount() <= numberChoices) {
        for (int i = choices.count(); i < m_doc->entryCount(); ++i) {
            KEduVocExpression *act = m_doc->entry(i);

            if (act != m_entry->exp) {
                choices.append(act->translation(Prefs::solutionLanguage()).text());
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;

        int count = numberChoices;
        int numNonEmptyEntries = 0;

        // find out if we got enough non-empty entries to fill all the options
        for(int i = 0; i < m_doc->entryCount(); i++) {
            if(!m_doc->entry(i)->translation(Prefs::solutionLanguage()).text().isEmpty())
                numNonEmptyEntries++;
            if(numNonEmptyEntries >= numberChoices)
                break;
        }

        // gather random expressions for the choice
        while (count > 0) {
            int nr;
            // if there are enough non-empty fields, fill the options only with those
            if(numNonEmptyEntries >= numberChoices) {
                do {
                    nr = randomSequence.getLong(m_doc->entryCount());
                } while (m_doc->entry(nr)->translation(Prefs::solutionLanguage()).text().isEmpty());
            } else {
                nr = randomSequence.getLong(m_doc->entryCount());
            }
            // append if new expr found
            bool newex = true;
            for (int i = 0; newex && i < exprlist.count(); i++) {
                if (exprlist[i] == m_doc->entry(nr))
                    newex = false;
            }
            if (newex && m_entry->exp != m_doc->entry(nr)) {
                count--;
                exprlist.append(m_doc->entry(nr));
            }
        }

        for (int i = 0; i < exprlist.count(); i++) {
            choices.append(exprlist[i]->translation(Prefs::solutionLanguage()).text());
        }
    }

    return choices;
}

#include "MCQueryDlg.moc"

