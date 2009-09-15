/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "writtenpracticewidget.h"
#include "ui_practice_widget_written.h"
#include <kdebug.h>

using namespace Practice;

WrittenPracticeWidget::WrittenPracticeWidget(QWidget *parent)
    : AbstractModeWidget(parent)
{
    m_ui = new Ui::WrittenPracticeWidget();
    m_ui->setupUi(this);
    kDebug() << "Created WrittenPracticeWidget";
    connect(m_ui->answerEdit, SIGNAL(editingFinished()), this, SLOT(continueClicked()));
}

void WrittenPracticeWidget::continueClicked()
{
    kDebug() << "cont";
    emit continueAction();
}

QVariant WrittenPracticeWidget::userInput()
{
    return QVariant(m_ui->answerEdit->text());
}

void WrittenPracticeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
}

void WrittenPracticeWidget::showQuestion()
{
    kDebug() << "update display";
    m_ui->answerEdit->setEnabled(true);
    m_ui->answerEdit->clear();
    m_ui->answerEdit->setFocus();
    m_ui->solutionLabel->setVisible(false);
}

void WrittenPracticeWidget::setSolution(const QVariant& question)
{
    m_ui->solutionLabel->setText(question.toString());
}

void WrittenPracticeWidget::showSolution()
{
    m_ui->answerEdit->setEnabled(false);
    m_ui->solutionLabel->setVisible(true);
}

void WrittenPracticeWidget::setHint(const QVariant& hint)
{
    m_ui->helpLabel->setText(hint.toString());
}

void WrittenPracticeWidget::setFeedback(const QVariant& feedback)
{
    m_ui->helpLabel->setText(feedback.toString());
}


#include "writtenpracticewidget.moc"
