/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef FLASHCARDMODEWIDGET_H
#define FLASHCARDMODEWIDGET_H

#include "abstractwidget.h"

#include <QVariant>


namespace Ui
{
class FlashCardPracticeWidget;
}

namespace Practice
{
class LatexRenderer;


class FlashCardModeWidget: public AbstractModeWidget
{
    Q_OBJECT

public:
    explicit FlashCardModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    QVariant userInput() override;

    void setQuestion(const QVariant& question) override;
    void setSolution(const QVariant& solution) override;
    void setQuestionFont(const QFont& font) override;
    void setSolutionFont(const QFont& font) override;
    void setHint(const QVariant& hint) override;
    void setFeedback(const QVariant& feedback) override {
        Q_UNUSED(feedback);
    }
    void setFeedbackState(AbstractFrontend::ResultState feedbackState) override {
        Q_UNUSED(feedbackState);
    }
    void setResultState(AbstractFrontend::ResultState resultState) override {
        Q_UNUSED(resultState);
    }

    void setQuestionSound(const QUrl& soundUrl) override;
    void setSolutionSound(const QUrl& soundUrl) override;
    void setSolutionPronunciation(const QString& pronunciationText) override;
    void setQuestionPronunciation(const QString& pronunciationText) override;

    void setSynonym(const QString& entry) override;

public Q_SLOTS:
    void showQuestion() override;
    void showSolution() override;
    void showSynonym() override;

private:
    Ui::FlashCardPracticeWidget* m_ui;
    QString m_solution;
    LatexRenderer *m_questionLatexRenderer;
    LatexRenderer *m_solutionLatexRenderer;
};

}

#endif // FLASHCARDMODEWIDGET_H

