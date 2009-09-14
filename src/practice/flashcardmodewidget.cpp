/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "flashcardmodewidget.h"

#include "ui_practice_widget_flashcard.h"

#include <KDebug>

using namespace Practice;


FlashCardModeWidget::FlashCardModeWidget (QWidget* parent )
    : AbstractModeWidget (parent)
{
    m_ui = new Ui::FlashCardPracticeWidget();
    m_ui->setupUi(this);
    connect(m_ui->continueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
}

void FlashCardModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
}

void FlashCardModeWidget::showQuestion()
{
    m_ui->solutionLabel->setHidden(true);
    m_ui->ratingStack->setCurrentIndex(0);
}

void FlashCardModeWidget::setSolution(const QVariant& question)
{
    m_ui->solutionLabel->setText(question.toString());
}

void FlashCardModeWidget::showSolution()
{
    m_ui->solutionLabel->setHidden(false);
    m_ui->ratingStack->setCurrentIndex(1);
}


QVariant FlashCardModeWidget::userInput()
{
    return QVariant(m_ui->easyButton->isChecked());
}

void Practice::FlashCardModeWidget::continueClicked()
{
    kDebug() << "cont";
    emit continueAction();
}

#include "flashcardmodewidget.moc"
