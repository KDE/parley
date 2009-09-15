/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "guifrontend.h"
#include "ui_practice_mainwindow.h"

#include <kdebug.h>
#include "writtenpracticewidget.h"
#include "flashcardmodewidget.h"

using namespace Practice;

GuiFrontend::GuiFrontend(QObject* parent)
    : AbstractFrontend(parent), m_centralWidget(0)
{
    m_mainWindow = new PracticeMainWindow();
    QWidget* centralWidget = new QWidget(m_mainWindow);
    m_mainWindow->setCentralWidget(centralWidget);
    m_ui = new Ui::PracticeMainWindow();
    m_ui->setupUi(centralWidget);
    m_ui->centralPracticeWidget->setLayout(new QHBoxLayout(m_mainWindow));

    connect(m_mainWindow, SIGNAL(enterPressed()), this, SIGNAL(signalContinueButton()));
    
    connect(m_ui->continueButton, SIGNAL(clicked()), this, SIGNAL(signalContinueButton()));
    connect(m_ui->answerLaterButton, SIGNAL(clicked()), this, SIGNAL(skipAction()));
    connect(m_ui->hintButton, SIGNAL(clicked()), this, SIGNAL(hintAction()));
    connect(m_ui->correctButton, SIGNAL(toggled(bool)), this, SLOT(resultRadioButtonsChanged()));

    kDebug() << "Created GuiFrontend";
}

QVariant GuiFrontend::userInput()
{
    return m_centralWidget->userInput();
}

KXmlGuiWindow* GuiFrontend::getWindow()
{
    return m_mainWindow;
}

void GuiFrontend::setMode(Mode mode)
{
    kDebug() << "setCentralWidget!" << mode;
    AbstractModeWidget *newWidget = 0;
    switch(mode) {
        case Written:
            if (/*m_centralWidget->metaObject()->className() == QLatin1String("WrittenPracticeWidget")*/false) {
                kDebug() << "Written practice widget is already the central widget";
                break;
            }
            newWidget = new WrittenPracticeWidget(m_mainWindow);
            break;
        case MultipleChoice:
            break;
        case FlashCard:
            newWidget = new FlashCardModeWidget(m_mainWindow);
            break;
        case MixedLetters:
            break;
    }
    if (newWidget) {
        m_ui->centralPracticeWidget->layout()->addWidget(newWidget);
        delete m_centralWidget;
        m_centralWidget = newWidget;
        connect(m_centralWidget, SIGNAL(continueAction()), this, SLOT(continueAction()));
        kDebug() << "set up frontend";
    }
}

void GuiFrontend::setLessonName(const QString& lessonName)
{
    m_ui->lessonLabel->setText(i18nc("Display of the current lesson during practice", "Lesson: %1", lessonName));
}

void GuiFrontend::showQuestion()
{
    m_ui->ratingStack->setCurrentIndex(0);
    m_ui->continueButton->setFocus();
    m_centralWidget->showQuestion();
}

void GuiFrontend::showSolution()
{
    m_ui->ratingStack->setCurrentIndex(1);
    if (m_ui->wrongButton->isChecked()) {
        m_ui->wrongButton->setFocus();
    } else {
        m_ui->correctButton->setFocus();
    }
    m_centralWidget->showSolution();
}

void GuiFrontend::setFinishedWordsTotalWords(int finished, int total)
{
    // update progress bar
    m_ui->totalProgress->setMaximum(total);
    m_ui->totalProgress->setValue(finished);
    m_ui->totalProgress->setToolTip(i18n("You answered %1 of a total of %2 words.\nYou are %3% done.", finished, total, finished/total*100));
}

void GuiFrontend::setQuestionImage(const KUrl& image)
{
    QPixmap pixmap(image.path());
    m_ui->imageWidget->setPixmap(pixmap);
}

void GuiFrontend::setHint(const QVariant& hint)
{
    m_centralWidget->setHint(hint);
}

void GuiFrontend::setQuestion(const QVariant& question)
{
    m_centralWidget->setQuestion(question);
}

void GuiFrontend::setSolution(const QVariant& solution)
{
    m_centralWidget->setSolution(solution);
}

void GuiFrontend::setSolutionImage(const QPixmap& img)
{
    // TODO
}

void GuiFrontend::setFeedback(const QVariant& feedback)
{
    m_centralWidget->setFeedback(feedback);
}

void GuiFrontend::setResultState(ResultState resultState)
{
    // TODO: temporary text labels instead of graphics
    m_ui->statusImageLabel->setFont(QFont("", 80, QFont::Bold));
    switch (resultState) {
    case AbstractFrontend::QuestionState:
        m_ui->statusImageLabel->setText("?");
        break;
    case AbstractFrontend::AnswerCorrect:
        m_ui->statusImageLabel->setText(QChar(0x2713));
        if(!m_ui->correctButton->isChecked()) {
            m_ui->correctButton->setChecked(true);
        }
        break;
    case AbstractFrontend::AnswerWrong:
        m_ui->statusImageLabel->setText(QChar(0x2717));
        if(!m_ui->wrongButton->isChecked()) {
            m_ui->wrongButton->setChecked(true);
        }
        break;
    }

    m_resultState = resultState;
}

AbstractFrontend::ResultState GuiFrontend::resultState()
{
    return m_resultState;
}

void GuiFrontend::resultRadioButtonsChanged()
{
    if(m_ui->correctButton->isChecked()) {
        setResultState(AnswerCorrect);
    } else {
        setResultState(AnswerWrong);
    }
}


void GuiFrontend::continueAction()
{
    // animateClick emits the pushed signal
    m_ui->continueButton->animateClick();
}


#include "guifrontend.moc"
