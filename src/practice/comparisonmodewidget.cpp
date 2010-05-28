/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "comparisonmodewidget.h"

#include "ui_practice_widget_comparison.h"

#include <KDebug>
#include <kcolorscheme.h>
#include <QtGui/QRadioButton>

using namespace Practice;

ComparisonModeWidget::ComparisonModeWidget (GuiFrontend *frontend, QWidget* parent )
    : AbstractModeWidget (frontend, parent)
{
    m_ui = new Ui::ComparisonPracticeWidget();
    m_ui->setupUi(this);

    connect(m_ui->absoluteEdit, SIGNAL(returnPressed(QString)), this, SLOT(nextComparisonForm()));
    connect(m_ui->comparativeEdit, SIGNAL(returnPressed(QString)), this, SLOT(nextComparisonForm()));
    connect(m_ui->superlativeEdit, SIGNAL(returnPressed(QString)), this, SLOT(nextComparisonForm()));
}

ComparisonModeWidget::~ComparisonModeWidget()
{
}

void ComparisonModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
    m_ui->absoluteEdit->clear();
    m_ui->comparativeEdit->clear();
    m_ui->superlativeEdit->clear();
}

void ComparisonModeWidget::showQuestion()
{
    m_ui->absoluteEdit->setFocus();
    m_ui->feedbackLabel->setText(i18n("Enter the comparison forms."));
}

void ComparisonModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toStringList();
    Q_ASSERT(m_solution.size() == 3);
    m_ui->absoluteEdit->setDisabled(m_solution.at(0).isEmpty());
    m_ui->comparativeEdit->setDisabled(m_solution.at(1).isEmpty());
    m_ui->superlativeEdit->setDisabled(m_solution.at(2).isEmpty());
}

void ComparisonModeWidget::setFeedback(const QVariant& feedback)
{
    m_ui->feedbackLabel->setText(feedback.toString());
}

void ComparisonModeWidget::showSolution()
{
}

QVariant ComparisonModeWidget::userInput()
{
    QStringList answers;
    answers.append(m_ui->absoluteEdit->text());
    answers.append(m_ui->comparativeEdit->text());
    answers.append(m_ui->superlativeEdit->text());
    return answers;
}

void ComparisonModeWidget::setHint(const QVariant& hint)
{

}

void ComparisonModeWidget::nextComparisonForm()
{
    if (m_ui->absoluteEdit->hasFocus()) {
        m_ui->comparativeEdit->setFocus();
    } else if (m_ui->comparativeEdit->hasFocus()) {
        m_ui->superlativeEdit->setFocus();
    } else  {
        emit continueAction();
    }
}

void ComparisonModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{

}
void ComparisonModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{

}
void ComparisonModeWidget::setQuestionSound(const KUrl& soundUrl)
{

}
void ComparisonModeWidget::setSolutionSound(const KUrl& soundUrl)
{

}


#include "comparisonmodewidget.moc"
