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

#include <kcolorscheme.h>

using namespace Practice;

WrittenPracticeWidget::WrittenPracticeWidget(GuiFrontend *frontend, QWidget *parent)
    : AbstractModeWidget(frontend, parent)
{
    m_ui = new Ui::WrittenPracticeWidget();
    m_ui->setupUi(this);
    m_ui->mixedSolutionLabel->setVisible(false);
    connect(m_ui->answerEdit, SIGNAL(returnPressed()), this, SLOT(continueClicked()));

    KColorScheme scheme(QPalette::Active);
    m_correctPalette = QApplication::palette();
    m_correctPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::PositiveText).color());
    m_correctPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::PositiveText).color());
    m_wrongPalette = QApplication::palette();
    m_wrongPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::NegativeText).color());
    m_wrongPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::NegativeText).color());
}

void WrittenPracticeWidget::continueClicked()
{
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
    m_ui->answerEdit->setEnabled(true);
    m_ui->answerEdit->clear();
    m_ui->answerEdit->setFocus();
    m_ui->answerEdit->setPalette(QApplication::palette());
    m_ui->solutionLabel->setText(QString());
    m_ui->helpLabel->clear();

    m_ui->questionPronunciationLabel->setVisible(m_ui->questionPronunciationLabel->isEnabled());
    m_ui->questionSoundButton->setVisible(m_ui->questionSoundButton->isEnabled());
    m_ui->solutionPronunciationLabel->setVisible(false);
    m_ui->solutionSoundButton->setVisible(false);
}

void WrittenPracticeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toString();
}

void WrittenPracticeWidget::showSolution()
{
    m_ui->solutionLabel->setText(m_solution);
    m_ui->answerEdit->setEnabled(false);
    if (m_resultState == AbstractFrontend::AnswerCorrect) {
        m_ui->answerEdit->setPalette(m_correctPalette);
    } else {
        m_ui->answerEdit->setPalette(m_wrongPalette);
    }
    m_ui->solutionLabel->setPalette(m_correctPalette);

    m_ui->solutionPronunciationLabel->setVisible(m_ui->solutionPronunciationLabel->isEnabled());
    m_ui->solutionSoundButton->setVisible(m_ui->solutionSoundButton->isEnabled());
}

void WrittenPracticeWidget::setHint(const QVariant& hint)
{
    m_ui->helpLabel->setText(hint.toString());
}

void WrittenPracticeWidget::setFeedback(const QVariant& feedback)
{
    m_ui->helpLabel->setText(feedback.toString());
}

void WrittenPracticeWidget::setResultState(AbstractFrontend::ResultState resultState)
{
    m_resultState = resultState;
}

void WrittenPracticeWidget::setQuestionSound(const KUrl& soundUrl)
{
    m_ui->questionSoundButton->setSoundFile(soundUrl);
}

void WrittenPracticeWidget::setSolutionSound(const KUrl& soundUrl)
{
    m_ui->solutionSoundButton->setSoundFile(soundUrl);
}

void WrittenPracticeWidget::setSolutionPronunciation(const QString& pronunciationText)
{
    m_ui->solutionPronunciationLabel->setText('['+pronunciationText+']');
    m_ui->solutionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

void WrittenPracticeWidget::setQuestionPronunciation(const QString& pronunciationText)
{
    m_ui->questionPronunciationLabel->setText('['+pronunciationText+']');
    m_ui->questionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

#include "writtenpracticewidget.moc"
