/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_GUIFRONTEND_H
#define PRACTICE_GUIFRONTEND_H

#include "abstractwidget.h"
#include "abstractfrontend.h"

#include <KXmlGuiWindow>
#include <KUrl>

#include "practicemainwindow.h"

namespace Ui
{
class PracticeMainWindow;
}

namespace Practice
{
class ThemedBackgroundRenderer;
class ImageWidget;

class GuiFrontend : public AbstractFrontend
{
    Q_OBJECT

public:
    GuiFrontend(QWidget* parent = 0);
    virtual ~GuiFrontend();

    virtual QVariant userInput();
    QWidget* widget();

    QFont knownLangFont() const;
    QFont learningLangFont() const;
    void setKnownLangFont(const QFont& font);
    void setLearningLangFont(const QFont& font);

    virtual void setQuestion(const QVariant& question);
    virtual void setQuestionImage(const KUrl& img);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setQuestionFont(const QFont& font);

    virtual void setSolution(const QVariant& solution);
    virtual void setSolutionImage(const KUrl& img);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionFont(const QFont& font);

    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);

    virtual void setFeedbackState(ResultState feedbackState);
    virtual void setResultState(ResultState resultState);
    virtual ResultState resultState();

    /// update lesson label
    void setLessonName(const QString& lessonName);
    void setGradeString(const QString& gradeString);
    virtual void setFinishedWordsTotalWords(int finished, int total);

    virtual void setSynonym(const QString& entry);

public Q_SLOTS:
    void setMode(Mode mode);
    void showQuestion();
    void showSolution();
    void setBoxes(grade_t currentBox, grade_t newBoxIfCorrect, grade_t newBoxIfWrong);

    void showSynonym();

    void backgroundChanged(const QPixmap& pixmap);

    // show two buttons (i didn't know it, i knew it) instead of the continue button and disable the correct/wrong toggle (needed for flash card mode)
    void showSetResultButtons(bool show);
    void toggleResultState();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private Q_SLOTS:
    void countAsCorrectButtonClicked();
    void countAsWrongButtonClicked();
    void updateBackground();
    void updateFontColors();
    void setTheme();

private:
    void setImage(const KUrl& image);

    ImageWidget* m_widget;
    Ui::PracticeMainWindow* m_ui;
    AbstractModeWidget* m_modeWidget;
    ResultState m_resultState;
    ResultState m_feedbackState;
    KUrl m_lastImage;
    KUrl m_questionImage;
    KUrl m_solutionImage;
    int m_currentBox;
    int m_newBoxIfCorrect;
    int m_newBoxIfWrong;
    ThemedBackgroundRenderer *m_themedBackgroundRenderer;

    QFont m_knownLangFont;
    QFont m_learningLangFont;
};

}

#endif // PRACTICE_GUIFRONTEND_H
