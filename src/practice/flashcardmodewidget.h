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
    QVariant userInput() Q_DECL_OVERRIDE;

    void setQuestion(const QVariant& question) Q_DECL_OVERRIDE;
    void setSolution(const QVariant& solution) Q_DECL_OVERRIDE;
    void setQuestionFont(const QFont& font) Q_DECL_OVERRIDE;
    void setSolutionFont(const QFont& font) Q_DECL_OVERRIDE;
    void setHint(const QVariant& hint) Q_DECL_OVERRIDE;
    void setFeedback(const QVariant& feedback) Q_DECL_OVERRIDE {
        Q_UNUSED(feedback);
    }
    void setFeedbackState(AbstractFrontend::ResultState feedbackState) Q_DECL_OVERRIDE {
        Q_UNUSED(feedbackState);
    }
    void setResultState(AbstractFrontend::ResultState resultState) Q_DECL_OVERRIDE {
        Q_UNUSED(resultState);
    }

    void setQuestionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setSolutionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setSolutionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;
    void setQuestionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;

    void setSynonym(const QString& entry) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void showQuestion() Q_DECL_OVERRIDE;
    void showSolution() Q_DECL_OVERRIDE;
    void showSynonym() Q_DECL_OVERRIDE;

private:
    Ui::FlashCardPracticeWidget* m_ui;
    QString m_solution;
    LatexRenderer *m_questionLatexRenderer;
    LatexRenderer *m_solutionLatexRenderer;
};

}

#endif // FLASHCARDMODEWIDGET_H

