/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef WRITTENBACKENDMODE_H
#define WRITTENBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/testentrymanager.h"
#include "practice/writtenpracticevalidator.h"

namespace Practice {

class WrittenBackendMode : public AbstractBackendMode
{
    Q_OBJECT
    
public:
    WrittenBackendMode(const Practice::PracticeOptions& practiceOptions, Practice::AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager);
    
    virtual void setTestEntry(TestEntry* current);

public Q_SLOTS:
    virtual void continueAction();
    virtual void hintAction();
    
private:
    void checkAnswer();
    void markSynonymCorrect(const QString& synonym);
    void handleSynonym();
    
    
    enum State {
        NotAnswered,
        AnswerWasWrong,
        SolutionShown
    };
    
    State m_state;
    QString m_lastAnswer;
    QString m_currentHint;
    TestEntryManager* m_testEntryManager;
    WrittenPracticeValidator* m_validator;
};

}

#endif // WRITTENBACKENDMODE_H
