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


#ifndef EXAMPLESENTENCEBACKENDMODE_H
#define EXAMPLESENTENCEBACKENDMODE_H

#include "writtenbackendmode.h"

namespace Practice {

class ExampleSentenceBackendMode : public WrittenBackendMode
{
    Q_OBJECT
public:
    ExampleSentenceBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend *frontend, QObject *parent);
    
    virtual void setTestEntry(TestEntry* current);
};

}

#endif

