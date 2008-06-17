/***************************************************************************

                    dialog when in query mode

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "writtenpracticedialog.h"

#include <keduvocdocument.h>
#include <keduvocwordtype.h>

#include <KComboBox>
#include <KLineEdit>
#include <KLocale>

#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRegExp>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QList>
#include <QTimer>

WrittenPracticeDialog::WrittenPracticeDialog(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Written Practice"), doc, parent)
{
    mw = new Ui::WrittenPracticeDialog();
    mw->setupUi(mainWidget());

    mw->continueButton->setIcon(KIcon("dialog-ok"));
    connect(mw->continueButton, SIGNAL(clicked()),  SLOT(continueButtonClicked()));

    mw->stopPracticeButton->setIcon( KIcon("process-stop") );
    mw->verify->setIcon(KIcon("dialog-ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    mw->show_more->setIcon(KIcon("help-hint"));
    mw->show_all->setIcon(KIcon("games-solve"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));

    connect(mw->typeCheckBox, SIGNAL(toggled(bool)), SLOT(slotTypeClicked(bool)));
    connect(mw->commentCheckBox, SIGNAL(toggled(bool)), SLOT(slotRemClicked(bool)));
    connect(mw->falsefriendCheckBox, SIGNAL(toggled(bool)), SLOT(slotFalseFriendClicked(bool)));
    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));
    connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()));

    showContinueButton(false);

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    connect(mw->answerLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotAnswerChanged(const QString&)));
//     connect(mw->answerLineEdit, SIGNAL(lostFocus()), SLOT(slotAnswerLostFocus()));

    mw->answerLineEdit->setFont(Prefs::tableFont());

    mw->imageGraphicsView->setVisible(false);

    mw->audioPlayQuestionButton->setIcon(KIcon("media-playback-start"));
    mw->audioPlaySolutionButton->setIcon(KIcon("media-playback-start"));
    connect(mw->audioPlayQuestionButton, SIGNAL(clicked()), SLOT(audioPlayFromIdentifier()));
    connect(mw->audioPlaySolutionButton, SIGNAL(clicked()), SLOT(audioPlayToIdentifier()));

    KConfigGroup cg(KGlobal::config(), "RandomQueryDialog");
    restoreDialogSize(cg);
}


WrittenPracticeDialog::~WrittenPracticeDialog()
{
    KConfigGroup cg(KGlobal::config(), "RandomQueryDialog");
    KDialog::saveDialogSize(cg);
    delete mw;
}


void WrittenPracticeDialog::setEntry( TestEntry* entry )
{
    PracticeDialog::setEntry(entry);
    showContinueButton(false);

//     QString trans = entry->entry()->translation( Prefs::solutionLanguage() ).text();
//     if (Prefs::split())
//         translations = extractTranslations(trans);
//     else
//         translations = QStringList(trans);

    mw->timebar->setVisible(Prefs::practiceTimeout());
    mw->timelabel->setVisible(Prefs::practiceTimeout());

    mw->answerLineEdit->clear();
    mw->answerLineEdit->setFont(Prefs::tableFont());
    setWidgetStyle(mw->answerLineEdit, Default);

    mw->verify->setEnabled(true);
    mw->orgField->setFont(Prefs::tableFont());
    mw->orgField->setText(entry->entry()->translation(Prefs::questionLanguage())->text());
    mw->show_all->setDefault(true);

    mw->progCount->setText( QString::number(entry->statisticCount()) );

    mw->commentLabel->hide();
    mw->falseFriendLabel->hide();
    mw->typeLabel->hide();

    mw->commentCheckBox->setChecked(false);
    mw->falsefriendCheckBox->setChecked(false);
    mw->typeCheckBox->setChecked(false);
    setHintFields();

    mw->status->clear();
    suggestion_hint = false;

//     if (Prefs::suggestions()) {
//         transCombos.at(0)->setFocus();
//     }
//     else {
//         transFields.at(0)->setFocus();
//     }
    mw->answerLineEdit->setFocus();

    mw->audioPlayQuestionButton->setVisible( Prefs::practiceSoundEnabled() &&
        !entry->entry()->translation(Prefs::questionLanguage())->soundUrl().isEmpty());

    mw->audioPlaySolutionButton->setVisible( Prefs::practiceSoundEnabled() &&
        !entry->entry()->translation(Prefs::solutionLanguage())->soundUrl().isEmpty());

    mw->correctionLabel->setText(QString());

    imageShowFromEntry( mw->imageGraphicsView );
}


void WrittenPracticeDialog::verifyClicked()
{
    QString userAnswer = mw->answerLineEdit->text();

    double result = verifyAnswer(userAnswer);
    TestEntry::ErrorTypes errors = m_entry->lastErrors();

//     kDebug() << "verifying grade: " << result << " errors: " << errors;

    QString errorText;

    if ( m_entry->lastErrors() & TestEntry::Correct ) {
        errorText.append(i18n("<font color=\"#188C18\">You are right!</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::CapitalizationMistake ) {
        errorText.append(i18n("<font color=\"#222978\">Watch your capitalization!</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::SpellingMistake ) {
        errorText.append(i18n("<font color=\"#8C1818\">I think you made a spelling mistake. But the word is right.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::UnknownMistake ) {
        errorText.append(i18n("<font color=\"#8C1818\">You made a mistake.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::AccentMistake ) {
        errorText.append(i18n("<font color=\"#222978\">Your accentuation was wrong.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::ArticleWrong ) {
        errorText.append(i18n("<font color=\"#222978\">The article is wrong.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::ArticleMissing ) {
        errorText.append(i18n("<font color=\"#222978\">The article is missing.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::FalseFriend ) {
        errorText.append(i18n("<font color=\"#8C1818\">Watch out! This is a false friend!</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::Synonym ) {
        errorText.append(i18n("<font color=\"#8C1818\">Great, you entered a synonym.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::Empty ) {
        errorText.append(i18n("<font color=\"#8C8C18\">Please enter something for me to check.</font>") + '\n');
    }
    if ( m_entry->lastErrors() & TestEntry::UnrelatedWord ) {
        errorText.append(i18n("<font color=\"#8C1818\">I know that word, but are you sure it is the right one here?</font>") + '\n');
    }

    if ( errorText.isEmpty() ) {
        errorText.append(i18n("There is a mistake.") + ' ' + QString::number(m_entry->lastErrors()));
    }

    if ( Prefs::ignoreCapitalizationMistakes() ) {
        if (m_entry->lastErrors() == TestEntry::CapitalizationMistake && !userAnswer.isEmpty()) {
            result = 1.0;
            errorText.append(" (" + userAnswer + ')');
        }
    }

    if ( Prefs::ignoreAccentMistakes() ) {
        if (m_entry->lastErrors() == TestEntry::AccentMistake && !userAnswer.isEmpty()) {
            result = 1.0;
            errorText.append(" (" + userAnswer + ')');
        }
    }

    if ( result == 1.0 ) {
        // will result in wrong if tainted
        resultCorrect();
        showContinueButton(true);
    } else {
        if(!userAnswer.isEmpty())
            errorText.append(" (" + userAnswer + ')');
        setWidgetStyle(mw->answerLineEdit, NegativeResult);
        // the percentage is very vague anyway, don't show floats...
//         mw->status->setText(i18n("That was wrong. (%1%)", (int)(result*100)));
        mw->status->setText(i18n("The answer was wrong."));
        mw->show_all->setDefault(true);
        setAnswerTainted();
    }
    mw->correctionLabel->setText(errorText);
}


// void WrittenPracticeDialog::showMoreClicked()
// {
//     setAnswerTainted();
//     if (Prefs::suggestions()) {
//         for (int i = 0; i < translations.count(); i ++) {
//             KComboBox* combo = transCombos.at(i);
//             if ( verifyAnswer(combo->currentText(), translations[i]) != 1.0 ) {
//                 int length = combo->currentText().length() + 1;
//                 if (length >= translations[i].length()) {
//                     combo->setEditText(translations[i]);
//                     verifyField(combo->lineEdit(), translations[i]);
//                     mw->verify->setEnabled(false);
//                 } else {
//                     combo->setEditText(translations[i].left(length));
//                     setWidgetStyle(combo->lineEdit());
//                 }
//                 mw->dont_know->setDefault(true);
//                 break;
//             }
//         }
//     } else {
//         for (int i = 0; i < translations.count(); i ++) {
//             KLineEdit* field = transFields.at(i);
//             if ( verifyAnswer(field->text(), translations[i]) != 1.0 ) {
//                 int length = 1;
//                 while ( field->text().startsWith( translations[i].left(length) ) ) {
//                     kDebug() << " length " << length << "text " << field->text();
//                     length++;
//                 }
//                 if (length >= translations[i].length()) {
//                     field->setText(translations[i]);
//                     verifyField(field, translations[i]);
//                     showContinueButton(true);
//                 } else {
//                     field->setText(translations[i].left(length));
//                     setWidgetStyle(field);
//                     mw->verify->setDefault(true);
//                     field->setFocus();
//                 }
//                 break;
//             }
//         }
//     }
//     mw->status->clear();
//     suggestion_hint = false;
// }

void WrittenPracticeDialog::showMoreClicked()
{
    QString solution = m_entry->entry()->translation(Prefs::solutionLanguage())->text();
    double result = verifyAnswer(mw->answerLineEdit->text());
    if ( result == 1.0 ) {
        resultCorrect();
        showContinueButton(true);
        return;
    }

    setAnswerTainted();

    int length = 1;
    while ( mw->answerLineEdit->text().startsWith( solution.left(length) ) ) {
        length++;
        if(length >= solution.length()) {
            break;
        }
    }
    if (length >= solution.length()) {
        setWidgetStyle(mw->answerLineEdit, PositiveResult);
        mw->answerLineEdit->setText(solution);
        showContinueButton(true);
    } else {
        mw->answerLineEdit->setText(solution.left(length));
        mw->verify->setDefault(true);
        mw->answerLineEdit->setFocus();
        setWidgetStyle(mw->answerLineEdit, HintStyle);
    }

    // get the hint up to date
    verifyAnswer(mw->answerLineEdit->text());
    mw->correctionLabel->setText(""); ///@todo

    mw->status->clear();
    suggestion_hint = false;
}

void WrittenPracticeDialog::showSolution()
{
    setAnswerTainted();
    resultWrong();
    showContinueButton(true);

    mw->status->clear();
    suggestion_hint = false;
}


// void WrittenPracticeDialog::slotTransChanged(const QString&)
// {
//     mw->verify->setDefault(true);
//     bool suggestions = Prefs::suggestions();
//     KComboBox* combo = qobject_cast<KComboBox*>(sender());
//     KLineEdit* senderedit = qobject_cast<KLineEdit*>(sender());
//
//     if (suggestions && combo) {
//         QLineEdit* edit = qobject_cast<QLineEdit*>(combo->lineEdit());
//         if ( !edit ) {
//             return; // cast failed
//         }
//         setWidgetStyle(edit);
//         suggestion_hint = ! edit->text().isEmpty() && edit->text().length() <= 10;
//         if (suggestion_hint)
//             mw->status->setText(i18n("Press F5 for a list of translations starting with '%1'\n"
//                                        "Press F6 for a list of translations containing '%1'", edit->text()));
//         else {
//             mw->status->clear();
//         }
//     } else {
//         if ( (!suggestions) && senderedit) {
//             setWidgetStyle(senderedit);
//         }
//     }
// }

// void WrittenPracticeDialog::slotTransLostFocus()
// {
//     if (suggestion_hint) {
//         mw->status->clear();
//     }
//     suggestion_hint = false;
// }



void WrittenPracticeDialog::slotAnswerChanged(const QString&)
{
    mw->verify->setDefault(true);
    setWidgetStyle(mw->answerLineEdit, Default);
}




void WrittenPracticeDialog::setHintFields()
{
    bool hasComment =
        !m_entry->entry()->translation(Prefs::questionLanguage())->comment().isEmpty();
    mw->commentLabel->setVisible(hasComment);
    mw->commentCheckBox->setVisible(hasComment);

//     bool hasFalseFriend = !m_entry->entry()->translation(Prefs::questionLanguage())->falseFriend(Prefs::solutionLanguage()).isEmpty();
//     mw->falseFriendLabel->setVisible(hasFalseFriend);
//     mw->falsefriendCheckBox->setVisible(hasFalseFriend);

    bool hasType = m_entry->entry()->translation(Prefs::questionLanguage())->wordType() != 0;
    mw->typeLabel->setVisible(hasType);
    mw->typeCheckBox->setVisible(hasType);

    // hide an empty hint box
    mw->hintGroupBox->setVisible((hasComment||hasType));
}


// void WrittenPracticeDialog::slotFalseFriendClicked(bool show)
// {
//     if ( show ) {
//         mw->falseFriendLabel->setText(m_entry->entry()->translation(Prefs::questionLanguage())->falseFriend(Prefs::solutionLanguage()));
//     } else {
//         mw->falseFriendLabel->setText(QString());
//     }
// }

void WrittenPracticeDialog::slotRemClicked(bool show)
{
    if ( show ) {
        mw->commentLabel->setText(m_entry->entry()->translation(Prefs::questionLanguage())->comment());
    } else {
        mw->commentLabel->setText(QString());
    }
}

void WrittenPracticeDialog::slotTypeClicked(bool show)
{
    if ( show ) {
        mw->typeLabel->setText(m_entry->entry()->translation(Prefs::questionLanguage())->wordType()->name());
    } else {
        mw->typeLabel->setText(QString());
    }
}


void WrittenPracticeDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        skipUnknown();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (mw->dont_know->isDefault()) {
            skipUnknown();
        } else if (mw->know_it->isDefault()) {
            skipKnown();
        } else if (mw->show_all->isDefault()) {
            showSolution();
        } else if (mw->verify->isDefault()) {
            verifyClicked();
        } else if (mw->continueButton->isDefault()) {
            continueButtonClicked();
        }
        break;

    default:
        e->ignore();
        break;
    }
}

void WrittenPracticeDialog::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}

void WrittenPracticeDialog::showContinueButton(bool show)
{
    if ( show ) {
        // after correct answer auto advance after x seconds...
        if(!answerTainted()) {
            // don't show the solution
            if ( !Prefs::showSolutionAfterAnswer() ) {
                mw->continueButton->click();
                return;
            }
        }
    }

    mw->dont_know->setVisible(!show);
    mw->know_it->setVisible(!show && Prefs::skipKnownEnabled());
    mw->show_more->setVisible(!show && Prefs::showMore());
    mw->show_all->setVisible(!show);
    mw->verify->setVisible(!show);

    mw->continueButton->setVisible(show);

    if ( show ) {
        stopAnswerTimer();
        mw->answerLineEdit->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->text());
        mw->continueButton->setDefault(true);
        mw->continueButton->setFocus();
        setWidgetStyle(mw->answerLineEdit, PositiveResult);
        mw->answerLineEdit->setReadOnly(true);

        mw->status->setText(i18n("Well done, you knew the correct answer."));

        if ( Prefs::practiceSoundEnabled() ) {
            audioPlayToIdentifier();
        }
        if ( !answerTainted() ) {
            if ( Prefs::showSolutionAfterAnswerTime() > 0 ) {
                startShowSolutionTimer();
            }
        }
    } else {
        mw->verify->setDefault(true);
        mw->answerLineEdit->setReadOnly(false);
    }
}


#include "writtenpracticedialog.moc"
