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


#ifndef MULTIPLECHOICEBACKENDMODE_H
#define MULTIPLECHOICEBACKENDMODE_H

#include "abstractbackendmode.h"

namespace Practice {

class MultipleChoiceBackendMode : public AbstractBackendMode
{
    Q_OBJECT
    
public:
    MultipleChoiceBackendMode(const PracticeOptions& PracticeOptions, AbstractFrontend *frontend, QObject *parent);
    
    virtual void setTestEntry(TestEntry* current);

public Q_SLOTS:
    virtual void continueAction();
    virtual void hintAction();
    
private:
    bool m_solutionVisible;
};

}

#endif
