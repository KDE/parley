/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_GUIFRONTEND_H
#define PRACTICE_GUIFRONTEND_H

#include "abstractwidget.h"
#include "abstractfrontend.h"

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
    explicit GuiFrontend(QWidget *parent = 0);
    virtual ~GuiFrontend();

    QVariant userInput() Q_DECL_OVERRIDE;
    QWidget* widget();

    QFont knownLangFont() const Q_DECL_OVERRIDE;
    QFont learningLangFont() const Q_DECL_OVERRIDE;
    void setKnownLangFont(const QFont& font) Q_DECL_OVERRIDE;
    void setLearningLangFont(const QFont& font) Q_DECL_OVERRIDE;

    void setQuestion(const QVariant& question) Q_DECL_OVERRIDE;
    void setQuestionImage(const QUrl& img) Q_DECL_OVERRIDE;
    void setQuestionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;
    void setQuestionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setQuestionFont(const QFont& font) Q_DECL_OVERRIDE;

    void setSolution(const QVariant& solution) Q_DECL_OVERRIDE;
    void setSolutionImage(const QUrl& img) Q_DECL_OVERRIDE;
    void setSolutionPronunciation(const QString& pronunciationText) Q_DECL_OVERRIDE;
    void setSolutionSound(const QUrl& soundUrl) Q_DECL_OVERRIDE;
    void setSolutionFont(const QFont& font) Q_DECL_OVERRIDE;

    void setHint(const QVariant& hint) Q_DECL_OVERRIDE;
    void setFeedback(const QVariant& feedback) Q_DECL_OVERRIDE;

    void setFeedbackState(ResultState feedbackState) Q_DECL_OVERRIDE;
    void setResultState(ResultState resultState) Q_DECL_OVERRIDE;
    ResultState resultState() Q_DECL_OVERRIDE;

    /// update lesson label
    void setLessonName(const QString& lessonName) Q_DECL_OVERRIDE;
    void showGrade(int preGrade, int grade) Q_DECL_OVERRIDE;
    void setFinishedWordsTotalWords(int finished, int total) Q_DECL_OVERRIDE;

    void setSynonym(const QString& entry) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void setMode(Mode mode) Q_DECL_OVERRIDE;
    void showQuestion() Q_DECL_OVERRIDE;
    void showSolution() Q_DECL_OVERRIDE;
    void setBoxes(grade_t currentBox, grade_t newBoxIfCorrect, grade_t newBoxIfWrong) Q_DECL_OVERRIDE;

    void showSynonym() Q_DECL_OVERRIDE;

    void backgroundChanged(const QPixmap& pixmap);

    // show two buttons (i didn't know it, i knew it) instead of the continue button and disable the correct/wrong toggle (needed for flash card mode)
    void showSetResultButtons(bool show);
    void toggleResultState();

protected:
    bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void countAsCorrectButtonClicked();
    void countAsWrongButtonClicked();
    void updateBackground();
    void updateFontColors();
    void setTheme();

private:
    void setImage(const QUrl& image);

    ImageWidget *m_widget;
    Ui::PracticeMainWindow *m_ui;
    AbstractModeWidget *m_modeWidget;
    ResultState m_resultState;
    ResultState m_feedbackState;
    QUrl m_lastImage;
    QUrl m_questionImage;
    QUrl m_solutionImage;
    int m_currentBox;
    int m_newBoxIfCorrect;
    int m_newBoxIfWrong;
    ThemedBackgroundRenderer *m_themedBackgroundRenderer;

    QFont m_knownLangFont;
    QFont m_learningLangFont;
};

}

#endif // PRACTICE_GUIFRONTEND_H
