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

#ifndef ABSTRACTBACKENDMODE_H
#define ABSTRACTBACKENDMODE_H

#include "practiceold/testentry.h"
#include "practiceoptions.h"
#include "abstractfrontend.h"

#include <QVariant>

namespace Practice {
    
class AbstractBackendMode :public QObject
{
    Q_OBJECT
    
public:
    AbstractBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend *frontend, QObject *parent);
    virtual ~AbstractBackendMode() {}
    
    /** start practicing a new word. sets some default that can be overwritten by the modes.
      * m_frontend->showQuestion() should be called after the initialization. */
    virtual void setTestEntry(TestEntry* current);
    
public Q_SLOTS:
    /** the frontend requested to continue */
    virtual void continueAction() = 0;
    /** the frontend requested a hint */
    virtual void hintAction() = 0;

Q_SIGNALS:
    /** the current word is done and should not be shown again */
    void currentEntryFinished();
    /** ask for the next word to be practiced */
    void nextEntry();
    
protected:
    PracticeOptions m_practiceOptions;
    AbstractFrontend* m_frontend;
    TestEntry* m_current;
};

}

#endif // ABSTRACTBACKENDMODE_H
