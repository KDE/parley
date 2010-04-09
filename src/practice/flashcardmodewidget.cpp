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

#include "guifrontend.h"

using namespace Practice;


FlashCardModeWidget::FlashCardModeWidget (GuiFrontend *frontend, QWidget* parent )
    : AbstractModeWidget (frontend, parent)
{
    m_ui = new Ui::FlashCardPracticeWidget();
    m_ui->setupUi(this);
}

void FlashCardModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
}

void FlashCardModeWidget::showQuestion()
{
    m_ui->solutionLabel->setHidden(true);
    m_frontend->showSetResultButtons(false);
}

void FlashCardModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toString();
}

void FlashCardModeWidget::showSolution()
{
    m_ui->solutionLabel->setHidden(false);
    m_ui->solutionLabel->setText(m_solution);
    m_frontend->showSetResultButtons(true);
}

void FlashCardModeWidget::setHint(const QVariant& hint)
{
    m_ui->solutionLabel->setHidden(false);
    m_ui->solutionLabel->setText(hint.toString());
}

QVariant FlashCardModeWidget::userInput()
{
    return QVariant();
}

void FlashCardModeWidget::setQuestionSound(const KUrl& soundUrl)
{

}

void FlashCardModeWidget::setSolutionSound(const KUrl& soundUrl)
{

}

void FlashCardModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{

}

void FlashCardModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{

}

#include "flashcardmodewidget.moc"
