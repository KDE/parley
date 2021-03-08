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

class MultiplechoiceModeWidget: public AbstractModeWidget
{
    Q_OBJECT

public:
    explicit MultiplechoiceModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    QVariant userInput() Q_DECL_OVERRIDE;

    void setQuestion(const QVariant& question) Q_DECL_OVERRIDE;
    void setSolution(const QVariant& solution) Q_DECL_OVERRIDE;
    void setHint(const QVariant& hint) Q_DECL_OVERRIDE;
    void setFeedback(const QVariant& feedback) Q_DECL_OVERRIDE;
    void setFeedbackState(AbstractFrontend::ResultState feedbackState) Q_DECL_OVERRIDE {
        Q_UNUSED(feedbackState);
    }
    void setResultState(AbstractFrontend::ResultState resultState) Q_DECL_OVERRIDE {
        Q_UNUSED(resultState);
    }

    void setQuestionFont(const QFont& font) Q_DECL_OVERRIDE;
    void setSolutionFont(const QFont& font) Q_DECL_OVERRIDE;
    void setQuestionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setSolutionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setSolutionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;
    void setQuestionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;

    void setSynonym(const QString& entry) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void showQuestion() Q_DECL_OVERRIDE;
    void showSolution() Q_DECL_OVERRIDE;
    virtual void setNumberOfPushButtons(const int numberOfChoices);
    void showSynonym() Q_DECL_OVERRIDE;

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    virtual void resetButtonStyleSheet();

private:
    Ui::MultiplechoicePracticeWidget* m_ui;
    int m_solution;
    QList<QPushButton*> m_choiceButtons;
    QList<QAction*> m_actions;
    LatexRenderer *m_latexRenderer;
    QFont m_solutionFont;
};

}

#endif // FLASHCARDMODEWIDGET_H

