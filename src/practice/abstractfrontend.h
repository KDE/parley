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
    
    AbstractFrontend(QObject* parent = 0);
    virtual ~AbstractFrontend() {}

    /**
     * Enables access to the input of the user.
     * This is queried by the backend when it needs to evaluate the input.
     */
    virtual QVariant userInput() = 0;

    virtual void setQuestion(const QVariant& question) = 0;
    
    
public slots:
    virtual void showQuestion() = 0;
    virtual void showSolution() = 0;
    virtual void setMode(Mode mode) = 0;

signals:
    void continueAction();
    void hintAction();
    void skipAction();
};

}

#endif // PRACTICE_ABSTRACTFRONTEND_H
