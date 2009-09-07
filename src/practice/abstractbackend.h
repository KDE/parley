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

#ifndef PRACTICE_ABSTRACTBACKEND_H
#define PRACTICE_ABSTRACTBACKEND_H

#include <QtCore/qobject.h>

class QVariant;

namespace Practice {

class AbstractBackend : public QObject
{
    Q_OBJECT
public:
    AbstractBackend(QObject *parent = 0);

    enum Mode {Written, MultipleChoice, FlashCards, MixedLetters};
    enum ContinueReason { Default, AnswerLater, Hint };

    virtual Mode mode() = 0;
    virtual QVariant question() = 0;

    virtual int progress() = 0;
    virtual int progressTotal() = 0;
    virtual int currentBox() = 0;
    virtual int previousBox() = 0;
    virtual QString lessonName() = 0;
    virtual QVariant solution() = 0;
    virtual bool acceptUserInput() = 0;

public slots:
    virtual void continueAction(ContinueReason) = 0;

signals:
    void updateDisplay();
    void modeChanged(AbstractBackend::Mode);
};

}

#endif // PRACTICE_ABSTRACTBACKEND_H
