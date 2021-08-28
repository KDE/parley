/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MULTIPLECHOICEMODEWIDGET_H
#define MULTIPLECHOICEMODEWIDGET_H

#include "abstractwidget.h"

namespace Ui
{
class MultiplechoicePracticeWidget;
}
class QPushButton;

namespace Practice
{
class LatexRenderer;

class MultiplechoiceModeWidget : public AbstractModeWidget
{
    Q_OBJECT

public:
    explicit MultiplechoiceModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
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

    void setSynonym(const QString &entry) override;

public Q_SLOTS:
    void showQuestion() override;
    void showSolution() override;
    virtual void setNumberOfPushButtons(const int numberOfChoices);
    void showSynonym() override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void resetButtonStyleSheet();

private:
    Ui::MultiplechoicePracticeWidget *m_ui;
    int m_solution;
    QList<QPushButton *> m_choiceButtons;
    QList<QAction *> m_actions;
    LatexRenderer *m_latexRenderer;
    QFont m_solutionFont;
};

}

#endif // FLASHCARDMODEWIDGET_H
