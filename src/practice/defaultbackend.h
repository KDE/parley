/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
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

#ifndef PRACTICE_DEFAULTBACKEND_H
#define PRACTICE_DEFAULTBACKEND_H

#include "abstractbackend.h"
#include "parleydocument.h"
#include "writtenbackendmode.h"
#include "flashcardbackendmode.h"


#include "practiceold/testentrymanager.h"
#include "practiceold/testentry.h"
#include "practiceoptions.h"

namespace Practice {

class DefaultBackend : public Practice::AbstractBackend
{
    Q_OBJECT

public:
    DefaultBackend(ParleyDocument *doc, const PracticeOptions& options, QObject *parent = 0);
    ~DefaultBackend();
    
    virtual QString lessonName();
    virtual int previousBox();
    virtual int currentBox();
    virtual int totalEntryCount();
    virtual int practicedEntryCount();
    virtual QVariant question();
    virtual QVariant solution();
    virtual QString questionPronunciation();
    virtual QString solutionPronunciation();
        
    virtual bool acceptUserInput();

    virtual Mode mode();
    
    
public slots:
    virtual void continueAction();
    virtual void hintAction();
    virtual void skipAction();
    
    void startPractice();
    void createPracticeMode();
    
private:
    TestEntryManager m_testEntryManager;
    TestEntry* m_current;
    PracticeOptions m_options;
    AbstractBackend::Mode m_currentMode;
    AbstractBackendMode* m_mode;
};

}

#endif
