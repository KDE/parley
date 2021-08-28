/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONJUGATIONMODEWIDGET_H
#define CONJUGATIONMODEWIDGET_H

#include "abstractwidget.h"

namespace Ui
{
class ConjugationPracticeWidget;
}

namespace Practice
{
struct PersonConjugationSolutionWidgets;

class ConjugationModeWidget : public AbstractModeWidget
{
    Q_OBJECT

public:
    explicit ConjugationModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual ~ConjugationModeWidget();
    QVariant userInput() override;

    void setQuestion(const QVariant &question) override;
    void setSolution(const QVariant &solution) override;
    void setHint(const QVariant &hint) override;
    void setFeedback(const QVariant &feedback) override;
    void setFeedbackState(AbstractFrontend::ResultState feedbackState) override
    {
        Q_UNUSED(feedbackState);
    }
    void setResultState(AbstractFrontend::ResultState resultState) override
    {
        Q_UNUSED(resultState);
    }

    void setQuestionFont(const QFont &font) override;
    void setSolutionFont(const QFont &font) override;
    void setQuestionSound(const QUrl &soundUrl) override;
    void setSolutionSound(const QUrl &soundUrl) override;
    void setSolutionPronunciation(const QString &pronunciationText) override;
    void setQuestionPronunciation(const QString &pronunciationText) override;
    void setSynonym(const QString & /*entry*/) override
    {
    }

public Q_SLOTS:
    void showQuestion() override;
    void showSolution() override;
    void showSynonym() override
    {
    }

private Q_SLOTS:
    void nextConjugationForm();

private:
    virtual void setNumberOfConjugationWidgets(const int numberOfForms);

    Ui::ConjugationPracticeWidget *m_ui;
    QStringList m_solution;
    QList<PersonConjugationSolutionWidgets *> m_personWidgets;
    QFont m_solutionFont;
};

}

#endif
