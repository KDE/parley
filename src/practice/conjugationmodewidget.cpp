/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "conjugationmodewidget.h"
#include "conjugationdata.h"

#include "ui_practice_widget_conjugation.h"

#include <QDebug>
#include <KColorScheme>
#include <KLocalizedString>
#include <QRadioButton>
#include <QTimer>

namespace Practice
{
struct PersonConjugationSolutionWidgets {
    QLabel* person;
    QLineEdit* input;
    QLabel* solution;

    PersonConjugationSolutionWidgets(QGridLayout* layout, QWidget* parent = 0) {
        person = new QLabel(parent);
        person->setAlignment(Qt::AlignRight);
        input = new QLineEdit(parent);
        solution = new QLabel(parent);
        int row = layout->rowCount();
        layout->addWidget(person, row, 0);
        layout->addWidget(input, row, 1);
        layout->addWidget(solution, row, 2);
    }

    void setVisible(bool visible) {
        person->setVisible(visible);
        input->setVisible(visible);
        solution->setVisible(visible);
    }
};
}

using namespace Practice;

ConjugationModeWidget::ConjugationModeWidget(GuiFrontend *frontend, QWidget* parent)
    : AbstractModeWidget(frontend, parent)
{
    m_ui = new Ui::ConjugationPracticeWidget();
    m_ui->setupUi(this);
}

ConjugationModeWidget::~ConjugationModeWidget()
{
    qDeleteAll(m_personWidgets);
}

void ConjugationModeWidget::setQuestionFont(const QFont& font)
{
    m_ui->questionLabel->setFont(font);
}

void ConjugationModeWidget::setSolutionFont(const QFont& font)
{
    m_ui->infinitiveEdit->setFont(font);
    m_solutionFont = font;
}

void ConjugationModeWidget::setQuestion(const QVariant& question)
{
    if (!question.canConvert<ConjugationData>()) {
        qWarning() << "expected ConjugationData";
        return;
    }
    ConjugationData data = question.value<ConjugationData>();

    // It's annoying to have to search for information, so put everything into one place if
    // there`s only one pronoun
    if (data.personalPronouns.size() > 1) {
        m_ui->tenseLabel->setText(data.tense);
    } else {
        m_ui->tenseLabel->clear();
    }

    m_ui->questionLabel->setText(data.questionInfinitive);
    m_ui->infinitiveEdit->setText(data.solutionInfinitive);

    setNumberOfConjugationWidgets(data.personalPronouns.size());
    int i = 0;
    foreach(QString pp, data.personalPronouns) {
        if (data.personalPronouns.size() == 1) {
            pp += " (" + data.tense + ')';
        }
        m_personWidgets.at(i)->person->setText(pp);
        m_personWidgets.at(i)->person->setFont(m_solutionFont);
        m_personWidgets.at(i)->input->clear();
        m_personWidgets.at(i)->input->setPalette(QApplication::palette());
        m_personWidgets.at(i)->input->setFont(m_solutionFont);
        m_personWidgets.at(i)->solution->clear();
        m_personWidgets.at(i)->solution->setFont(m_solutionFont);
        connect(m_personWidgets.at(i)->input, &QLineEdit::returnPressed, this, &ConjugationModeWidget::nextConjugationForm);
        ++i;
    }
}

void ConjugationModeWidget::showQuestion()
{
    Q_ASSERT(!m_personWidgets.isEmpty());
    QTimer::singleShot(0, m_personWidgets.at(0)->input, SLOT(setFocus()));
    m_ui->feedbackLabel->setText(i18n("Enter all conjugation forms."));
}

void ConjugationModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toStringList();
}

void ConjugationModeWidget::setFeedback(const QVariant& feedback)
{
    m_ui->feedbackLabel->setText(feedback.toString());
}

void ConjugationModeWidget::showSolution()
{
    for (int i = 0; i < m_solution.size() && i < m_personWidgets.size(); ++i) {
        if (m_personWidgets.at(i)->input->text() == m_solution.at(i)) {
            m_personWidgets.at(i)->input->setPalette(m_correctPalette);
        } else {
            m_personWidgets.at(i)->input->setPalette(m_wrongPalette);
            m_personWidgets.at(i)->solution->setPalette(m_correctPalette);
            m_personWidgets.at(i)->solution->setText(m_solution.at(i));
        }
    }
}

QVariant ConjugationModeWidget::userInput()
{
    QStringList answers;
    for (int i = 0; i < m_personWidgets.size(); ++i) {
        answers.append(m_personWidgets.at(i)->input->text());
    }
    return answers;
}

void ConjugationModeWidget::setHint(const QVariant& /*hint*/)
{
}

void ConjugationModeWidget::setNumberOfConjugationWidgets(const int numberOfForms)
{
    while (m_personWidgets.size() < numberOfForms) {
        PersonConjugationSolutionWidgets* w = new PersonConjugationSolutionWidgets(m_ui->conjugationsLayout, this);
        m_personWidgets.append(w);
    }
    for (int i = 0; i < m_personWidgets.size(); ++i) {
        m_personWidgets[i]->setVisible(i < numberOfForms);
    }
}

void ConjugationModeWidget::nextConjugationForm()
{
    for (int i = 0; i < m_personWidgets.count(); ++i) {
        if (sender() == m_personWidgets.at(i)->input) {
            if ((i + 1 < m_personWidgets.count()) && (m_personWidgets.at(i + 1)->input->isVisible())) {
                // move to the next input widget
                m_personWidgets.at(i + 1)->input->setFocus();
            } else {
                emit continueAction();
            }
            return;
        }
    }
}

void ConjugationModeWidget::setQuestionPronunciation(const QString& /*pronunciationText*/)
{
}

void ConjugationModeWidget::setSolutionPronunciation(const QString& /*pronunciationText*/)
{
}

void ConjugationModeWidget::setQuestionSound(const QUrl& /*soundUrl*/)
{

}
void ConjugationModeWidget::setSolutionSound(const QUrl& /*soundUrl*/)
{
}
