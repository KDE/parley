/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "multiplechoicemodewidget.h"
#include "guifrontend.h"
#include "latexrenderer.h"
#include "multiplechoicedata.h"

#include "ui_practice_widget_multiplechoice.h"

#include <KColorScheme>
#include <QAction>
#include <QDebug>
#include <QKeyEvent>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

using namespace Practice;

MultiplechoiceModeWidget::MultiplechoiceModeWidget(GuiFrontend *frontend, QWidget *parent)
    : AbstractModeWidget(frontend, parent)
    , m_latexRenderer(nullptr)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        QAction *action = new QAction(this);
        action->setShortcut(Qt::Key_1 + i);
        addAction(action);
        m_actions.append(action);
    }
    connect(frontend, &AbstractFrontend::continueAction, this, &AbstractModeWidget::stopAudio);
    connect(frontend, &AbstractFrontend::skipAction, this, &AbstractModeWidget::stopAudio);
}

void MultiplechoiceModeWidget::setQuestionFont(const QFont &font)
{
    m_ui->questionLabel->setFont(font);
}

void MultiplechoiceModeWidget::setSolutionFont(const QFont &font)
{
    m_solutionFont = font;
    resetButtonStyleSheet();
}

void MultiplechoiceModeWidget::setQuestion(const QVariant &question)
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
        setNumberOfPushButtons(data.choices.size());
    }

    int j = 0;
    for (QPushButton *pushButton : std::as_const(m_choiceButtons)) {
        pushButton->setText(data.choices[j]);
        pushButton->setToolTip(data.choices[j]);
        pushButton->setFont(m_solutionFont);
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

    resetButtonStyleSheet();

    if (!m_choiceButtons.isEmpty()) {
        for (QPushButton *pushButton : std::as_const(m_choiceButtons)) {
            pushButton->setChecked(false);
            pushButton->setEnabled(true);
        }

        QTimer::singleShot(0, m_choiceButtons[0], SLOT(setFocus()));
    }
}

void MultiplechoiceModeWidget::setNumberOfPushButtons(const int numberOfChoices)
{
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    m_ui->gridLayout->addLayout(verticalLayout, 2, 0);

    for (int i = 0; i < numberOfChoices; i++) {
        QHBoxLayout *horizontalLayout = new QHBoxLayout();
        verticalLayout->addLayout(horizontalLayout);

        // Display number of entry
        QLabel *label = new QLabel(QString::number(i + 1) + QStringLiteral(":"), this);
        horizontalLayout->addWidget(label);

        // Button displaying choice
        QPushButton *pushButton = new QPushButton(this);
        pushButton->setCheckable(true);
        pushButton->setFlat(true);
        pushButton->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
        m_choiceButtons.append(pushButton);
        if (i < 5) {
            connect(m_actions.at(i), &QAction::triggered, pushButton, &QAbstractButton::click);
        }
        connect(pushButton, &QPushButton::clicked, this, &MultiplechoiceModeWidget::continueAction);
        pushButton->installEventFilter(this);
        horizontalLayout->addWidget(pushButton);

        // Spacer to align button to the left
        horizontalLayout->addStretch(1);
    }
}

void MultiplechoiceModeWidget::setSynonym(const QString & /*entry*/)
{
    // TODO Do something here to show synonyms
}

void MultiplechoiceModeWidget::showSynonym()
{
    // TODO Do something here to show synonyms
}

bool MultiplechoiceModeWidget::eventFilter(QObject *obj, QEvent *event)
{
    // make it possible to use the enter key to select multiple choice options
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            QPushButton *pushButton = qobject_cast<QPushButton *>(obj);
            if (pushButton) {
                pushButton->click();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void MultiplechoiceModeWidget::setSolution(const QVariant &solution)
{
    m_solution = solution.toInt();
}

void MultiplechoiceModeWidget::setHint(const QVariant &hint)
{
    m_choiceButtons.at(hint.toInt())->setEnabled(false);
}

void MultiplechoiceModeWidget::setFeedback(const QVariant &feedback)
{
    m_ui->feedbackLabel->setText(feedback.toString());
}

void MultiplechoiceModeWidget::showSolution()
{
    int input = -1;
    if (userInput().isValid()) {
        input = userInput().toInt();
    }

    const QColor textColor = palette().color(QPalette::WindowText);
    // Set border to text color with light transparency
    const QString borderColor = QStringLiteral("#90") + palette().color(QPalette::WindowText).name().remove(0, 1);
    // Set background to correct color, but with transparency
    const QString correctBackground = QStringLiteral("#7D") + m_correctPalette.color(QPalette::Text).name().remove(0, 1);

    m_choiceButtons[m_solution]->setStyleSheet(m_choiceButtons[m_solution]->styleSheet()
                                               + QStringLiteral(" QPushButton:checked { "
                                                                "color: ")
                                               + textColor.name()
                                               + QStringLiteral("; "
                                                                "border-color: ")
                                               + borderColor
                                               + QStringLiteral("; "
                                                                "background-color: ")
                                               + correctBackground + QStringLiteral(" }"));
    // Always check correct button to highlight correct answer
    m_choiceButtons[m_solution]->setChecked(true);

    if (input != -1 && input != m_solution) {
        const QString wrongBackground = QStringLiteral("#7D") + m_wrongPalette.color(QPalette::Text).name().remove(0, 1);

        m_choiceButtons[input]->setStyleSheet(m_choiceButtons[input]->styleSheet()
                                              + QStringLiteral(" QPushButton:checked { "
                                                               "color: ")
                                              + textColor.name()
                                              + QStringLiteral("; "
                                                               "border-color: ")
                                              + borderColor
                                              + QStringLiteral("; "
                                                               "background-color: ")
                                              + wrongBackground + QStringLiteral(" }"));
    }
    for (QPushButton *pushButton : std::as_const(m_choiceButtons)) {
        pushButton->setEnabled(false);
    }
    m_ui->solutionPronunciationLabel->setVisible(m_ui->solutionPronunciationLabel->isEnabled());
    m_ui->solutionSoundButton->setVisible(m_ui->solutionSoundButton->isEnabled());
}

QVariant MultiplechoiceModeWidget::userInput()
{
    int i = 0;
    for (QPushButton *pushButton : std::as_const(m_choiceButtons)) {
        if (pushButton->isChecked())
            return i;
        i++;
    }

    return QVariant();
}

void MultiplechoiceModeWidget::setQuestionSound(const QUrl &soundUrl)
{
    m_ui->questionSoundButton->setSoundFile(soundUrl);
}

void MultiplechoiceModeWidget::setSolutionSound(const QUrl &soundUrl)
{
    m_ui->solutionSoundButton->setSoundFile(soundUrl);
}

void MultiplechoiceModeWidget::setSolutionPronunciation(const QString &pronunciationText)
{
    m_ui->solutionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->solutionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

void MultiplechoiceModeWidget::setQuestionPronunciation(const QString &pronunciationText)
{
    m_ui->questionPronunciationLabel->setText('[' + pronunciationText + ']');
    m_ui->questionPronunciationLabel->setEnabled(!pronunciationText.isNull());
}

void MultiplechoiceModeWidget::resetButtonStyleSheet()
{
    // Define default QPushButton StyleSheet
    const QColor textColor = palette().color(QPalette::WindowText);
    // Set border to text color with light transparency
    const QString borderColor = QStringLiteral("#90") + palette().color(QPalette::WindowText).name().remove(0, 1);
    const QString defaultStyleSheet = QStringLiteral(
                                          "QPushButton { text-align: left; "
                                          "color: ")
        + textColor.name()
        + QStringLiteral("; "
                         "padding: 5px; "
                         "border-color: #00FFFFFF; " // Make border transparent
                         "border-style: solid; "
                         "border-width: 1px; "
                         "border-radius: 4px; "
                         "font-style: ")
        + m_solutionFont.styleName() + QStringLiteral("; ") + QStringLiteral("font-weight: ") + QString::number(m_solutionFont.weight()) + QStringLiteral("; ")
        + QStringLiteral("font-size: ") + QString::number(m_solutionFont.pointSize()) + QStringLiteral("pt } ")
        + QStringLiteral("QPushButton:hover { "
                         "border-color: ")
        + borderColor
        + QStringLiteral(" } "
                         "QPushButton:focus { "
                         "color: ")
        + textColor.name()
        + QStringLiteral(" } "
                         "QPushButton:focus:!hover { "
                         "border-style: dashed; "
                         "border-color: ")
        + borderColor + QStringLiteral(" }");

    if (!m_choiceButtons.isEmpty()) {
        for (QPushButton *pushButton : std::as_const(m_choiceButtons)) {
            pushButton->setStyleSheet(defaultStyleSheet);
        }
    }
}

#include "moc_multiplechoicemodewidget.cpp"
