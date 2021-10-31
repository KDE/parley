/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_GUIFRONTEND_H
#define PRACTICE_GUIFRONTEND_H

#include "abstractfrontend.h"
#include "abstractwidget.h"

#include <KXmlGuiWindow>
#include <QUrl>

#include "practicemainwindow.h"

///@brief classes generated from the .ui XML files
namespace Ui
{
class PracticeMainWindow;
}

namespace Practice
{
class ThemedBackgroundRenderer;
class ImageWidget;

/** GuiFrontend **/
class GuiFrontend : public AbstractFrontend
{
    Q_OBJECT

public:
    explicit GuiFrontend(QWidget *parent = nullptr);
    ~GuiFrontend() override;

    QVariant userInput() override;
    QWidget *widget();

    QFont knownLangFont() const override;
    QFont learningLangFont() const override;
    void setKnownLangFont(const QFont &font) override;
    void setLearningLangFont(const QFont &font) override;

    void setQuestion(const QVariant &question) override;
    void setQuestionImage(const QUrl &img) override;
    void setQuestionPronunciation(const QString &pronunciationText) override;
    void setQuestionSound(const QUrl &soundUrl) override;
    void setQuestionFont(const QFont &font) override;

    void setSolution(const QVariant &solution) override;
    void setSolutionImage(const QUrl &img) override;
    void setSolutionPronunciation(const QString &pronunciationText) override;
    void setSolutionSound(const QUrl &soundUrl) override;
    void setSolutionFont(const QFont &font) override;

    void setHint(const QVariant &hint) override;
    void setFeedback(const QVariant &feedback) override;

    void setFeedbackState(ResultState feedbackState) override;
    void setResultState(ResultState resultState) override;
    ResultState resultState() override;

    /// update lesson label
    void setLessonName(const QString &lessonName) override;
    void showGrade(int preGrade, int grade) override;
    void setFinishedWordsTotalWords(int finished, int total) override;

    void setSynonym(const QString &entry) override;

public Q_SLOTS:
    void setMode(Practice::AbstractFrontend::Mode mode) override;
    void showQuestion() override;
    void showSolution() override;
    void setBoxes(grade_t currentBox, grade_t newBoxIfCorrect, grade_t newBoxIfWrong) override;

    void showSynonym() override;

    void backgroundChanged(const QPixmap &pixmap);

    // show two buttons (i didn't know it, i knew it) instead of the continue button and disable the correct/wrong toggle (needed for flash card mode)
    void showSetResultButtons(bool show);
    void toggleResultState();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private Q_SLOTS:
    void countAsCorrectButtonClicked();
    void countAsWrongButtonClicked();
    void updateBackground();
    void updateFontColors();
    void setTheme();

private:
    void setImage(const QUrl &image);

    ImageWidget *m_widget{nullptr};
    Ui::PracticeMainWindow *m_ui{nullptr};
    AbstractModeWidget *m_modeWidget{nullptr};
    ResultState m_resultState;
    ResultState m_feedbackState;
    QUrl m_lastImage;
    QUrl m_questionImage;
    QUrl m_solutionImage;
    int m_currentBox{0};
    int m_newBoxIfCorrect{0};
    int m_newBoxIfWrong{0};
    ThemedBackgroundRenderer *m_themedBackgroundRenderer{nullptr};

    QFont m_knownLangFont;
    QFont m_learningLangFont;
};

}

#endif // PRACTICE_GUIFRONTEND_H
