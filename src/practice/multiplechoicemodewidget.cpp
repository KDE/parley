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

#include "multiplechoicemodewidget.h"

#include "ui_practice_widget_multiplechoice.h"

#include <KDebug>

using namespace Practice;


MultiplechoiceModeWidget::MultiplechoiceModeWidget (QWidget* parent )
    : AbstractModeWidget (parent)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);
}

void MultiplechoiceModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
}

void MultiplechoiceModeWidget::showQuestion()
{
    m_ui->solutionLabel->setHidden(true);
}

void MultiplechoiceModeWidget::setSolution(const QVariant& solution)
{
    m_ui->solutionLabel->setText(solution.toString());
}

void MultiplechoiceModeWidget::showSolution()
{
    m_ui->solutionLabel->setHidden(false);
}

QVariant MultiplechoiceModeWidget::userInput()
{
    return QVariant();
}

void MultiplechoiceModeWidget::setQuestionSound(const KUrl& soundUrl)
{

}

void MultiplechoiceModeWidget::setSolutionSound(const KUrl& soundUrl)
{

}

void MultiplechoiceModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{

}

void MultiplechoiceModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{

}

#include "multiplechoicemodewidget.moc"
