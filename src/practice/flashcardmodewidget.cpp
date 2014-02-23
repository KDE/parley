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
#include "latexrenderer.h"

using namespace Practice;


FlashCardModeWidget::FlashCardModeWidget(GuiFrontend *frontend, QWidget* parent)
    : AbstractModeWidget(frontend, parent), m_questionLatexRenderer(0), m_solutionLatexRenderer(0)
{
    m_ui = new Ui::FlashCardPracticeWidget();
    m_ui->setupUi(this);
    connect(frontend, SIGNAL(continueAction()), this, SIGNAL(stopAudio()));
    connect(frontend, SIGNAL(skipAction()), this, SIGNAL(stopAudio()));
}

void FlashCardModeWidget::setQuestionFont(const QFont& font)
{
    m_ui->questionLabel->setFont(font);
}

void FlashCardModeWidget::setSolutionFont(const QFont& font)
{
    m_ui->solutionLabel->setFont(font);
}

void FlashCardModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setMinimumSize(QSize(0, 0));
    if (LatexRenderer::isLatex(question.toString())) {
        if (!m_questionLatexRenderer) {
            m_questionLatexRenderer = new LatexRenderer(this);
            m_questionLatexRenderer->setResultLabel(m_ui->questionLabel);
        }
        m_questionLatexRenderer->renderLatex(question.toString());
    } else {
        m_ui->questionLabel->setText(question.toString());
    }
}

void FlashCardModeWidget::showQuestion()
{
    m_ui->solutionLabel->setHidden(true);
    m_frontend->showSetResultButtons(false);

    m_ui->questionPronunciationLabel->setVisible(m_ui->questionPronunciationLabel->isEnabled());
    m_ui->questionSoundButton->setVisible(true); // TODO: Use Configuration's Sound Enable QCheckbox
    m_ui->solutionPronunciationLabel->setVisible(false);
    m_ui->solutionSoundButton->setVisible(false);
}

void FlashCardModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toString();
}

void FlashCardModeWidget::setSynonym(const QString& entry)
{
    //TODO Do something here to show synonyms
}

void FlashCardModeWidget::showSynonym()
{
    //TODO Do something here to show synonyms
}

void FlashCardModeWidget::showSolution()
{
    m_ui->solutionLabel->setHidden(true);
    m_ui->solutionLabel->setPalette(m_correctPalette);

    m_ui->solutionLabel->setMinimumSize(QSize(0, 0));
    if (LatexRenderer::isLatex(m_solution)) {
        if (!m_solutionLatexRenderer) {
            m_solutionLatexRenderer = new LatexRenderer(this);
            m_solutionLatexRenderer->setResultLabel(m_ui->solutionLabel);
        }
        m_solutionLatexRenderer->renderLatex(m_solution);
    } else {
        m_ui->solutionLabel->setText(m_solution);
    }

    m_ui->solutionLabel->setHidden(false);
    m_frontend->showSetResultButtons(true);

    m_ui->solutionPronunciationLabel->setVisible(m_ui->solutionPronunciationLabel->isEnabled());
    m_ui->solutionSoundButton->setVisible(true); // TODO: Use Configuration's Sound Enable QCheckbox
}

void FlashCardModeWidget::setHint(const QVariant& hint)
{
    m_ui->solutionLabel->setHidden(true);
    m_ui->solutionLabel->setText(hint.toString());
    m_ui->solutionLabel->setHidden(false);
}

QVariant FlashCardModeWidget::userInput()
{
    return QVariant();
}

void FlashCardModeWidget::setQuestionSound(const KUrl& soundUrl)
{
    m_ui->questionSoundButton->setSoundFile(soundUrl);
}

void FlashCardModeWidget::setSolutionSound(const KUrl& soundUrl)
{
    m_ui->solutionSoundButton->setSoundFile(soundUrl);
}

void FlashCardModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{
    m_ui->solutionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->solutionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

void FlashCardModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{
    m_ui->questionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->questionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

#include "flashcardmodewidget.moc"
