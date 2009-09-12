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

using namespace Practice;


FlashCardModeWidget::FlashCardModeWidget ( AbstractBackend* backend, QWidget* parent )
    : AbstractModeWidget ( backend, parent )
{
    m_ui = new Ui::FlashCardPracticeWidget();
    m_ui->setupUi(this);
    connect(m_ui->continueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
}


void FlashCardModeWidget::updateDisplay()
{
    m_ui->questionLabel->setText(m_backend->question().toString());
    m_ui->solutionLabel->setText(m_backend->solution().toString());
}

QVariant FlashCardModeWidget::userInput()
{
    return QVariant();
}

void Practice::FlashCardModeWidget::continueClicked()
{
    emit continueAction();
}

#include "flashcardmodewidget.moc"
