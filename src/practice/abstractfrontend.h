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

#ifndef PRACTICE_ABSTRACTFRONTEND_H
#define PRACTICE_ABSTRACTFRONTEND_H

#include <QtCore/qobject.h>
#include <QPixmap>

namespace Practice {

class AbstractFrontend : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        FlashCard,
        MixedLetters,
        MultipleChoice,
        Written
    };

    enum ResultState {
        QuestionState,
        AnswerCorrect,
        AnswerWrong
    };
    
    AbstractFrontend(QObject* parent = 0);
    virtual ~AbstractFrontend() {}

    /**
     * Enables access to the input of the user.
     * This is queried by the backend when it needs to evaluate the input.
     */
    virtual QVariant userInput() = 0;
    
    /** The status such as lesson or number of words has changed */
    virtual void setFinishedWordsTotalWords(int finished, int total) = 0;

    virtual void setQuestion(const QVariant& question) = 0;
    virtual void setSolution(const QVariant& solution) = 0;
    virtual void setHint(const QVariant& hint) = 0;
    virtual void setQuestionImage(const QPixmap& img) = 0;
    virtual void setSolutionImage(const QPixmap& img) = 0;
    virtual void setLessonName(const QString& lesson) = 0;
    virtual void setResultState(ResultState resultState) = 0;
    
public Q_SLOTS:
    /** enter question mode - the user is asked to provide the solution */
    virtual void showQuestion() = 0;
    /** enter show solution mode - the solution is shown */
    virtual void showSolution() = 0;
    /** switch between different modes such as written, flash card, etc */
    virtual void setMode(Mode mode) = 0;

Q_SIGNALS:
    void signalContinueButton();
    void hintAction();
    void skipAction();
    
};

}

#endif // PRACTICE_ABSTRACTFRONTEND_H
