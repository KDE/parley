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
    enum Mode {
        FlashCard,
        MixedLetters,
        MultipleChoice,
        Written
    };
    
    AbstractBackend(QObject *parent = 0);
    virtual ~AbstractBackend() {}
    
    /** The type of practice to be displayed, such as multiple choice or written */
    virtual Mode mode() = 0;
    
    /** The word that is displayed to the user */
    virtual QVariant question() = 0;
    /** The translation of the question */
    virtual QVariant solution() = 0;    
    /** Pronunciation of the question [in text form] */
    virtual QString questionPronunciation() = 0;
    /** Pronunciation of the solution [in text form] */
    virtual QString solutionPronunciation() = 0;
    
    /** Wether the front end should allow the user to enter a solution or is in "read only" mode when displaying the solution */
    virtual bool acceptUserInput() = 0;

    /** The number of finished entries that will not be asked in this practice session again */
    virtual int practicedEntryCount() = 0;
    /** Total of entries to practice */
    virtual int totalEntryCount() = 0;
    /** The current grade of the entry */
    virtual int currentBox() = 0;
    /** The new grade */
    virtual int previousBox() = 0;
    /** Name of the lesson the currently practiced word is in */
    virtual QString lessonName() = 0;
    
public slots:
    /** continue was selected by the user (button clicked) */
    virtual void continueAction() = 0;
    /** the user requested a hint */
    virtual void hintAction() = 0;
    /** skip the current word (show it again later) */
    virtual void skipAction() = 0;

signals:
    /** something changed, update the frontend */
    void updateDisplay();
    /** display a different test type */
    void modeChanged(AbstractBackend::Mode);
};

}

#endif // PRACTICE_ABSTRACTBACKEND_H
