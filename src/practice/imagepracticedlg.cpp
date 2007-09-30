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

#include "imagepracticedlg.h"

#include <kvttablemodel.h>
#include <keduvocdocument.h>

#include <KLocale>
#include <KRandomSequence>
#include <QGraphicsItem>
#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

ImagePracticeDlg::ImagePracticeDlg(KEduVocDocument *doc, QWidget *parent)
    :PracticeDialog(i18n("Image Practice"), doc, parent), Ui::ImagePracticeDlg()
{
    setupUi(mainWidget());

    stopPracticeButton->setIcon( KIcon("list-remove") );
    verifySolutionButton->setIcon(KIcon("ok"));

    connect(stopPracticeButton, SIGNAL(clicked()), SLOT(close()));

    connect(verifySolutionButton, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(showSolutionButton, SIGNAL(clicked()), SLOT(showSolution()));
    connect(answerLineEdit, SIGNAL(textEdited(const QString&)), SLOT(slotAnswerChanged()));

    countbar->setFormat("%v/%m");
    timeProgressBar->setFormat("%v");

    verifySolutionButton->setIcon( KIcon("ok") );

    imageGraphicsView->setScene(new QGraphicsScene());

    KConfigGroup cg(KGlobal::config(), "ImagePracticeDlg");
    restoreDialogSize(cg);
}


ImagePracticeDlg::~ImagePracticeDlg()
{
    KConfigGroup cg(KGlobal::config(), "ImagePracticeDlg");
    KDialog::saveDialogSize(cg);
}


void ImagePracticeDlg::setEntry(TestEntry* entry)
{
    m_wrongAnswer = false;
    PracticeDialog::setEntry(entry);

    timeProgressBar->setEnabled(Prefs::showCounter());
    timelabel->setEnabled(Prefs::showCounter());
    answerLineEdit->setFont(Prefs::tableFont());
    answerLineEdit->setText("");

    int identifier = Prefs::toIdentifier();

    verifySolutionButton->setDefault(true);

    progCount->setText( QString::number(entry->statisticCount()) );

    resetQueryWidget(answerLineEdit);
    answerLineEdit->setFocus();

    QString original = m_entry->exp->translation(Prefs::fromIdentifier()).text();
    originalLabel->setText( original );

    QString translation = m_entry->exp->translation(Prefs::toIdentifier()).text();

    // remove old items
    foreach ( QGraphicsItem* item, imageGraphicsView->scene()->items() ) {
        imageGraphicsView->scene()->removeItem(item);
        delete item;
    }
    m_answerTextItems.clear();

    KRandomSequence random = KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    for ( int i = 0; i < translation.length(); i++ ) {
        QGraphicsTextItem* textItem = new QGraphicsTextItem( QString(translation[i]) );
        textItem->translate( 10 + random.getLong(imageGraphicsView->width() - 20 ), 10 + random.getLong(imageGraphicsView->height() -20 ) );
        m_answerTextItems.append(textItem);
        imageGraphicsView->scene()->addItem( textItem );
    }
}


void ImagePracticeDlg::slotAnswerChanged()
{
    QString solution = m_entry->exp->translation(Prefs::toIdentifier()).text();
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
    resetQueryWidget(answerLineEdit);
    verifySolutionButton->setDefault(true);
}


void ImagePracticeDlg::showSolution()
{
    answerLineEdit->setText( m_entry->exp->translation(Prefs::toIdentifier()).text() );
    verifyField(answerLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).text());
    verifySolutionButton->setFocus();
}


void ImagePracticeDlg::verifyClicked()
{
    if (verifyField(answerLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).text())) {
        if ( m_wrongAnswer ) {
            resultWrong();
        }
        resultCorrect();
    } else {
        m_wrongAnswer = true;
    }
}

void ImagePracticeDlg::setProgressCounter(int current, int total)
{
    countbar->setMaximum(total);
    countbar->setValue(current);
}

QProgressBar * ImagePracticeDlg::timebar()
{
    return timeProgressBar;
}

#include "imagepracticedlg.moc"


