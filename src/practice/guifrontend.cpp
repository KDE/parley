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
#include "multiplechoicemodewidget.h"
#include "flashcardmodewidget.h"

using namespace Practice;

GuiFrontend::GuiFrontend(QWidget* parent)
    : AbstractFrontend(parent), m_modeWidget(0), m_lastImage("invalid")
{
    m_widget = new QWidget();
    
    m_ui = new Ui::PracticeMainWindow();
    m_ui->setupUi(m_widget);
    m_ui->centralPracticeWidget->setLayout(new QHBoxLayout(parent));
    
    connect(m_ui->continueButton, SIGNAL(clicked()), this, SIGNAL(signalContinueButton()));
    connect(m_ui->answerLaterButton, SIGNAL(clicked()), this, SIGNAL(skipAction()));
    connect(m_ui->hintButton, SIGNAL(clicked()), this, SIGNAL(hintAction()));
    connect(m_ui->correctButton, SIGNAL(toggled(bool)), this, SLOT(resultRadioButtonsChanged()));

    kDebug() << "Created GuiFrontend";
}

GuiFrontend::~GuiFrontend()
{
        // FIXME delete m_widget;
}

QVariant GuiFrontend::userInput()
{
    return m_modeWidget->userInput();
}

QWidget* GuiFrontend::widget()
{
    return m_widget;
}

void GuiFrontend::setMode(Mode mode)
{
    kDebug() << "setCentralWidget!" << mode;
    AbstractModeWidget *newWidget = 0;
    switch(mode) {
        case Written:
            if (/*m_modeWidget->metaObject()->className() == QLatin1String("WrittenPracticeWidget")*/false) {
                kDebug() << "Written practice widget is already the central widget";
                break;
            }
            newWidget = new WrittenPracticeWidget(m_widget);
            break;
        case MultipleChoice:
            newWidget = new MultiplechoiceModeWidget(m_widget);
            break;
        case FlashCard:
            newWidget = new FlashCardModeWidget(m_widget);
            break;
        case MixedLetters:
            break;
    }
    if (newWidget) {
        m_ui->centralPracticeWidget->layout()->addWidget(newWidget);
        delete m_modeWidget;
        m_modeWidget = newWidget;
        connect(m_modeWidget, SIGNAL(continueAction()), this, SLOT(continueAction()));
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
    m_ui->answerLaterButton->setEnabled(true);
    m_ui->hintButton->setEnabled(true);
    m_ui->continueButton->setFocus();
    m_modeWidget->showQuestion();
    kDebug() << "active window: " << m_ui->continueButton->isActiveWindow();
}

void GuiFrontend::showSolution()
{
    m_ui->ratingStack->setCurrentIndex(1);
    if (m_ui->wrongButton->isChecked()) {
        m_ui->wrongButton->setFocus();
    } else {
        m_ui->correctButton->setFocus();
    }
    m_modeWidget->showSolution();
    m_ui->answerLaterButton->setEnabled(false);
    m_ui->hintButton->setEnabled(false);
}

void GuiFrontend::setFinishedWordsTotalWords(int finished, int total)
{
    // update progress bar
    m_ui->totalProgress->setMaximum(total);
    m_ui->totalProgress->setValue(finished);
    m_ui->totalProgress->setToolTip(i18n("You answered %1 of a total of %2 words.\nYou are %3% done.", finished, total, finished/total*100));
}

void GuiFrontend::setHint(const QVariant& hint)
{
    m_modeWidget->setHint(hint);
}

void GuiFrontend::setQuestion(const QVariant& question)
{
    m_modeWidget->setQuestion(question);
}

void GuiFrontend::setQuestionImage(const KUrl& image)
{
    if (m_lastImage == image) {
        m_lastImage = image;
        return;
    }
    if (image.path().isEmpty()) {
        m_ui->imageWidget->setPixmap(KIcon("parley").pixmap(128));
    } else {
        QPixmap pixmap(image.path());
        m_ui->imageWidget->setPixmap(pixmap);
    }
    m_lastImage = image;
}

void GuiFrontend::setQuestionPronunciation(const QString& pronunciationText)
{
    m_modeWidget->setQuestionPronunciation(pronunciationText);
}

void GuiFrontend::setQuestionSound(const KUrl& soundUrl)
{
    m_modeWidget->setQuestionSound(soundUrl);
}

void GuiFrontend::setSolution(const QVariant& solution)
{
    m_modeWidget->setSolution(solution);
}

void GuiFrontend::setSolutionImage(const KUrl& img)
{
    // TODO
}

void GuiFrontend::setSolutionPronunciation(const QString& pronunciationText)
{
    m_modeWidget->setSolutionPronunciation(pronunciationText);
}

void GuiFrontend::setSolutionSound(const KUrl& soundUrl)
{
    m_modeWidget->setSolutionSound(soundUrl);
}

void GuiFrontend::setFeedback(const QVariant& feedback)
{
    m_modeWidget->setFeedback(feedback);
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
    m_modeWidget->setResultState(resultState);
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
