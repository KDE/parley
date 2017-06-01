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
    FlashCardBackendMode(AbstractFrontend *frontend, QObject *parent);
    bool setTestEntry(TestEntry* current) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void checkAnswer() Q_DECL_OVERRIDE;
    void hintAction() Q_DECL_OVERRIDE;

private:
    bool m_solutionVisible;
    TestEntry *m_current;
    QString m_currentHint;
};

}

#endif // FLASHCARDBACKENDMODE_H
