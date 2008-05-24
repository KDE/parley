///\file input.h
// C++ Interface: input
//
// Description: Accepts user input and checks for correctness.
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef EDU_INPUT_H
#define EDU_INPUT_H

#include <QObject>
#include <QLineEdit>

#include "eduwidget.h"
#include "statistics.h"

#include "../../libkdeedu/keduvocdocument/keduvocexpression.h"
#include "../../libkdeedu/keduvocdocument/keduvoctranslation.h"


/**
* @class Input
* @author David Capel <wot.narg@gmail.com>
* @brief This is the base class for the input-collection mechanisms.
* It is provided the answer to each question and does the correction checking
* after the user has inputted their answer.
* This may be represented in a variety of forms; for example, a line edit or
* a set of labelled radio buttons.
*/

class Input
{
    public: // slots:
        /// Called to show the solution.
        virtual void slotShowAnswer() = 0;
        /// Called to check the users's answer against the correct answer.
        virtual void slotCheckAnswer() = 0;
        /// Called to set the correct answer to a given question.
        virtual void slotSetAnswer(KEduVocTranslation* answer) { m_answer = answer; };
        virtual ~Input();
        Input();
        
    protected:
        /// @return if the QStrings are the same.
        /// Does a smart comare of the two QStrings.
        bool smartCompare(const QString&, const QString&) const;

        /**
        * Use this to check if the answer is correct.
        * Use only to check the word itself, does not work on other properties like comparison forms etc.
        * @param userAnswer
        * @return a fuzzy answer where 1.0 is the correct solution and 0.0 completely wrong
        */
        double verifyAnswer(const QString& userAnswer);

        /**
        * Like verifyAnswer but for two strings, so the entry is not taken into consideration.
        * @param solution
        * @param userAnswer
        * @return
        */
        double verifyAnswer(const QString& solution, const QString& userAnswer);


    //signals:
    protected:
        /// Emitted if the answer was incorrect, with the ErrorType denoting why it was incorrect.
        void signalIncorrect(Statistics::ErrorType);
        /// Emitted if the answer was correct.
        void signalCorrect();


        KEduVocTranslation * m_answer;
};

class TextualInput : public QLineEdit, public Input
{

        Q_OBJECT

    public:
        TextualInput(QWidget * parent = 0);

    public slots:
        /// Called to show the final solution.
        void slotShowAnswer();
        /// Called to check the users's answer against the correct answer.
        void slotCheckAnswer();
        void slotSetAnswer(KEduVocTranslation* answer);

    signals:
        /// Emitted if the answer was incorrect, with the ErrorType denoting why it was incorrect.
        void signalIncorrect(Statistics::ErrorType);
        /// Emitted if the answer was correct.
        void signalCorrect();
};
#endif
