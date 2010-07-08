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
    WrittenBackendMode(const Practice::PracticeOptions& practiceOptions, Practice::AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc);

    virtual bool setTestEntry(TestEntry* current);

    void checkAnswer();

public Q_SLOTS:
    virtual void hintAction();

private:
    QString getFeedbackString(TestEntry::ErrorTypes error);

    QString m_lastAnswer;
    QString m_currentHint;
    bool m_firstAttempt;
    TestEntryManager* m_testEntryManager;
    WrittenPracticeValidator* m_validator;
    KEduVocDocument* m_doc;
};

}

#endif // WRITTENBACKENDMODE_H
