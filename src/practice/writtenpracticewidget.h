/*
    SPDX-FileCopyrightText: 2009-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2009-2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_WRITTENPRACTICEWIDGET_H
#define PRACTICE_WRITTENPRACTICEWIDGET_H

#include "abstractwidget.h"

namespace Ui
{
class WrittenPracticeWidget;
}

namespace Practice
{
class LatexRenderer;

class WrittenPracticeWidget : public Practice::AbstractModeWidget
{
    Q_OBJECT

public:
    explicit WrittenPracticeWidget(GuiFrontend *frontend, QWidget *parent = nullptr, bool isExampleSentenceMode = false);
    QVariant userInput() override;

    void setQuestion(const QVariant &question) override;
    void setSolution(const QVariant &solution) override;

    void setQuestionFont(const QFont &font) override;
    void setSolutionFont(const QFont &font) override;
    void setHint(const QVariant &hint) override;
    void setFeedback(const QVariant &feedback) override;
    void setFeedbackState(AbstractFrontend::ResultState feedbackState) override;
    void setResultState(AbstractFrontend::ResultState resultState) override;

    void setQuestionSound(const QUrl &soundUrl) override;
    void setSolutionSound(const QUrl &soundUrl) override;
    void setSolutionPronunciation(const QString &pronunciationText) override;
    void setQuestionPronunciation(const QString &pronunciationText) override;

    void setSynonym(const QString &entry) override;

public slots:
    void showQuestion() override;
    void showSolution() override;
    void continueClicked();
    void showSynonym() override;

protected:
    Ui::WrittenPracticeWidget *m_ui{nullptr};

private:
    QString m_solution;
    QString m_synonym;
    AbstractFrontend::ResultState m_resultState;
    AbstractFrontend::ResultState m_feedbackState;
    QList<QWidget *> synonymWidgets;
    LatexRenderer *m_latexRenderer{nullptr};
    bool isExampleSentenceMode{false};
};

}

#endif // PRACTICE_WRITTENPRACTICEWIDGET_H
