/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PRACTICESTATEMACHINE_H
#define PRACTICESTATEMACHINE_H

#include <QObject>

#include "abstractbackendmode.h"

class KEduVocEditorDocument;

namespace Practice
{
class SessionManagerBase;

class PracticeStateMachine : public QObject
{
    Q_OBJECT
public:
    PracticeStateMachine(AbstractFrontend* frontend, KEduVocEditorDocument* doc,
                         SessionManagerBase* sessionManager,
                         QObject* parent = 0);
    void start();

Q_SIGNALS:
    void practiceFinished();
    void stopPractice();

public Q_SLOTS:
    void slotPracticeFinished();

private Q_SLOTS:
    /** Call when current entry is finished and next one should be selected */
    void nextEntry();

    void continueAction();

    // these come from the mode
    void answerRight();
    void answerWrongRetry();
    void answerWrongShowSolution();
    void showSolution();

    void gradeEntryAndContinue();

private:
    /** Set the current word as done, so it will not be repeated */
    void currentEntryFinished();
    void createPracticeMode();
    void updateFrontend();

    enum State {
        NotAnswered,
        AnswerWasWrong,
        SolutionShown
    };

    State m_state;

    AbstractFrontend* m_frontend;
    AbstractBackendMode* m_mode;
    KEduVocEditorDocument* m_document;
    TestEntry* m_current;
    SessionManagerBase* m_sessionManager;
};

}

#endif
