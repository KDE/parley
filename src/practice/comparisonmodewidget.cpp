/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "comparisonmodewidget.h"

#include "ui_practice_widget_comparison.h"

#include <QDebug>
#include <KColorScheme>
#include <QRadioButton>
#include <QTimer>
#include <KLocalizedString>

using namespace Practice;

ComparisonModeWidget::ComparisonModeWidget(GuiFrontend *frontend, QWidget* parent)
    : AbstractModeWidget(frontend, parent)
{
    m_ui = new Ui::ComparisonPracticeWidget();
    m_ui->setupUi(this);

    connect(m_ui->absoluteEdit, &QLineEdit::returnPressed, this, &ComparisonModeWidget::nextComparisonForm);
    connect(m_ui->comparativeEdit, &QLineEdit::returnPressed, this, &ComparisonModeWidget::nextComparisonForm);
    connect(m_ui->superlativeEdit, &QLineEdit::returnPressed, this, &ComparisonModeWidget::nextComparisonForm);

    m_ui->absoluteSolutionLabel->setPalette(m_correctPalette);
    m_ui->comparativeSolutionLabel->setPalette(m_correctPalette);
    m_ui->superlativeSolutionLabel->setPalette(m_correctPalette);
}

void ComparisonModeWidget::setQuestionFont(const QFont& font)
{
    m_ui->questionLabel->setFont(font);
}

void ComparisonModeWidget::setSolutionFont(const QFont& font)
{
    m_ui->absoluteSolutionLabel->setFont(font);
    m_ui->comparativeSolutionLabel->setFont(font);
    m_ui->superlativeSolutionLabel->setFont(font);
    m_ui->absoluteEdit->setFont(font);
    m_ui->comparativeEdit->setFont(font);
    m_ui->superlativeEdit->setFont(font);
}

void ComparisonModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
    m_ui->absoluteEdit->clear();
    m_ui->comparativeEdit->clear();
    m_ui->superlativeEdit->clear();
    m_ui->absoluteEdit->setPalette(QPalette());
    m_ui->comparativeEdit->setPalette(QPalette());
    m_ui->superlativeEdit->setPalette(QPalette());
    m_ui->absoluteSolutionLabel->clear();
    m_ui->comparativeSolutionLabel->clear();
    m_ui->superlativeSolutionLabel->clear();
}

void ComparisonModeWidget::showQuestion()
{
    QTimer::singleShot(0, m_ui->absoluteEdit, SLOT(setFocus()));
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
    if (m_ui->absoluteEdit->text() == m_solution.at(0)) {
        m_ui->absoluteEdit->setPalette(m_correctPalette);
    } else {
        m_ui->absoluteEdit->setPalette(m_wrongPalette);
        m_ui->absoluteSolutionLabel->setText(m_solution.at(0));
    }
    if (m_ui->comparativeEdit->text() == m_solution.at(1)) {
        m_ui->comparativeEdit->setPalette(m_correctPalette);
    } else {
        m_ui->comparativeEdit->setPalette(m_wrongPalette);
        m_ui->comparativeSolutionLabel->setText(m_solution.at(1));
    }

    if (m_ui->superlativeEdit->text() == m_solution.at(2)) {
        m_ui->superlativeEdit->setPalette(m_correctPalette);
    } else {
        m_ui->superlativeEdit->setPalette(m_wrongPalette);
        m_ui->superlativeSolutionLabel->setText(m_solution.at(2));
    }
}

QVariant ComparisonModeWidget::userInput()
{
    QStringList answers;
    answers.append(m_ui->absoluteEdit->text());
    answers.append(m_ui->comparativeEdit->text());
    answers.append(m_ui->superlativeEdit->text());
    return answers;
}

void ComparisonModeWidget::setHint(const QVariant& /*hint*/)
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

void ComparisonModeWidget::setQuestionPronunciation(const QString& /*pronunciationText*/)
{
}

void ComparisonModeWidget::setSolutionPronunciation(const QString& /*pronunciationText*/)
{
}

void ComparisonModeWidget::setQuestionSound(const QUrl& /*soundUrl*/)
{
}

void ComparisonModeWidget::setSolutionSound(const QUrl& /*soundUrl*/)
{
}
