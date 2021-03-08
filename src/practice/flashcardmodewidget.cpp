/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "flashcardmodewidget.h"

#include "ui_practice_widget_flashcard.h"

#include <QUrl>

#include "guifrontend.h"
#include "latexrenderer.h"

using namespace Practice;


FlashCardModeWidget::FlashCardModeWidget(GuiFrontend *frontend, QWidget* parent)
    : AbstractModeWidget(frontend, parent), m_questionLatexRenderer(0), m_solutionLatexRenderer(0)
{
    m_ui = new Ui::FlashCardPracticeWidget();
    m_ui->setupUi(this);
    connect(frontend, &AbstractFrontend::continueAction, this, &AbstractModeWidget::stopAudio);
    connect(frontend, &AbstractFrontend::skipAction, this, &AbstractModeWidget::stopAudio);
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
        return;
    }

    if (question.canConvert<QUrl>()) {
        // A QUrl might be an image.
        QPixmap pixmap(question.value<QUrl>().path());
        if (!pixmap.isNull()) {
            m_ui->questionLabel->setText(QString() );
            m_ui->questionLabel->setPixmap(
                pixmap.scaled(m_ui->questionLabel->size(), Qt::KeepAspectRatio ));
            return;
        }
    }

    // A normal text.
    m_ui->questionLabel->setText(question.toString());
}

void FlashCardModeWidget::showQuestion()
{
    m_ui->solutionLabel->setHidden(true);
    m_frontend->showSetResultButtons(false);

    m_ui->questionPronunciationLabel->setVisible(m_ui->questionPronunciationLabel->isEnabled());
    m_ui->questionSoundButton->setVisible(m_ui->questionSoundButton->isEnabled());
    m_ui->solutionPronunciationLabel->setVisible(false);
    m_ui->solutionSoundButton->setVisible(false);
}

void FlashCardModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toString();
}

void FlashCardModeWidget::setSynonym(const QString& /*entry*/)
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
    m_ui->solutionSoundButton->setVisible(m_ui->solutionSoundButton->isEnabled());
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

void FlashCardModeWidget::setQuestionSound(const QUrl& soundUrl)
{
    m_ui->questionSoundButton->setSoundFile(soundUrl);
}

void FlashCardModeWidget::setSolutionSound(const QUrl& soundUrl)
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
