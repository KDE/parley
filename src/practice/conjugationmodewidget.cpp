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

#include "conjugationmodewidget.h"
#include "conjugationdata.h"

#include "ui_practice_widget_conjugation.h"

#include <KDebug>
#include <kcolorscheme.h>
#include <QtGui/QRadioButton>

namespace Practice {
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

ConjugationModeWidget::ConjugationModeWidget (GuiFrontend *frontend, QWidget* parent )
    : AbstractModeWidget (frontend, parent)
{
    m_ui = new Ui::ConjugationPracticeWidget();
    m_ui->setupUi(this);
    KColorScheme scheme(QPalette::Active);
    m_correctPalette = QApplication::palette();
    m_correctPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::PositiveText).color());
    m_correctPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::PositiveText).color());
    m_wrongPalette = QApplication::palette();
    m_wrongPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::NegativeText).color());
    m_wrongPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::NegativeText).color());
}

ConjugationModeWidget::~ConjugationModeWidget()
{
    qDeleteAll(m_personWidgets);
}

void ConjugationModeWidget::setQuestion(const QVariant& question)
{
    if (!question.canConvert<ConjugationData>()) {
        kWarning() << "expected ConjugationData";
        return;
    }
    ConjugationData data = question.value<ConjugationData>();
    m_ui->tenseLabel->setText(data.tense);
    m_ui->questionLabel->setText(data.questionInfinitive);
    m_ui->infinitiveEdit->setText(data.solutionInfinitive);

    setNumberOfConjugationWidgets(data.personalPronouns.size());
    int i = 0;
    foreach(const QString& pp, data.personalPronouns) {
        m_personWidgets.at(i)->person->setText(pp);
        m_personWidgets.at(i)->input->clear();
        m_personWidgets.at(i)->solution->clear();
        ++i;
    }
}

void ConjugationModeWidget::showQuestion()
{}

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
    for(int i = 0; i < m_solution.size() && i < m_personWidgets.size(); ++i) {
        m_personWidgets[i]->solution->setText(m_solution.at(i));
    }
}

QVariant ConjugationModeWidget::userInput()
{
    QStringList answers;
    for(int i = 0; i < m_personWidgets.size(); ++i) {
        answers.append(m_personWidgets.at(i)->input->text());
    }
    return answers;
}

void ConjugationModeWidget::setHint(const QVariant& hint)
{

}

void ConjugationModeWidget::setNumberOfConjugationWidgets(const int numberOfForms)
{
    while (m_personWidgets.size() < numberOfForms) {
        PersonConjugationSolutionWidgets* w = new PersonConjugationSolutionWidgets(m_ui->conjugationsLayout, this);
        m_personWidgets.append(w);
    }
    for(int i = 0; i < m_personWidgets.size(); ++i) {
        m_personWidgets[i]->setVisible(i < numberOfForms);
    }
}

void ConjugationModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{

}
void ConjugationModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{

}
void ConjugationModeWidget::setQuestionSound(const KUrl& soundUrl)
{

}
void ConjugationModeWidget::setSolutionSound(const KUrl& soundUrl)
{

}


#include "conjugationmodewidget.moc"
