/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_ABSTRACTFRONTEND_H
#define PRACTICE_ABSTRACTFRONTEND_H

#include <QObject>
#include <KEduVocText>
#include <QFont>

class QUrl;

namespace Practice
{

class AbstractFrontend : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        None,
        FlashCard,
        MixedLetters,
        MultipleChoice,
        Written,
        Conjugation,
        Comparison,
        ExampleSentence
    };

    enum ResultState {
        QuestionState,
        AnswerCorrect,
        AnswerSynonym,
        AnswerWrong
    };

    explicit AbstractFrontend(QObject* parent = 0);
    virtual ~AbstractFrontend() {}

    /**
     * Enables access to the input of the user.
     * This is queried by the backend when it needs to evaluate the input.
     */
    virtual QVariant userInput() = 0;

    /** The status such as lesson or number of words has changed */
    virtual void setFinishedWordsTotalWords(int finished, int total) = 0;

    /** fonts for learning and known languages. These will be used to
     * set question and answer fonts for individual entries.
     */
    virtual QFont knownLangFont() const = 0;
    virtual QFont learningLangFont() const = 0;
    virtual void setKnownLangFont(const QFont& font) = 0;
    virtual void setLearningLangFont(const QFont& font) = 0;

    virtual void setQuestion(const QVariant& question) = 0;
    virtual void setSolution(const QVariant& solution) = 0;
    virtual void setFeedback(const QVariant& feedback) = 0;

    virtual void setQuestionFont(const QFont& font) = 0;
    virtual void setSolutionFont(const QFont& font) = 0;

    virtual void setHint(const QVariant& hint) = 0;
    virtual void setQuestionImage(const QUrl& img) = 0;
    virtual void setSolutionImage(const QUrl& img) = 0;
    virtual void setQuestionSound(const QUrl& soundUrl) = 0;
    virtual void setSolutionSound(const QUrl& soundUrl) = 0;
    virtual void setSolutionPronunciation(const QString& pronunciationText) = 0;
    virtual void setQuestionPronunciation(const QString& pronunciationText) = 0;

    virtual void setLessonName(const QString& lesson) = 0;
    virtual void showGrade(int preGrade, int grade) = 0;

    /** The feedback state tells the user if the currently entered word is correct
        (independent of whether the word is counted as correct) **/
    virtual void setFeedbackState(ResultState feedbackState) = 0;

    /** The result state indicated whether a word is counted as correct (and grades are raised)
        and can be changed by the user. **/
    virtual void setResultState(ResultState resultState) = 0;
    virtual ResultState resultState() = 0;

    /** set a new synonym that should be shown */
    virtual void setSynonym(const QString& entry) = 0;

public Q_SLOTS:
    /** enter question mode - the user is asked to provide the solution */
    virtual void showQuestion() = 0;
    /** enter show solution mode - the solution is shown */
    virtual void showSolution() = 0;

    /** show the synonyms */
    virtual void showSynonym() = 0;

    /** switch between different modes such as written, flash card, etc */
    virtual void setMode(Mode mode) = 0;
    virtual void setBoxes(grade_t currentBox, grade_t newBoxIfCorrect, grade_t newBoxIfWrong) = 0;

Q_SIGNALS:
    void continueAction();
    void hintAction();
    void skipAction();
    /** request to stop practicing */
    void stopPractice();
};

}

#endif // PRACTICE_ABSTRACTFRONTEND_H
