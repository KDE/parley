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

#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <KLocale>

#include <kvttablemodel.h>
#include <keduvocdocument.h>

ImagePracticeDlg::ImagePracticeDlg(KEduVocDocument *doc, QWidget *parent)
    :PracticeDialog(i18n("Image Practice"), doc, parent), Ui::ImagePracticeDlg()
{
    setupUi(mainWidget());

    m_scene = new QGraphicsScene();
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
    PracticeDialog::setEntry(entry);

    timeProgressBar->setEnabled(Prefs::showCounter());
    timelabel->setEnabled(Prefs::showCounter());
    questionLineEdit->setFont(Prefs::tableFont());
    answerLineEdit->setFont(Prefs::tableFont());
    answerLineEdit->setText("");

    int identifier = Prefs::toIdentifier();

    questionLineEdit->setText( m_entry->exp->translation(identifier).text() );

    verifySolutionButton->setDefault(true);

    progCount->setText( QString::number(entry->statisticCount()) );

    resetQueryWidget(answerLineEdit);
    answerLineEdit->setFocus();

    if ( !m_entry->exp->translation(identifier).imageUrl().isEmpty() ) {
kDebug() << "image found:" << m_entry->exp->translation(identifier).imageUrl().url();
//         m_scene->addPixmap(QPixmap(m_entry->exp->translation(identifier).imageUrl().url()));
        QString url = m_entry->exp->translation(identifier).imageUrl().toLocalFile();
        m_scene->addPixmap(QPixmap(url));
kDebug() << "pixmap is null: " << QPixmap(url).isNull();
    } else {
        m_scene->addText("Hello, world!");
    }
    m_scene->invalidate();
}


void ImagePracticeDlg::slotAnswerChanged()
{
    verifySolutionButton->setDefault(true);
    resetQueryWidget(answerLineEdit);
}


void ImagePracticeDlg::showSolution()
{
    answerLineEdit->setText( m_entry->exp->translation(Prefs::toIdentifier()).text() );
    verifyField(answerLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).text());
}


void ImagePracticeDlg::verifyClicked()
{
    if (verifyField(answerLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).text())) {
        resultCorrect();
    } else {
        ///@todo better do something sensible here...
        resultWrong();
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


