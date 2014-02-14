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


#ifndef FLASHCARDBACKENDMODE_H
#define FLASHCARDBACKENDMODE_H

#include "abstractbackendmode.h"

namespace Practice
{

class FlashCardBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    FlashCardBackendMode(const PracticeOptions& PracticeOptions, AbstractFrontend *frontend, QObject *parent);
    virtual bool setTestEntry(TestEntry* current);

public Q_SLOTS:
    virtual void checkAnswer();
    virtual void hintAction();

private:
    bool m_solutionVisible;
    TestEntry *m_current;
    QString m_currentHint;
};

}

#endif // FLASHCARDBACKENDMODE_H
