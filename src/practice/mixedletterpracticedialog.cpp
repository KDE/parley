/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mixedletterpracticedialog.h"

#include <keduvocdocument.h>

#include <KLocale>
#include <KRandomSequence>
#include <QGraphicsItem>
#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

MixedLetterPracticeDialog::MixedLetterPracticeDialog(KEduVocDocument *doc, QWidget *parent)
    :PracticeDialog(i18n("Image Practice"), doc, parent), Ui::MixedLetterPracticeDialog()
{
    setupUi(mainWidget());

    stopPracticeButton->setIcon( KIcon("list-remove") );
    verifySolutionButton->setIcon(KIcon("dialog-ok"));

    connect(stopPracticeButton, SIGNAL(clicked()), SLOT(close()));

    connect(verifySolutionButton, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(showSolutionButton, SIGNAL(clicked()), SLOT(showSolution()));
    connect(answerLineEdit, SIGNAL(textEdited(const QString&)), SLOT(slotAnswerChanged()));

    countbar->setFormat("%v/%m");
    timeProgressBar->setFormat("%v");

    verifySolutionButton->setIcon( KIcon("dialog-ok") );

    mixedLettersGraphicsView->setScene(new QGraphicsScene(mixedLettersGraphicsView));
    imageGraphicsView->setVisible(false);

    KConfigGroup cg(KGlobal::config(), "MixedLetterPracticeDlg");
    restoreDialogSize(cg);
}


MixedLetterPracticeDialog::~MixedLetterPracticeDialog()
{
    KConfigGroup cg(KGlobal::config(), "MixedLetterPracticeDlg");
    KDialog::saveDialogSize(cg);
}


void MixedLetterPracticeDialog::setEntry(TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    timeProgressBar->setEnabled(Prefs::practiceTimeout());
    timelabel->setEnabled(Prefs::practiceTimeout());
    answerLineEdit->setFont(Prefs::tableFont());
    answerLineEdit->setText("");

    verifySolutionButton->setDefault(true);

    progCount->setText( QString::number(entry->statisticCount()) );

    setWidgetStyle(answerLineEdit);
    answerLineEdit->setFocus();

    QString original = m_entry->entry()->translation(Prefs::questionLanguage())->text();
    originalLabel->setText( original );

    QString solution = m_entry->entry()->translation(Prefs::solutionLanguage())->text();

    // remove old items
    foreach ( QGraphicsItem* item, mixedLettersGraphicsView->scene()->items() ) {
        mixedLettersGraphicsView->scene()->removeItem(item);
        delete item;
    }
    m_answerTextItems.clear();

    KRandomSequence random = KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    QList<int> positions;
    for ( int i = 0; i < solution.length(); i++ ) {
        positions.append( (mixedLettersGraphicsView->width()-20) * i/solution.length()  + 10 );
    }

    for ( int i = 0; i < solution.length(); i++ ) {
        QGraphicsTextItem* textItem = new QGraphicsTextItem( QString(solution[i]) );
        textItem->translate( positions.takeAt( random.getLong(positions.size()) ), mixedLettersGraphicsView->height()/4 - 5 + random.getLong( mixedLettersGraphicsView->height()/2 ) );
        m_answerTextItems.append(textItem);
        mixedLettersGraphicsView->scene()->addItem( textItem );
    }

    imageShowFromEntry(imageGraphicsView);
}


void MixedLetterPracticeDialog::slotAnswerChanged()
{
    QString solution = m_entry->entry()->translation(Prefs::solutionLanguage())->text();
    for ( int i = 0; i < solution.length(); i++ ) {
        if ( answerLineEdit->text()[i] == solution[i] ) {
            m_answerTextItems[i]->setHtml("<b><font color=\"#188C18\">" + solution[i] + "</font></b>");
        } else {
//             if ( i >= answerLineEdit->text().length() ) {
                // no input yet
            m_answerTextItems[i]->setHtml(QString(solution[i]));
//             } else {
                // wrong
//                 m_answerTextItems[i]->setHtml("<b><font color=\"#FF0000\">" + solution[i] + "</font></b>");
//             }
        }
    }
    setWidgetStyle(answerLineEdit);
    verifySolutionButton->setDefault(true);
}


void MixedLetterPracticeDialog::showSolution()
{
    answerLineEdit->setText( m_entry->entry()->translation(Prefs::solutionLanguage())->text() );
    setWidgetStyle( answerLineEdit, PositiveResult );
    setAnswerTainted();
    verifySolutionButton->setFocus();
}


void MixedLetterPracticeDialog::verifyClicked()
{
    double result = verifyAnswer(answerLineEdit->text(), m_entry->entry()->translation(Prefs::solutionLanguage())->text());
    if ( result == 1.0 ) {
        resultCorrect();
        emit showSolutionFinished();
    } else {
        setWidgetStyle( answerLineEdit, NegativeResult );
        setAnswerTainted();
    }
}

void MixedLetterPracticeDialog::setProgressCounter(int current, int total)
{
    countbar->setMaximum(total);
    countbar->setValue(current);
}

QProgressBar * MixedLetterPracticeDialog::timebar()
{
    return timeProgressBar;
}

#include "mixedletterpracticedialog.moc"


