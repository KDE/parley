/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPARISONMODEWIDGET_H
#define COMPARISONMODEWIDGET_H

#include "abstractwidget.h"

namespace Ui
{
class ComparisonPracticeWidget;
}

namespace Practice
{
class ComparisonModeWidget : public AbstractModeWidget
{
    Q_OBJECT

public:
    explicit ComparisonModeWidget(GuiFrontend *frontend, QWidget *parent = nullptr);
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
    void nextComparisonForm();

private:
    Ui::ComparisonPracticeWidget *m_ui{nullptr};
    QStringList m_solution;
};

}

#endif
