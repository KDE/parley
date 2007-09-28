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

    m_scene = new QGraphicsScene();
    m_scene->setSceneRect( 0.0, 0.0, 1.0, 1.0 );
    imageGraphicsView->setScene(m_scene);

    connect(verifySolutionButton, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(showSolutionButton, SIGNAL(clicked()), SLOT(showSolution()));
    connect(answerLineEdit, SIGNAL(textChanged()), SLOT(slotAnswerChanged()));

    countbar->setFormat("%v/%m");
    timeProgressBar->setFormat("%v");

    KConfigGroup cg(KGlobal::config(), "ImagePracticeDlg");
    restoreDialogSize(cg);
}


ImagePracticeDlg::~ImagePracticeDlg()
{
    m_scene->deleteLater();
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


    foreach ( QGraphicsItem* item, m_scene->items() ) {
        m_scene->removeItem(item);
        delete item;
    }

    QString url;
    url = m_entry->exp->translation(identifier).imageUrl().toLocalFile();
    if ( url.isEmpty() ) {
        url = m_entry->exp->translation(Prefs::fromIdentifier()).imageUrl().toLocalFile();
    }
    if ( !url.isEmpty() ) {
        kDebug() << "image found:" << url;
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap(url));

        qreal scale = qMin(imageGraphicsView->width()/pixmapItem->boundingRect().width(), imageGraphicsView->height()/pixmapItem->boundingRect().height());

        pixmapItem->scale( scale, scale );

        pixmapItem->translate( -pixmapItem->boundingRect().width()/2.0, -pixmapItem->boundingRect().height()/2.0 );

        m_scene->addItem(pixmapItem);
    } else {
        m_scene->addText( m_entry->exp->translation(identifier).text() );
    }
}


void ImagePracticeDlg::slotAnswerChanged()
{
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


