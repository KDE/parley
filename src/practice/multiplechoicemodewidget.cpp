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
#include "multiplechoicedata.h"
#include "latexrenderer.h"
#include "guifrontend.h"

#include "ui_practice_widget_multiplechoice.h"

#include <QDebug>
#include <kcolorscheme.h>
#include <QtWidgets/QRadioButton>
#include <QTimer>
#include <QKeyEvent>
#include <QVBoxLayout>

using namespace Practice;


MultiplechoiceModeWidget::MultiplechoiceModeWidget(GuiFrontend *frontend, QWidget* parent)
    : AbstractModeWidget(frontend, parent), m_latexRenderer(0)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        QAction *action = new QAction(this);
        action->setShortcut(Qt::Key_1 + i);
        addAction(action);
        m_actions.append(action);
    }
    connect(frontend, SIGNAL(continueAction()), this, SIGNAL(stopAudio()));
    connect(frontend, SIGNAL(skipAction()), this, SIGNAL(stopAudio()));

}

void MultiplechoiceModeWidget::setQuestionFont(const QFont& font)
{
    m_ui->questionLabel->setFont(font);
}

void MultiplechoiceModeWidget::setSolutionFont(const QFont& font)
{
    m_solutionFont = font;
}

void MultiplechoiceModeWidget::setQuestion(const QVariant& question)
{
    if (!question.canConvert<MultipleChoiceData>()) {
        qWarning() << "expected MultipleChoiceData";
        return;
    }
    MultipleChoiceData data = question.value<MultipleChoiceData>();

    m_ui->questionLabel->setMinimumSize(QSize(0, 0));
    if (LatexRenderer::isLatex(data.question)) {
        if (!m_latexRenderer) {
            m_latexRenderer = new LatexRenderer(this);
            m_latexRenderer->setResultLabel(m_ui->questionLabel);
        }
        m_latexRenderer->renderLatex(data.question);
    } else {
        m_ui->questionLabel->setText(data.question);
    }

    if (m_choiceButtons.size() != data.choices.size()) {
        qDeleteAll(m_choiceButtons);
        m_choiceButtons.clear();
        setNumberOfRadioButtons(data.choices.size());
    }

    int j = 0;
    foreach(QRadioButton * radio, m_choiceButtons) {
        radio->setText(data.choices[j]);
        radio->setToolTip(data.choices[j]);
        radio->setFont(m_solutionFont);
        j++;
    }
}

void MultiplechoiceModeWidget::showQuestion()
{
    m_ui->questionPronunciationLabel->setVisible(m_ui->questionPronunciationLabel->isEnabled());
    m_ui->questionSoundButton->setVisible(m_ui->questionSoundButton->isEnabled());
    m_ui->solutionPronunciationLabel->setVisible(false);
    m_ui->solutionSoundButton->setVisible(false);
    m_ui->feedbackLabel->clear();

    if ( ! m_choiceButtons.isEmpty() ) {
        //necessary trick to uncheck'em all
        m_choiceButtons[0]->setChecked(true);
        m_choiceButtons[0]->setAutoExclusive(false);
        m_choiceButtons[0]->setChecked(false);
        m_choiceButtons[0]->setAutoExclusive(true);
        foreach(QRadioButton * radio, m_choiceButtons) {
            radio->setPalette(palette());
            radio->setEnabled(true);
        }

        QTimer::singleShot(0, m_choiceButtons[0], SLOT(setFocus()));
    }
}

void MultiplechoiceModeWidget::setNumberOfRadioButtons(const int numberOfChoices)
{
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    m_ui->gridLayout->addLayout(verticalLayout, 2, 0);

    for (int i = 0; i < numberOfChoices; i++) {
        QRadioButton *radio_button = new QRadioButton(this);
        verticalLayout->addWidget(radio_button);
        m_choiceButtons.append(radio_button);
        if (i < 5) {
            connect(m_actions.at(i), SIGNAL(triggered()), radio_button, SLOT(click()));
        }
        connect(radio_button, &QRadioButton::clicked, this, &MultiplechoiceModeWidget::continueAction);
        radio_button->installEventFilter(this);
    }
}

void MultiplechoiceModeWidget::setSynonym(const QString& /*entry*/)
{
    //TODO Do something here to show synonyms
}


void MultiplechoiceModeWidget::showSynonym()
{
    //TODO Do something here to show synonyms
}

bool MultiplechoiceModeWidget::eventFilter(QObject *obj, QEvent *event)
{
    // make it possible to use the enter key to select multiple choice options
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            QRadioButton *radioButton = qobject_cast<QRadioButton*>(obj);
            if (radioButton) {
                radioButton->click();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void MultiplechoiceModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toInt();
}

void MultiplechoiceModeWidget::setHint(const QVariant& hint)
{
    m_choiceButtons.at(hint.toInt())->setEnabled(false);
}

void MultiplechoiceModeWidget::setFeedback(const QVariant& feedback)
{
    m_ui->feedbackLabel->setText(feedback.toString());
}

void MultiplechoiceModeWidget::showSolution()
{
    int input = -1;
    if (userInput().isValid()) {
        input = userInput().toInt();
    }
    m_choiceButtons[m_solution]->setPalette(m_correctPalette);
    if (input != -1 && input != m_solution) {
        m_choiceButtons[input]->setPalette(m_wrongPalette);
    }
    foreach(QRadioButton * radio, m_choiceButtons) {
        radio->setEnabled(false);
    }
    m_ui->solutionPronunciationLabel->setVisible(m_ui->solutionPronunciationLabel->isEnabled());
    m_ui->solutionSoundButton->setVisible(m_ui->solutionSoundButton->isEnabled());
}

QVariant MultiplechoiceModeWidget::userInput()
{

    int i = 0;
    foreach(QRadioButton * radio, m_choiceButtons) {
        if (radio->isChecked()) return i;
        i++;
    }

    return QVariant();
}

void MultiplechoiceModeWidget::setQuestionSound(const QUrl& soundUrl)
{
    m_ui->questionSoundButton->setSoundFile(soundUrl);
}

void MultiplechoiceModeWidget::setSolutionSound(const QUrl& soundUrl)
{
    m_ui->solutionSoundButton->setSoundFile(soundUrl);
}

void MultiplechoiceModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{
    m_ui->solutionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->solutionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

void MultiplechoiceModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{
    m_ui->questionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->questionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}
